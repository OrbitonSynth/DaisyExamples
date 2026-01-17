#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
Resonator res;
Metro     tick;

float frac;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size) {
    for(size_t i = 0; i < size; i++) {
        res.SetFreq(440); //110 - 880
        res.SetStructure(0.5f);
        res.SetBrightness(0.5f);
        res.SetDamping(0.5f);


        out[0][i] = out[1][i] = res.Process(in[0][i]);
    }
}

int main(void){
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);
    float sample_rate = hw.AudioSampleRate();

    res.Init(.015, 24, sample_rate);
    res.SetStructure(-7.f);

    tick.Init(1.f, sample_rate);

    frac = 1.f / RAND_MAX;

    hw.StartAudio(AudioCallback);
    while(1) {}
}