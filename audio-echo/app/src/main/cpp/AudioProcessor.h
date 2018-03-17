//
// Created by Thomas on 17/03/2018.
//

#ifndef AUDIO_ECHO_AUDIOPROCESSOR_H
#define AUDIO_ECHO_AUDIOPROCESSOR_H

#include "audio_common.h"


class AudioProcessor {
public:
    explicit AudioProcessor(SampleFormat *sampleFormat);
    ~AudioProcessor();

    void process(sample_buf *ioBuffer);

private:

    SampleFormat sampleInfo_;
    sample_buf internalBuffer_;
    float phase;
    float frequency;
    float phaseIncrement;
};


#endif //AUDIO_ECHO_AUDIOPROCESSOR_H
