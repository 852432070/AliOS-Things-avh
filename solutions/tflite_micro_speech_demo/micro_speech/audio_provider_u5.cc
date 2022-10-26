#include "audio_provider.h"
#include "micro_features/micro_model_settings.h"
#include <stdio.h>
#include "stm32u5xx_hal_mdf.h"

extern MDF_HandleTypeDef AdfHandle0;

namespace {
int16_t g_dummy_audio_data[kMaxAudioSampleSize];
int32_t g_latest_audio_timestamp = 0;
}  // namespace

TfLiteStatus GetAudioSamples(tflite::ErrorReporter* error_reporter,
                             int start_ms, int duration_ms,
                             int* audio_samples_size, int16_t** audio_samples) {
  const int yes_start = (0 * kAudioSampleFrequency) / 1000;
  const int yes_end = (1000 * kAudioSampleFrequency) / 1000;
  const int no_start = (4000 * kAudioSampleFrequency) / 1000;
  const int no_end = (5000 * kAudioSampleFrequency) / 1000;
  const int wraparound = (8000 * kAudioSampleFrequency) / 1000;
  const int start_sample = (start_ms * kAudioSampleFrequency) / 1000;
  for (int i = 0; i < kMaxAudioSampleSize; ++i) {
    int32_t p_data = 0;
    if(HAL_MDF_GetAcqValue(&AdfHandle0, &p_data) == HAL_OK) {
		g_dummy_audio_data[i] = uint16_t(p_data >> 16);
	}
    else {
        g_dummy_audio_data[i] = 0;
    }

  }
  *audio_samples_size = kMaxAudioSampleSize;
  *audio_samples = g_dummy_audio_data;
  return kTfLiteOk;
}

int32_t LatestAudioTimestamp() {
  g_latest_audio_timestamp += 100;
  return g_latest_audio_timestamp;
}