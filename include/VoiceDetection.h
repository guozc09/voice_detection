/*
 * @Description:
 * @version:
 * @Author: Zhc Guo
 * @Date: 2020-06-09 14:33:26
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-19 16:36:29
 */
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string>

using namespace std;
namespace voice_detection {

/**
 * @description: Keyword detect result callback.
 * @param delayMs Ending silence length in number of samples.
 * @param context  The context of Caller.
 * @return: 0 on success or error code.
 */
using Notify_f = int32_t (*)(int32_t latencyMs, void *context);

class VoiceDetection {
  public:
    VoiceDetection() = default;
    virtual ~VoiceDetection() = default;

    /**
     * @description: Start recognize.
     * @return: 0 on success or error code.
     */
    virtual int32_t start() = 0;

    /**
     * @description: Add voice samples to the recognizer and perform recognition.
     * @param data[in]    The pointer of voice data buffer.
     * @param dataLen[in] The number of voice data (a unit is a int16_t, we prefer to add 480 samples per call).
     * @return: The length of the data actually write.
     */
    virtual int32_t write(uint8_t *data, uint32_t dataLen) = 0;

    /**
     * @description: Stop recognize.
     * @return: 0 on success or error code.
     */
    virtual int32_t stop() = 0;

    /**
     * @description: Set the callback called by recognizer.
     * @param notify_f Function pointer of the executive body.
     * @return: none
     */
    virtual void setNotify(Notify_f notify_f, void *context) = 0;

    /**
     * @description: Get module version.
     * @return: string    module version.
     */
    virtual string getVersion() = 0;
};

}  // namespace voice_detection
