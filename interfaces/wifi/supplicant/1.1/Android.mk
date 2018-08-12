# This file is autogenerated by hidl-gen. Do not edit manually.

LOCAL_PATH := $(call my-dir)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.mediatek.hardware.wifi.supplicant-V1.1-java
LOCAL_MODULE_CLASS := JAVA_LIBRARIES

intermediates := $(call local-generated-sources-dir, COMMON)

HIDL := $(HOST_OUT_EXECUTABLES)/hidl-gen$(HOST_EXECUTABLE_SUFFIX)

LOCAL_JAVA_LIBRARIES := \
    android.hardware.wifi.supplicant-V1.0-java \


#
# Build IMtkSupplicantStaIface.hal
#
GEN := $(intermediates)/vendor/mediatek/hardware/wifi/supplicant/V1_1/IMtkSupplicantStaIface.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IMtkSupplicantStaIface.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.mediatek.hardware:vendor/mediatek/proprietary/hardware/interfaces \
        vendor.mediatek.hardware.wifi.supplicant@1.1::IMtkSupplicantStaIface

$(GEN): $(LOCAL_PATH)/IMtkSupplicantStaIface.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build IMtkSupplicantStaNetwork.hal
#
GEN := $(intermediates)/vendor/mediatek/hardware/wifi/supplicant/V1_1/IMtkSupplicantStaNetwork.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IMtkSupplicantStaNetwork.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.mediatek.hardware:vendor/mediatek/proprietary/hardware/interfaces \
        vendor.mediatek.hardware.wifi.supplicant@1.1::IMtkSupplicantStaNetwork

$(GEN): $(LOCAL_PATH)/IMtkSupplicantStaNetwork.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)
include $(BUILD_JAVA_LIBRARY)


################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.mediatek.hardware.wifi.supplicant-V1.1-java-static
LOCAL_MODULE_CLASS := JAVA_LIBRARIES

intermediates := $(call local-generated-sources-dir, COMMON)

HIDL := $(HOST_OUT_EXECUTABLES)/hidl-gen$(HOST_EXECUTABLE_SUFFIX)

LOCAL_STATIC_JAVA_LIBRARIES := \
    android.hardware.wifi.supplicant-V1.0-java-static \


#
# Build IMtkSupplicantStaIface.hal
#
GEN := $(intermediates)/vendor/mediatek/hardware/wifi/supplicant/V1_1/IMtkSupplicantStaIface.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IMtkSupplicantStaIface.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.mediatek.hardware:vendor/mediatek/proprietary/hardware/interfaces \
        vendor.mediatek.hardware.wifi.supplicant@1.1::IMtkSupplicantStaIface

$(GEN): $(LOCAL_PATH)/IMtkSupplicantStaIface.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)

#
# Build IMtkSupplicantStaNetwork.hal
#
GEN := $(intermediates)/vendor/mediatek/hardware/wifi/supplicant/V1_1/IMtkSupplicantStaNetwork.java
$(GEN): $(HIDL)
$(GEN): PRIVATE_HIDL := $(HIDL)
$(GEN): PRIVATE_DEPS := $(LOCAL_PATH)/IMtkSupplicantStaNetwork.hal
$(GEN): PRIVATE_OUTPUT_DIR := $(intermediates)
$(GEN): PRIVATE_CUSTOM_TOOL = \
        $(PRIVATE_HIDL) -o $(PRIVATE_OUTPUT_DIR) \
        -Ljava \
        -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport \
        -rvendor.mediatek.hardware:vendor/mediatek/proprietary/hardware/interfaces \
        vendor.mediatek.hardware.wifi.supplicant@1.1::IMtkSupplicantStaNetwork

$(GEN): $(LOCAL_PATH)/IMtkSupplicantStaNetwork.hal
	$(transform-generated-source)
LOCAL_GENERATED_SOURCES += $(GEN)
include $(BUILD_STATIC_JAVA_LIBRARY)



include $(call all-makefiles-under,$(LOCAL_PATH))