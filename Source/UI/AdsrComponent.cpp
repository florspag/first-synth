/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 16 Apr 2025 11:15:00pm
    Author:  florentspagni

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts, juce::String name, juce::String attackID, juce::String decayID,juce::String sustainID,juce::String releaseID)
{
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    attackSliderAttachment = std::make_unique<SliderAttachment>(apvts, attackID, attackSlider);
    decaySliderAttachment = std::make_unique<SliderAttachment>(apvts, decayID, decaySlider);
    sustainSliderAttachment = std::make_unique<SliderAttachment>(apvts, sustainID, sustainSlider);
    releaseSliderAttachment = std::make_unique<SliderAttachment>(apvts, releaseID, releaseSlider);

    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);
    
    adsrName=name;
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    auto labelSpace = bounds.removeFromTop(20.0f);
    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText(adsrName, labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 2.0f);
}

void AdsrComponent::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight()-20;
    const auto sliderStartX = 5;
    const auto sliderStartY = 25;
    
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
}

void AdsrComponent::setSliderParams(juce::Slider& slider){
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
