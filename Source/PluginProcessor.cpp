/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FirstSynthAudioProcessor::FirstSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

FirstSynthAudioProcessor::~FirstSynthAudioProcessor()
{
    
}

//==============================================================================
const juce::String FirstSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FirstSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FirstSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FirstSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FirstSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FirstSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FirstSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FirstSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FirstSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FirstSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FirstSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i=0; i<synth.getNumVoices(); i++){
        
        if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(i))){
            voice->prepareToPlay(sampleRate,samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void FirstSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FirstSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FirstSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // update voice
    {
        for (int i=0; i<synth.getNumVoices();++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                
                // FM OSC
                auto& oscWaveChoice= *apvts.getRawParameterValue("Osc1WaveType");
                auto& fmDepth= *apvts.getRawParameterValue("Osc1FMDepth");
                auto& fmFreq= *apvts.getRawParameterValue("Osc1FMFreq");
                
                // Amplification ADSR
                auto& attack = *apvts.getRawParameterValue("ATTACK");
                auto& decay = *apvts.getRawParameterValue("DECAY");
                auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
                auto& release = *apvts.getRawParameterValue("RELEASE");
                
                // Filter
                auto& filterType = *apvts.getRawParameterValue("FilterType");
                auto& filterCutOff = *apvts.getRawParameterValue("FilterCutOff");
                auto& filterRes = *apvts.getRawParameterValue("FilterRes");
                
                // Modulator ADSR
                auto& modAttack = *apvts.getRawParameterValue("MODAttack");
                auto& modDecay = *apvts.getRawParameterValue("MODDecay");
                auto& modSustain = *apvts.getRawParameterValue("MODSustain");
                auto& modRelease = *apvts.getRawParameterValue("MODRelease");
                
                voice->getOscillator().setOscWaveType(oscWaveChoice);
                voice->getOscillator().setFMParams(fmDepth, fmFreq);
                // atomic are heavier than regular float => load() value is atomic
                voice->getAdsr().updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
                voice->getModAdsr().updateADSR(modAttack.load(), modDecay.load(), modSustain.load(), modRelease.load());
                voice->updateFilter(filterType, filterCutOff, filterRes);
                
            }
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    

}

//==============================================================================
bool FirstSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FirstSynthAudioProcessor::createEditor()
{
    return new FirstSynthAudioProcessorEditor (*this);
}

//==============================================================================
void FirstSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FirstSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FirstSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FirstSynthAudioProcessor::createParameters(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // combo box : switch oscillator
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "OSC",  1 }, juce::String("Oscilator"), juce::StringArray{"Saw", "Square", "Sine"}, 0));
    
    
    // FM
    // freq
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "Osc1WaveType", 16 }, "Osc 1 Wave type", juce::StringArray("Sine", "Saw", "Square"), 0, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "Osc1FMFreq", 2 },"FM Frequency", juce::NormalisableRange<float>{0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    // depth
    params.push_back(std::make_unique<juce::AudioParameterFloat>( juce::ParameterID { "Osc1FMDepth", 3 },"FM Depth", juce::NormalisableRange<float>{0.0f, 1000.0f, 0.01f, 0.3f }, 0.0f));
    
    
    
    // attack
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "ATTACK", 4 },"Attack" , juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    // decay
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DECAY", 5 },"Decay" , juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    // sustain
    params.push_back(std::make_unique<juce::AudioParameterFloat>( juce::ParameterID { "SUSTAIN", 6 },"Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.0f));
    // release
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "RELEASE", 7 }, "Release", juce::NormalisableRange<float>{0.1f, 3.0f, 0.1f}, 0.4f));
    
    // Filter MOD
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "MODAttack", 8 },"Mod Attack" , juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    // decay
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "MODDecay", 9 },"Mod Decay" , juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    // sustain
    params.push_back(std::make_unique<juce::AudioParameterFloat>( juce::ParameterID { "MODSustain", 10 },"Mod Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.0f));
    // release
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "MODRelease",11 }, "Mod Release", juce::NormalisableRange<float>{0.1f, 3.0f, 0.1f}, 0.4f));
    


    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "GAIN", 12 },"Gain" ,0.0f, 1.0f, 0.5f));
    
    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "FilterType",  13 }, juce::String("Filter type"), juce::StringArray{"Low-Pass", "Band-Pass", "High-Pass"}, 0));
    // cut off (skewed)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "FilterCutOff", 14 },"Filter cutoff" , juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "FilterRes", 15 },"Filter resonance" , juce::NormalisableRange<float>{1.0f, 10.0f, 0.1f}, 1.0f));
    
    
    return {params.begin(), params.end()};
};

// Value Treee

