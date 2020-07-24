/*
 * @Description: VoiceActivityDetection.cpp
 * @version: 1.0
 * @Author: Zhc Guo
 * @Date: 2020-06-09 14:35:42
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-24 11:05:10
 */
#define LOG_TAG "VAD"
#include "VoiceActivityDetection.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/prctl.h>
#include <queue>
#include "VoiceDetectionPriv.h"
#include "vad_function.h"

namespace voice_detection {

struct DataPacket {
    uint32_t dataLen;
    int16_t *data;
};

class VoiceActivityDetection::VoiceActivityDetectionCPP {
  public:
    static void *VAD_thread(void *threadData);

    pthread_t mThread{0};
    pthread_mutex_t mMutex;
    pthread_cond_t mCv;
    Notify_f mNotify{nullptr};
    volatile bool mIsRuning{false};
    void *mContext{nullptr};
    queue<DataPacket> mDataQueue;
};

VoiceActivityDetection::VoiceActivityDetection() : m(new VoiceActivityDetectionCPP) {
    VoiceUtilsTrace trace(__FUNCTION__);
    vad_init();
    pthread_mutex_init(&m->mMutex, nullptr);
    pthread_cond_init(&m->mCv, nullptr);
}

VoiceActivityDetection::~VoiceActivityDetection() {
    VoiceUtilsTrace trace(__FUNCTION__);
    vad_del();
    pthread_mutex_destroy(&m->mMutex);
    pthread_cond_destroy(&m->mCv);
    while (!m->mDataQueue.empty()) {
        DataPacket &dp = m->mDataQueue.front();
        if (dp.data) {
            free(dp.data);
        }
        m->mDataQueue.pop();
    }
    delete m;
}

int32_t VoiceActivityDetection::start() {
    VoiceUtilsTrace trace(__FUNCTION__);
    if (m->mThread == 0) {
        m->mIsRuning = true;
        int32_t n =
            pthread_create(&m->mThread, nullptr, VoiceActivityDetection::VoiceActivityDetectionCPP::VAD_thread, this);
        if (n != 0) {
            printf("Fail to create VAD_thread.\n");
            m->mThread = 0;
            m->mIsRuning = false;
            return n;
        }
    } else {
        printf("VAD_thread has been started, do nothing.\n");
    }
    return 0;
}

int32_t VoiceActivityDetection::write(uint8_t *data, uint32_t dataLen) {
    // VoiceUtilsTrace trace(__FUNCTION__);
    if (m->mIsRuning) {
        DataPacket dp;
        dp.data = (int16_t *)malloc(dataLen);
        memcpy(dp.data, (int16_t *)data, dataLen);
        dp.dataLen = dataLen;
        pthread_mutex_lock(&m->mMutex);
        m->mDataQueue.push(dp);
        pthread_cond_signal(&m->mCv);
        pthread_mutex_unlock(&m->mMutex);
    } else {
        printf("VAD not started, do nothing\n");
    }
    return 0;
}

int32_t VoiceActivityDetection::stop() {
    VoiceUtilsTrace trace(__FUNCTION__);
    if (m->mThread) {
        m->mIsRuning = false;
        pthread_mutex_lock(&m->mMutex);
        pthread_cond_signal(&m->mCv);
        pthread_mutex_unlock(&m->mMutex);
        pthread_join(m->mThread, nullptr);
        m->mThread = 0;
    } else {
        printf("VAD not started, do nothing\n");
    }
    return 0;
}

void VoiceActivityDetection::setNotify(Notify_f notify_f, void *context) {
    VoiceUtilsTrace trace(__FUNCTION__);
    m->mNotify = notify_f;
    m->mContext = context;
    return;
}

string VoiceActivityDetection::getVersion() {
    string version = "voice_detection version: ";
    version = version + to_string(VOICE_DETECTION_VERSION_MAJOR) + "." + to_string(VOICE_DETECTION_VERSION_MINOR) + "." +
              to_string(VOICE_DETECTION_VERSION_RELEASE);
    version = version + ", Voice Activity Detection version: 2.0";
    return version;
}

void *VoiceActivityDetection::VoiceActivityDetectionCPP::VAD_thread(void *threadData) {
    VoiceUtilsTrace trace(__FUNCTION__);
#if __ANDROID__
    pthread_setname_np(pthread_self(), "VAD thread");
#else
    prctl(PR_SET_NAME, "VAD thread");
#endif
    VoiceActivityDetection *p_vad = reinterpret_cast<VoiceActivityDetection *>(threadData);
    int16_t nRes;
    DataPacket dp = {0, nullptr};
    while (p_vad->m->mIsRuning) {
        pthread_mutex_lock(&p_vad->m->mMutex);
        if (p_vad->m->mDataQueue.empty()) {
            pthread_cond_wait(&p_vad->m->mCv, &p_vad->m->mMutex);
        }
        if (!p_vad->m->mDataQueue.empty()) {
            dp = p_vad->m->mDataQueue.front();
            p_vad->m->mDataQueue.pop();
        }
        pthread_mutex_unlock(&p_vad->m->mMutex);
        if (dp.data != nullptr && dp.dataLen > 0) {
            nRes = vad((int16_t *)dp.data, dp.dataLen / 2);
            if (nRes == 1) {
                int32_t latencyMs = 312;
                printf("Trigger requestSiri latencyMs(%dms)\n", latencyMs);
                if (p_vad->m->mNotify)
                    p_vad->m->mNotify(latencyMs, p_vad->m->mContext);
            }
            free(dp.data);
            dp.data = nullptr;
            dp.dataLen = 0;
        }
    }
    return nullptr;
}

}  // namespace voice_detection
