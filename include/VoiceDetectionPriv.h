/*
 * @Description: Internally used header files
 * @version: 1.0
 * @Author: Zhc Guo
 * @Date: 2020-06-18 14:10:07
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-24 10:35:28
 */
#pragma once

#include <stdint.h>

using namespace android;
namespace voice_detection {

class VoiceUtilsTrace {
  public:
    VoiceUtilsTrace(const char *func) : mFunc(func) {
        printf("%s in.\n", mFunc);
    }
    ~VoiceUtilsTrace() {
        printf("%s out.\n", mFunc);
    }

  private:
    const char *mFunc;
};

}  // namespace voice_detection
