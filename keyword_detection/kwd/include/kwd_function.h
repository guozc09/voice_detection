#pragma once

#include <stdint.h>

void *KWD_Init_Multi(uint8_t *memPool);
void *KWD_Release(void *handle);
const char *KWD_VerInfo();
int KWD_AddSample(void *handle, uint16_t *data, uint32_t size);
int KWD_GetResultNoWait(void *handle);
void KWD_GetResultEPD(void *handle, int32_t &nDur, int32_t &nDelay);
void KWD_Reset(void *handle);
