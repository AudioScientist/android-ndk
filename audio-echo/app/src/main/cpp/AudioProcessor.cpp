//
// Created by Thomas on 17/03/2018.
//

#include "AudioProcessor.h"
#include <math.h>

AudioProcessor::AudioProcessor(SampleFormat *sampleFormat) {

    sampleInfo_ = *sampleFormat;
    SLAndroidDataFormat_PCM_EX format_pcm;
    ConvertToSLSampleFormat(&format_pcm, sampleFormat);

    internalBuffer_.cap_ = (format_pcm.containerSize >> 3) * format_pcm.numChannels * sampleInfo_.framesPerBuf_;
    internalBuffer_.buf_ = new uint8_t[internalBuffer_.cap_];
    memset(internalBuffer_.buf_, 0, internalBuffer_.cap_);
    internalBuffer_.size_ = internalBuffer_.cap_;

    phase = 0.0f;
    frequency = 1000.0f;

    phaseIncrement = 2.0f * M_PI * frequency / static_cast<float>(sampleFormat->sampleRate_/1000);

    LOGI("sample rate = %d", sampleFormat->sampleRate_);
}

AudioProcessor::~AudioProcessor() {
    delete(internalBuffer_.buf_);
}

void AudioProcessor::process(sample_buf *ioBuffer) {

    int16_t* audioData = (int16_t*)(ioBuffer->buf_);
    float sample = 0;
    float coeff = 0.0f;

    float int16Max = static_cast<float>(INT16_MAX);
    float int16Min = static_cast<float>(INT16_MIN);

    for (int i = 0; i < sampleInfo_.framesPerBuf_; ++i) {

        sample = audioData[i] >= 0 ? static_cast<float>(audioData[i]) / int16Max : static_cast<float>(-audioData[i]) / int16Min;

        coeff = (1.0f+sin(phase))/2.0f;
        phase += phaseIncrement;

        sample = coeff*sample;

        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;

        audioData[i] = sample > 0 ? static_cast<int16_t>(sample*INT16_MAX) : static_cast<int16_t>(-sample*INT16_MIN);
    }

}