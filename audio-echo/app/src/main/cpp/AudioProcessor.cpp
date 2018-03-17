//
// Created by Thomas on 17/03/2018.
//

#include "AudioProcessor.h"
#include <math.h>

AudioProcessor::AudioProcessor(SampleFormat *sampleFormat) {

    sampleInfo_ = *sampleFormat;
    SLAndroidDataFormat_PCM_EX format_pcm;
    ConvertToSLSampleFormat(&format_pcm, sampleFormat);

    delaySeconds = 0.2f;
    delaySamples = static_cast<uint32_t>(delaySeconds * static_cast<float>(sampleFormat->sampleRate_/1000));

    feedback = 0.5f;

    memoryBuffer_.cap_ = (format_pcm.containerSize >> 3) * format_pcm.numChannels * (delaySamples);
    memoryBuffer_.buf_ = new uint8_t[memoryBuffer_.cap_];
    memset(memoryBuffer_.buf_, 0, memoryBuffer_.cap_);
    memoryBuffer_.size_ = memoryBuffer_.cap_;

    //phase = 0.0f;
    //frequency = 1000.0f;

    //phaseIncrement = 2.0f * M_PI * frequency / static_cast<float>(sampleFormat->sampleRate_/1000);

    LOGI("sample rate = %d", sampleFormat->sampleRate_);
}

AudioProcessor::~AudioProcessor() {
    delete(memoryBuffer_.buf_);
}

void AudioProcessor::process(sample_buf *ioBuffer) {

    int16_t* audioData = (int16_t*)(ioBuffer->buf_);
    int16_t* memoryData = (int16_t*)(memoryBuffer_.buf_);
    float sample = 0;
    float output = 0;
    float memory = 0;
    //float coeff = 0.0f;

    float int16Max = static_cast<float>(INT16_MAX);
    float int16Min = static_cast<float>(INT16_MIN);

    for (int i = 0; i < sampleInfo_.framesPerBuf_; ++i) {

        sample = audioData[i] >= 0 ? static_cast<float>(audioData[i]) / int16Max : static_cast<float>(-audioData[i]) / int16Min;
        memory = memoryData[memoryReadPosition] >= 0 ? static_cast<float>(memoryData[memoryReadPosition]) / int16Max : static_cast<float>(-memoryData[memoryReadPosition]) / int16Min;
        output = feedback * memory + sample;

        //coeff = (1.0f+sin(phase))/2.0f;
        //phase += phaseIncrement;
        //sample = coeff*sample;

        if (output > 1.0f) output = 1.0f;
        if (output < -1.0f) output = -1.0f;

        audioData[i] = output > 0 ? static_cast<int16_t>(output*INT16_MAX) : static_cast<int16_t>(-output*INT16_MIN);

        memoryData[memoryReadPosition] = audioData[i];

        if(++memoryReadPosition == delaySamples) memoryReadPosition = 0;
    }

}
