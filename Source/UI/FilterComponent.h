/*
  ==============================================================================

    FilterComponent.h
    Created: 15 May 2025 9:40:56pm
    Author:  florentspagni

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorID, juce::String cutoffSelectorID, juce::String resonanceSelectorID);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::ComboBox filterTypeSelector {"FilterType"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;
    juce::Label filterTypeLabel{"FilterTypeLabel", "Filter Type"};
    
    juce::Slider filterCutoff;
    juce::Slider resonance;
    
    using Attachment =juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<Attachment>     cutOffAttachement;
    juce::Label cutoffLabel{"CutoffLabel", "Cutoff"};
    
    std::unique_ptr<Attachment>     resonanceAttachement;
    juce::Label resonanceLabel{"ResonanceLabel", "Resonance"};

    void setSliderWithLabel(juce::Slider& sidler, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment ) ;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
