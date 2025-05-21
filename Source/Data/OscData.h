/*
  ==============================================================================

    OscData.h
    Created: 19 Apr 2025 9:19:04am
    Author:  florentspagni

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setOscWaveType(const int choice);
    void getNextAudioBlock(juce::dsp::AudioBlock<float> &audioBlock);
    void setWaveFrequency(const int midiNoteNumber);
    void setFMParams(const float fmDepth, const float frequency);
private:
    juce::dsp::Oscillator<float> fmOsc{[](float x){return std::sin(x);}};
    float fmMod {0.0f}; // between -1 and 1
    float fmDepth{0.0f};
    int lastMidiNote{0};
};
