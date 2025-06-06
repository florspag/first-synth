/*
  ==============================================================================

    SynthVoice.h
    Created: 31 Mar 2025 10:47:29pm
    Author:  florentspagni

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "Data/OscData.h"
#include "Data/FilterData.h"

class SynthVoice  : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);

    void updateFilter(const int filterType, const float frequency, const float resonance);
   
    OscData& getOscillator(){return osc;};
    AdsrData& getAdsr(){return adsr;};
    AdsrData& getModAdsr(){return modAdsr;};
private:
    juce::AudioBuffer<float> synthBuffer;
    
    OscData osc;
    AdsrData adsr;
    AdsrData modAdsr;
    FilterData filter;
    juce::dsp::Gain<float> gain;
    
    bool isPrepared {false};
};
