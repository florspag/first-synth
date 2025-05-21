/*
  ==============================================================================

    FilterData.cpp
    Created: 15 May 2025 8:52:13pm
    Author:  florentspagni

  ==============================================================================
*/

#include "FilterData.h"
void FilterData::prepareToPlay (double sampleRate, int samplesPerBlock, int numOuputChannels){
    
    filter.reset();

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize =  samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOuputChannels;

    filter.prepare(spec);
    
    isPrepared = true;
};

void FilterData::processBlock (juce::AudioBuffer<float>& buffer){
    
    jassert(isPrepared); // checks that is prepared
    
    juce::dsp::AudioBlock<float> audioBlock{buffer};
    filter.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});
};

void FilterData::updateParameters(const int filterType, const float frequency, const float resonance, const float modulator){
    switch (filterType) {
        case 0:
            filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
        case 1:
            filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            break;
        case 3:
            filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            break;
    };
    
    float modulatedFreq = frequency * modulator;
    
    // bound the modulated frequency between 20hz and 20khz
    modulatedFreq = std::fmax (std::fmin (modulatedFreq, 20000.0f), 20.0f);
    
    filter.setCutoffFrequency(modulatedFreq);
    filter.setResonance(resonance);
};

void FilterData::reset(){
    filter.reset();
};
