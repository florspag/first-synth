/*
  ==============================================================================

    OscData.cpp
    Created: 19 Apr 2025 9:19:04am
    Author:  florentspagni

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec){
    fmOsc.prepare(spec);
    prepare(spec);
}

void OscData::setOscWaveType(const int choice){
    switch (choice) {
        case 0:
            // sin wave
            initialise([](float x){return std::sin(x);});
            break;
        case 1:
            // saw wave
            initialise ([](float x) { return x / juce::MathConstants<float>::pi; });
            break;
        case 2:
            // square wave
            initialise([](float x){ return x < 0.0f ? -1.0f : 1.0f; });
            break;
        default:
            jassert(false);
            break;
    }
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& audioBlock){
    for (int ch=0; ch<audioBlock.getNumChannels(); ++ch){
        for (int sample=0; sample<audioBlock.getNumSamples(); ++sample){
            fmMod = fmOsc.processSample(audioBlock.getSample(ch, sample)) * fmDepth;
        }
    }
    process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
}


void OscData::setWaveFrequency(const int midiNoteNumber){
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod );
    lastMidiNote = midiNoteNumber;
}

// FM modulation => s = AmpSignal * sin(2pi freq t + fmDepth * sin (2pi fmFreq t))
void OscData::setFMParams(const float depth, const float frequency){
    fmOsc.setFrequency(frequency);
    fmDepth=depth;
    auto currentFreq =juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency( currentFreq >= 0 ? currentFreq : -1.0f * currentFreq);
}
