#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

static DaisySeed  hw;
CpuLoadMeter loadMeter;
static Biquad     flt;

float frac;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
    float output;

    loadMeter.OnBlockStart();
    for (size_t i = 0; i < size; i++) {
        output = flt.Process(in[0][i]);
        // left out
        out[0][i] = output;
        // right out
        out[1][i] = output;
    }
    loadMeter.OnBlockEnd();
}

int main(void) {
    // initialize seed hardware and daisysp modules
    float sample_rate;
    hw.Configure();
    hw.Init();
    hw.StartLog();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();

    frac = 1.f / RAND_MAX;

    // initialize Biquad and set parameters
    flt.Init(sample_rate);
    flt.SetRes(rand() * frac * 0.9f); // 0 - 0.9
    flt.SetCutoff(100 + rand() * frac * 4900.0f); // 100Hz - 5kHz

    // start callback
    loadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());
    hw.StartAudio(AudioCallback);

    while(1) {
		// get the current load (smoothed value and peak values)
        const float avgLoad = loadMeter.GetAvgCpuLoad();
        const float maxLoad = loadMeter.GetMaxCpuLoad();
        const float minLoad = loadMeter.GetMinCpuLoad();
        // print it to the serial connection (as percentages)
        hw.PrintLine("Processing Load %:");
        hw.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad * 100.0f));
        hw.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad * 100.0f));
        hw.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad * 100.0f));
        // don't spam the serial connection too much
        System::Delay(500);
	}
}
