/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "android.hardware.biometrics.fingerprint@2.3-service.lge"

#include "BiometricsFingerprint.h"

namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {
namespace V2_3 {
namespace implementation {

BiometricsFingerprint::BiometricsFingerprint()
    : mScannerFd(open("/dev/esfp0", O_RDWR)), mBacklightFd(open("/sys/devices/virtual/panel/brightness/fp_lhbm",O_WRONLY))  {
    mLgeBiometricsFingerprint = ILgeBiometricsFingerprint::getService();
    mLgeBiometricsFingerprint->setHalCallback(this);
}

Return<uint64_t> BiometricsFingerprint::setNotify(
        const sp<V2_1::IBiometricsFingerprintClientCallback>& clientCallback) {
    mClientCallback = std::move(clientCallback);
    return mLgeBiometricsFingerprint->setNotify(this);
}

Return<uint64_t> BiometricsFingerprint::preEnroll() {
    setDimlayerHbm(LGE_FP_LHBM_READY);
    return mLgeBiometricsFingerprint->preEnroll();
}

Return<RequestStatus> BiometricsFingerprint::enroll(const hidl_array<uint8_t, 69>& hat,
                                                    uint32_t gid, uint32_t timeoutSec) {
    return mLgeBiometricsFingerprint->enroll(hat, gid, timeoutSec);
}

Return<RequestStatus> BiometricsFingerprint::postEnroll() {
    setDimlayerHbm(LGE_FP_LHBM_EXIT);
    return mLgeBiometricsFingerprint->postEnroll();
}

Return<uint64_t> BiometricsFingerprint::getAuthenticatorId() {
    return mLgeBiometricsFingerprint->getAuthenticatorId();
}

Return<RequestStatus> BiometricsFingerprint::cancel() {
    setDimlayerHbm(LGE_FP_LHBM_EXIT);
    return mLgeBiometricsFingerprint->cancel();
}

Return<RequestStatus> BiometricsFingerprint::enumerate() {
    return mLgeBiometricsFingerprint->enumerate();
}

Return<RequestStatus> BiometricsFingerprint::remove(uint32_t gid, uint32_t fid) {
    return mLgeBiometricsFingerprint->remove(gid, fid);
}

Return<RequestStatus> BiometricsFingerprint::setActiveGroup(uint32_t gid,
                                                            const hidl_string& storePath) {
    return mLgeBiometricsFingerprint->setActiveGroup(gid, storePath);
}

Return<RequestStatus> BiometricsFingerprint::authenticate(uint64_t operationId, uint32_t gid) {
    setDimlayerHbm(LGE_FP_LHBM_READY);
    return mLgeBiometricsFingerprint->authenticate(operationId, gid);
}

Return<bool> BiometricsFingerprint::isUdfps(uint32_t sensorID) {
    return mLgeBiometricsFingerprint->isUdfps(sensorID);
}

Return<void> BiometricsFingerprint::onFingerDown(uint32_t x, uint32_t y, float minor, float major) {
    setFpPress(1) && setDimlayerHbm(LGE_FP_LHBM_ON);
    return mLgeBiometricsFingerprint->onFingerDown(x, y, minor, major);
}

Return<void> BiometricsFingerprint::onFingerUp() {
    setDimlayerHbm(LGE_FP_LHBM_EXIT) && setDimlayerHbm(LGE_FP_LHBM_READY) && setFpPress(0);  //No known to turn off "Finger On" light without exiting Hbm mode. So it exits then immediately goes back into to HBM mode
    return mLgeBiometricsFingerprint->onFingerUp();
}

Return<void> BiometricsFingerprint::onEnrollResult(uint64_t deviceId, uint32_t fingerId,
                                                   uint32_t groupId, uint32_t remaining) {
    return mClientCallback->onEnrollResult(deviceId, fingerId, groupId, remaining);
}

Return<void> BiometricsFingerprint::onAcquired(uint64_t deviceId,
                                               V2_1::FingerprintAcquiredInfo acquiredInfo,
                                               int32_t vendorCode) {
    return mClientCallback->onAcquired(deviceId, acquiredInfo, vendorCode);
}

Return<void> BiometricsFingerprint::onAuthenticated(uint64_t deviceId, uint32_t fingerId,
                                                    uint32_t groupId,
                                                    const hidl_vec<uint8_t>& token) {
    if (fingerId != 0) {
        setDimlayerHbm(LGE_FP_LHBM_EXIT);
    }
    setFpPress(0);
    return mClientCallback->onAuthenticated(deviceId, fingerId, groupId, token);
}

Return<void> BiometricsFingerprint::onError(uint64_t deviceId, FingerprintError error,
                                            int32_t vendorCode) {
    setDimlayerHbm(LGE_FP_LHBM_EXIT);
    setFpPress(0);
    return mClientCallback->onError(deviceId, error, vendorCode);
}

Return<void> BiometricsFingerprint::onRemoved(uint64_t deviceId, uint32_t fingerId,
                                              uint32_t groupId, uint32_t remaining) {
    return mClientCallback->onRemoved(deviceId, fingerId, groupId, remaining);
}

Return<void> BiometricsFingerprint::onEnumerate(uint64_t deviceId, uint32_t fingerId,
                                                uint32_t groupId, uint32_t remaining) {
    return mClientCallback->onEnumerate(deviceId, fingerId, groupId, remaining);
}

Return<void> BiometricsFingerprint::onAcquired_2_2(uint64_t deviceId,
                                                   FingerprintAcquiredInfo acquiredInfo,
                                                   int32_t vendorCode) {
    return reinterpret_cast<V2_2::IBiometricsFingerprintClientCallback*>(mClientCallback.get())
            ->onAcquired_2_2(deviceId, acquiredInfo, vendorCode);
}

Return<void> BiometricsFingerprint::onEngineeringInfoUpdated(
        uint32_t /*lenth*/, const hidl_vec<uint32_t>& /*keys*/,
        const hidl_vec<hidl_string>& /*values*/) {
    return Void();
}

Return<void> BiometricsFingerprint::onFingerprintCmd(int32_t /*cmdId*/,
                                                     const hidl_vec<uint32_t>& /*result*/,
                                                     uint32_t /*resultLen*/) {
    return Void();
}

}  // namespace implementation
}  // namespace V2_3
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
