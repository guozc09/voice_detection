/*
 * @Description: KeywordDetection.cpp
 * @version: 1.0
 * @Author: Zhc Guo
 * @Date: 2020-06-01 11:39:23
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-28 10:34:33
 */
#define LOG_TAG "KWD"
#include "KeywordDetection.h"

#include <pthread.h>
#include <stdio.h>
#include <sys/prctl.h>

#include <queue>

#include "VoiceDetectionPriv.h"
#include "kwd_function.h"

using namespace std;

namespace voice_detection {

#define N_MEM_SIZE 2000

struct DataPacket {
  uint32_t dataLen;
  uint16_t *data;
};

class KeywordDetection::KeywordDetectionCPP {
 public:
  static void *KWD_thread(void *threadData);

  void *mKWD{nullptr};
  pthread_t mThread{0};
  pthread_mutex_t mMutex;
  pthread_cond_t mCv;
  Notify_f mNotify{nullptr};
  volatile bool mIsRuning{false};
  void *mContext{nullptr};
  queue<DataPacket> mDataQueue;
  uint8_t *mMemPool{nullptr};
};

KeywordDetection::KeywordDetection(uint32_t kwdCmdModeMask)
    : m(new KeywordDetectionCPP) {
  VoiceUtilsTrace trace(__FUNCTION__);
  (void)kwdCmdModeMask;
  int32_t nErr = 0;
  m->mMemPool = (uint8_t *)malloc(sizeof(uint8_t) * N_MEM_SIZE);

  // 1:create recognition engine
  m->mKWD = KWD_Init_Multi(m->mMemPool);
  if (m->mKWD == nullptr) {
    printf("KWD_Init_Multi Err: %d\n", nErr);
  } else {
    pthread_mutex_init(&m->mMutex, nullptr);
    pthread_cond_init(&m->mCv, nullptr);
  }
}

KeywordDetection::~KeywordDetection() {
  VoiceUtilsTrace trace(__FUNCTION__);
  if (m->mKWD) {
    KWD_Release(m->mKWD);
    pthread_mutex_destroy(&m->mMutex);
    pthread_cond_destroy(&m->mCv);
  }
  while (!m->mDataQueue.empty()) {
    DataPacket &dp = m->mDataQueue.front();
    if (dp.data) {
      free(dp.data);
    }
    m->mDataQueue.pop();
  }
  if (m->mMemPool) {
    free(m->mMemPool);
    m->mMemPool = nullptr;
  }
  delete m;
}

int32_t KeywordDetection::start() {
  VoiceUtilsTrace trace(__FUNCTION__);
  if (m->mThread == 0) {
    m->mIsRuning = true;
    int32_t n =
        pthread_create(&m->mThread, nullptr,
                       KeywordDetection::KeywordDetectionCPP::KWD_thread, this);
    if (n != 0) {
      printf("Fail to create KWD_thread.\n");
      m->mThread = 0;
      m->mIsRuning = false;
      return n;
    }
  } else {
    printf("KWD_thread has been started, do nothing.\n");
  }
  return 0;
}

int32_t KeywordDetection::write(uint8_t *data, uint32_t dataLen) {
  // VoiceUtilsTrace trace(__FUNCTION__);
  if (m->mIsRuning) {
    DataPacket dp;
    dp.data = (uint16_t *)malloc(dataLen);
    memcpy(dp.data, (uint16_t *)data, dataLen);
    dp.dataLen = dataLen;
    pthread_mutex_lock(&m->mMutex);
    m->mDataQueue.push(dp);
    pthread_cond_signal(&m->mCv);
    pthread_mutex_unlock(&m->mMutex);
  } else {
    printf("Keyword detection not started, do nothing\n");
  }
  return 0;
}

int32_t KeywordDetection::stop() {
  VoiceUtilsTrace trace(__FUNCTION__);
  if (m->mThread) {
    m->mIsRuning = false;
    pthread_mutex_lock(&m->mMutex);
    pthread_cond_signal(&m->mCv);
    pthread_mutex_unlock(&m->mMutex);
    pthread_join(m->mThread, nullptr);
    m->mThread = 0;
  } else {
    printf("Keyword detection not started, do nothing\n");
  }
  return 0;
}

void KeywordDetection::setNotify(Notify_f notify_f, void *context) {
  VoiceUtilsTrace trace(__FUNCTION__);
  m->mNotify = notify_f;
  m->mContext = context;
}

string KeywordDetection::getVersion() {
  string version = "voice_detection version: ";
  version = version + to_string(VOICE_DETECTION_VERSION_MAJOR) + "." +
            to_string(VOICE_DETECTION_VERSION_MINOR) + "." +
            to_string(VOICE_DETECTION_VERSION_RELEASE);
  string KWDVer = KWD_VerInfo();
  version = version + ", Keyword Detection version(KWD): " + KWDVer;
  return version;
}

void *KeywordDetection::KeywordDetectionCPP::KWD_thread(void *threadData) {
  VoiceUtilsTrace trace(__FUNCTION__);
#if __ANDROID__
  pthread_setname_np(pthread_self(), "Keyword detection thread");
#else
  prctl(PR_SET_NAME, "Keyword detection");
#endif
  KeywordDetection *kwd = reinterpret_cast<KeywordDetection *>(threadData);
  int32_t nRes;
  DataPacket dp = {0, nullptr};
  while (kwd->m->mIsRuning) {
    pthread_mutex_lock(&kwd->m->mMutex);
    if (kwd->m->mDataQueue.empty()) {
      pthread_cond_wait(&kwd->m->mCv, &kwd->m->mMutex);
    }
    if (!kwd->m->mDataQueue.empty()) {
      dp = kwd->m->mDataQueue.front();
      kwd->m->mDataQueue.pop();
    }
    pthread_mutex_unlock(&kwd->m->mMutex);
    if (dp.data != nullptr && dp.dataLen > 0) {
      nRes = KWD_AddSample(kwd->m->mKWD, (uint16_t *)dp.data, dp.dataLen / 2);
      nRes = KWD_GetResultNoWait(kwd->m->mKWD);
      if (nRes == 0 || nRes == 1) {
        int32_t nDur = 0, nDelay = 0, nLatency = 0;
        int32_t nDurMs = 0, nDelayMs = 0, nLatencyMs = 0;

        KWD_GetResultEPD(kwd->m->mKWD, nDur, nDelay, nLatency);
        nDurMs = nDur / 16;
        nDelayMs = nDelay / 16;
        nLatencyMs = nLatency / 16;
        int32_t latencyMs = nDurMs + nDelayMs + nLatencyMs;
        printf(
            "Result ID = %d, %s\nDuration = %d(%dms), Command Delay = "
            "%d(%dms), "
            "Siri latency = %dms\n",
            nRes, cmdlist[nRes], nDur, nDurMs, nDelay, nDelayMs, latencyMs);
        if (kwd->m->mNotify) kwd->m->mNotify(latencyMs, kwd->m->mContext);
        KWD_Reset(kwd->m->mKWD);
      } else if (nRes > 1) {
        KWD_Reset(kwd->m->mKWD);
      }
      free(dp.data);
      dp.data = nullptr;
      dp.dataLen = 0;
    }
  }
  return nullptr;
}

}  // namespace voice_detection
