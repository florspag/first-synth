/*
  ==============================================================================

    SynthVoice.cpp
    Created: 31 Mar 2025 10:47:29pm
    Author:  florentspagni

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
};
void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {

    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
    modAdsr.noteOn();
};

void SynthVoice::stopNote (float velocity, bool allowTailOff) {
    adsr.noteOff();
    modAdsr.noteOff();
    if(! allowTailOff || !adsr.isActive()){
        clearCurrentNote();
    }
};

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {
    
};

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);
    
    if ( !isVoiceActive()){
        return;
    }
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples,false, false, true);
    modAdsr.applyEnvelopeToBuffer(outputBuffer, 0, numSamples);
    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock{synthBuffer}; // just an alias

    osc.getNextAudioBlock(audioBlock);
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    filter.processBlock(synthBuffer);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    for (int channel =0;channel < outputBuffer.getNumChannels();++channel){
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel,0, numSamples);
        if (!adsr.isActive()){
            clearCurrentNote(); // clear state of voice
        }
    }
    

};

void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {
    
};

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepareToPlay(spec);
    
    modAdsr.setSampleRate(sampleRate);
    filter.prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
    adsr.setSampleRate(sampleRate);
    gain.prepare(spec);
    gain.setGainLinear(0.3f);
    
    isPrepared = true;
}



void SynthVoice::updateFilter(const int filterType, const float frequency, const float resonance)
{
    float modulator = modAdsr.getNextSample(); // get next sample modulated
    filter.updateParameters ( filterType, frequency, resonance, modulator);
}


