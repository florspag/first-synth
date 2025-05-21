/*
  ==============================================================================

    FilterComponent.cpp
    Created: 15 May 2025 9:40:56pm
    Author:  florentspagni

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorID,juce::String cutoffSelectorID, juce::String resonanceSelectorID)
{
    juce::StringArray choices {"Low-Pass", "Band-Pass", "High-Pass"};
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);
    
    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorID,  filterTypeSelector);
    
    filterTypeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterTypeLabel.setJustificationType(juce::Justification::left);
    filterTypeLabel.setFont(juce::Font(juce::FontOptions(15.0)));
    addAndMakeVisible(filterTypeLabel);

    setSliderWithLabel(filterCutoff, cutoffLabel, apvts,  cutoffSelectorID, cutOffAttachement );
    setSliderWithLabel(resonance, resonanceLabel, apvts,  resonanceSelectorID, resonanceAttachement );
    
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    
    auto bounds = getLocalBounds();
    auto labelSpace = bounds.removeFromTop(20.0f);
    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Filter", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 2.0f);

}

void FilterComponent::resized()
{
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    const auto ySpacing = 20;
    const auto xSpacing =5;

    filterTypeLabel.setBounds(xSpacing, 20, 90,  labelHeight);
    filterTypeSelector.setBounds(xSpacing, filterTypeLabel.getHeight()+ySpacing, 90, 20);
 
    
    filterCutoff.setBounds(xSpacing, filterTypeSelector.getBottom()+ySpacing, sliderWidth, sliderHeight);
    cutoffLabel.setBounds(filterCutoff.getX(), filterCutoff.getHeight()-labelYOffset, filterCutoff.getWidth(), labelHeight);
    
    resonance.setBounds(filterCutoff.getRight(), filterTypeSelector.getBottom()+ySpacing, sliderWidth, sliderHeight);
    resonanceLabel.setBounds(resonance.getX(), resonance.getY()-labelYOffset, resonance.getWidth(), labelHeight);
    
    
}

void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment )
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
    attachment =std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID,  slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::Font(juce::FontOptions(15.0)));
    addAndMakeVisible(label);
}

