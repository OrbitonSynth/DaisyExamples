#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed  hw;
static Biquad     flt;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size) {
    float output;
    for(size_t i = 0; i < size; i += 2) {
        output = flt.Process(in[i]);
        // left out
        out[i] = output;
        // right out
        out[i + 1] = output;
    }
}

int main(void) {
    // initialize seed hardware and daisysp modules
    float sample_rate;
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    // initialize Biquad and set parameters
    flt.Init(sample_rate);
    flt.SetRes(0.9);
    flt.SetCutoff(4000);
    // start callback
    hw.StartAudio(AudioCallback);


    while(1) {}
}
