/*
 * @Description: VoiceActivityDetection.h
 * @version: 1.0
 * @Author: Zhc Guo
 * @Date: 2020-06-09 14:32:19
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-24 11:05:34
 */
#pragma once

#include <stdint.h>
#include "VoiceDetection.h"

using namespace std;
namespace voice_detection {

class VoiceActivityDetection : public VoiceDetection {
  public:
    /**
     * @description: Create a recognizer for recognizing multiple groups of commands simultaneously.
     * @return: The handle of keyword detection.
     */
    VoiceActivityDetection();

    /**
     * @description: Destroy a recognizer (free resources).
     * @return: 0 on success or error code.
     */
    ~VoiceActivityDetection();

    /**
     * @description: Start recognize.
     * @return: 0 on success or error code.
     */
    int32_t start() override;

    /**
     * @description: Add voice samples to the recognizer and perform recognition.
     * @param data[in]    The pointer of voice data buffer.
     * @param dataLen[in] The number of voice data (a unit is a int16_t, we prefer to add 480 samples per call).
     * @return: The length of the data actually write.
     */
    int32_t write(uint8_t *data, uint32_t dataLen) override;

    /**
     * @description: Stop recognize.
     * @return: 0 on success or error code.
     */
    int32_t stop() override;

    /**
     * @description: Set the callback called by recognizer.
     * @param callback Function pointer of the executive body.
     * @return: none
     */
    void setNotify(Notify_f notify_f, void *context) override;

    /**
     * @description: Get module version.
     * @return: string    module version.
     */
    string getVersion() override;

  private:
    class VoiceActivityDetectionCPP;
    VoiceActivityDetectionCPP *m;
};

}  // namespace voice_detection
