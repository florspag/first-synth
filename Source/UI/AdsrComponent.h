/*
  ==============================================================================

    AdsrComponent.h
    Created: 16 Apr 2025 11:15:00pm
    Author:  florentspagni

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdsrComponent  : public juce::Component
{
public:
    AdsrComponent(juce::AudioProcessorValueTreeState& apvts,juce::String name, juce::String attackID, juce::String decayID,juce::String sustainID, juce::String releaseID);
    ~AdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    juce::Slider gainSlider;
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    
    juce::String adsrName;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> attackSliderAttachment;
    std::unique_ptr<SliderAttachment> decaySliderAttachment;
    std::unique_ptr<SliderAttachment> sustainSliderAttachment;
    std::unique_ptr<SliderAttachment> releaseSliderAttachment;

    
    
    void setSliderParams(juce::Slider& slider);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
