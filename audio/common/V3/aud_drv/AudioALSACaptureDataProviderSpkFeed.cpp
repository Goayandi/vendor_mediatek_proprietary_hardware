#include "AudioALSACaptureDataProviderSpkFeed.h"

#include <pthread.h>

#include <sys/prctl.h>

#include "AudioType.h"
#include "AudioALSAHardwareResourceManager.h"
#include "AudioALSADeviceString.h"
#include "AudioALSADriverUtility.h"
#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "AudioALSACaptureDataProviderSpkFeed"

namespace android {


/*==============================================================================
 *                     Constant
 *============================================================================*/

static const uint32_t kReadBufferSize = 6144; // 6k


/*==============================================================================
 *                     Implementation
 *============================================================================*/

AudioALSACaptureDataProviderSpkFeed *AudioALSACaptureDataProviderSpkFeed::mAudioALSACaptureDataProviderSpkFeed = NULL;
AudioALSACaptureDataProviderSpkFeed *AudioALSACaptureDataProviderSpkFeed::getInstance() {
    static AudioLock mGetInstanceLock;
    AL_AUTOLOCK(mGetInstanceLock);

    if (mAudioALSACaptureDataProviderSpkFeed == NULL) {
        mAudioALSACaptureDataProviderSpkFeed = new AudioALSACaptureDataProviderSpkFeed();
    }
    ASSERT(mAudioALSACaptureDataProviderSpkFeed != NULL);
    return mAudioALSACaptureDataProviderSpkFeed;
}

AudioALSACaptureDataProviderSpkFeed::AudioALSACaptureDataProviderSpkFeed() {
    ALOGD("%s()", __FUNCTION__);
    int pcmindex = AudioALSADeviceParser::getInstance()->GetPcmIndexByString(keypcmUl2Capture);
    int cardindex = AudioALSADeviceParser::getInstance()->GetCardIndexByString(keypcmUl2Capture);
    ALOGD("%s cardindex = %d  pcmindex = %d", __FUNCTION__, cardindex, pcmindex);

    struct pcm_params *params;
    params = pcm_params_get(cardindex, pcmindex,  PCM_IN);
    if (params == NULL) {
        ALOGD("Device does not exist.\n");
    }
    unsigned int buffersizemax = pcm_params_get_max(params, PCM_PARAM_BUFFER_BYTES);
    ALOGD("%s() buffersizemax = %d", __FUNCTION__, buffersizemax);
    pcm_params_free(params);

    // TODO(Harvey): query this
    mConfig.channels = 2;
    mConfig.rate = 48000;

    // Buffer size: 2048(period_size) * 2(ch) * 2(byte) * 8(period_count) = 64 kb
    mConfig.period_count = 2;
    mConfig.period_size = (buffersizemax / mConfig.channels) / 2 / mConfig.period_count;
    mConfig.format = PCM_FORMAT_S16_LE;

    mConfig.start_threshold = 0;
    mConfig.stop_threshold = 0;
    mConfig.silence_threshold = 0;

    mCaptureDataProviderType = CAPTURE_PROVIDER_SPK_FEED;
}

AudioALSACaptureDataProviderSpkFeed::~AudioALSACaptureDataProviderSpkFeed() {
    ALOGD("%s()", __FUNCTION__);
}


status_t AudioALSACaptureDataProviderSpkFeed::open() {
    ALOGD("%s()", __FUNCTION__);
    AL_AUTOLOCK(*AudioALSADriverUtility::getInstance()->getStreamSramDramLock());

    ASSERT(mEnable == false);

    // config attribute (will used in client SRC/Enh/... later) // TODO(Harvey): query this
    mStreamAttributeSource.audio_format = AUDIO_FORMAT_PCM_16_BIT;
    mStreamAttributeSource.audio_channel_mask = AUDIO_CHANNEL_IN_STEREO;
    mStreamAttributeSource.num_channels = popcount(mStreamAttributeSource.audio_channel_mask);
    mStreamAttributeSource.sample_rate = 48000;

    // Reset frames readed counter
    mStreamAttributeSource.Time_Info.total_frames_readed = 0;


    OpenPCMDump(LOG_TAG);

    // enable pcm
    ASSERT(mPcm == NULL);
    int pcmIdx = AudioALSADeviceParser::getInstance()->GetPcmIndexByString(keypcmUl2Capture);
    int cardIdx = AudioALSADeviceParser::getInstance()->GetCardIndexByString(keypcmUl2Capture);
    mPcm = pcm_open(cardIdx, pcmIdx, PCM_IN, &mConfig);
    ASSERT(mPcm != NULL && pcm_is_ready(mPcm) == true);
    ALOGV("%s(), mPcm = %p", __FUNCTION__, mPcm);

    pcm_start(mPcm);

    // create reading thread
    mEnable = true;
    int rate = ((mConfig.period_size * mConfig.period_count * mConfig.channels * 2) - 2048) / (mConfig.channels * 2);
    AudioALSAHardwareResourceManager::getInstance()->setInterruptRate2(rate);
    int ret = pthread_create(&hReadThread, NULL, AudioALSACaptureDataProviderSpkFeed::readThread, (void *)this);
    if (ret != 0) {
        ALOGE("%s() create thread fail!!", __FUNCTION__);
        return UNKNOWN_ERROR;
    }

    return NO_ERROR;
}

status_t AudioALSACaptureDataProviderSpkFeed::close() {
    ALOGD("%s()", __FUNCTION__);

    mEnable = false;
    pthread_join(hReadThread, NULL);
    ALOGD("pthread_join hReadThread done");

    AL_AUTOLOCK(*AudioALSADriverUtility::getInstance()->getStreamSramDramLock());

    ClosePCMDump();

    pcm_stop(mPcm);
    pcm_close(mPcm);
    mPcm = NULL;
    ALOGD("-%s()", __FUNCTION__);
    return NO_ERROR;
}


void *AudioALSACaptureDataProviderSpkFeed::readThread(void *arg) {
    prctl(PR_SET_NAME, (unsigned long)__FUNCTION__, 0, 0, 0);

#ifdef MTK_AUDIO_ADJUST_PRIORITY
#define RTPM_PRIO_AUDIO_RECORD (77)
    // force to set priority
    struct sched_param sched_p;
    sched_getparam(0, &sched_p);
    sched_p.sched_priority = RTPM_PRIO_AUDIO_RECORD + 1;
    if (0 != sched_setscheduler(0, SCHED_RR, &sched_p)) {
        ALOGE("[%s] failed, errno: %d", __FUNCTION__, errno);
    } else {
        sched_p.sched_priority = RTPM_PRIO_AUDIO_CCCI_THREAD;
        sched_getparam(0, &sched_p);
        ALOGD("sched_setscheduler ok, priority: %d", sched_p.sched_priority);
    }
#endif
    ALOGD("+%s(), pid: %d, tid: %d", __FUNCTION__, getpid(), gettid());

    AudioALSACaptureDataProviderSpkFeed *pDataProvider = static_cast<AudioALSACaptureDataProviderSpkFeed *>(arg);

    uint32_t open_index = pDataProvider->mOpenIndex;

    // read raw data from alsa driver
    char linear_buffer[kReadBufferSize];
    while (pDataProvider->mEnable == true) {
        if (open_index != pDataProvider->mOpenIndex) {
            ALOGD("%s(), open_index(%d) != mOpenIndex(%d), return", __FUNCTION__, open_index, pDataProvider->mOpenIndex);
            break;
        }

        ASSERT(pDataProvider->mPcm != NULL);
        int retval = pcm_read(pDataProvider->mPcm, linear_buffer, kReadBufferSize);
        if (retval != 0) {
            ALOGE("%s(), pcm_read() error, retval = %d", __FUNCTION__, retval);
        }

        // use ringbuf format to save buffer info
        pDataProvider->mPcmReadBuf.pBufBase = linear_buffer;
        pDataProvider->mPcmReadBuf.bufLen   = kReadBufferSize + 1; // +1: avoid pRead == pWrite
        pDataProvider->mPcmReadBuf.pRead    = linear_buffer;
        pDataProvider->mPcmReadBuf.pWrite   = linear_buffer + kReadBufferSize;

        pDataProvider->provideCaptureDataToAllClients(open_index);
    }

    ALOGD("-%s(), pid: %d, tid: %d", __FUNCTION__, getpid(), gettid());
    pthread_exit(NULL);
    return NULL;
}

} // end of namespace android
