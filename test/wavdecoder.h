#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif

unsigned char* load_wav_file(const char *fname, int16_t *chans, int32_t *rate, int32_t *length, int16_t *bits);
