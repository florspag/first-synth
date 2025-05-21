/*
  ==============================================================================

    FilterData.h
    Created: 15 May 2025 8:52:13pm
    Author:  florentspagni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class FilterData{

public:
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels);
    void processBlock (juce::AudioBuffer<float>& buffer);
    void updateParameters(const int filterType, const float frequency, const float resonance ,  const float modulator=1.0f);
    void reset();
    
private:
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared = false;
};
