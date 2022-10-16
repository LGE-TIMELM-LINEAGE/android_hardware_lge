LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := GCam
LOCAL_MODULE_TAGS := optional
LOCAL_OPTIONAL_USES_LIBRARIES := \
	com.google.android.gestureservice \
	com.google.android.camera2 \
	com.google.android.camera.experimental2015 \
	com.google.android.camera.experimental2016 \
	com.google.android.camera.experimental2017 \
	com.google.android.camera.experimental2018 \
	com.google.android.camera.experimental2019 \
	com.google.android.camera.experimental2020 \
	com.google.android.camera.experimental2020_midyear \
	com.google.android.wearable
LOCAL_SRC_FILES := priv-app/BSGC/BSGC.apk
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_MODULE_PATH := $(TARGET_OUT)/priv-app
LOCAL_OVERRIDES_PACKAGES := SnapdragonCamera Snap Camera2 Camera
include $(BUILD_PREBUILT)