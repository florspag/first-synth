/*
  ==============================================================================

    OscComponent.cpp
    Created: 22 Apr 2025 9:06:55pm
    Author:  florentspagni

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID,juce::String fmFreqSelectorID, juce::String fmDepthSelectorID)
{
    juce::StringArray choices {"Sine", "Saw", "Square"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);
    
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorID,  oscWaveSelector);
    
    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    waveSelectorLabel.setFont(juce::Font(juce::FontOptions(15.0)));
    addAndMakeVisible(waveSelectorLabel);

    setSliderWithLabel(fmDepthSlider, fmDepthLabel, apvts,  fmDepthSelectorID, fmDepthAttachement );
    setSliderWithLabel(fmFreqSlider, fmFreqLabel, apvts,  fmFreqSelectorID, fmFreqAttachement );
    
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    auto labelSpace = bounds.removeFromTop(20.0f);
    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("OSC", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 2.0f);
}

void OscComponent::resized()
{
    
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    const auto ySpacing = 20;
    const auto xSpacing = 5;

    waveSelectorLabel.setBounds(xSpacing, 20, 90, labelHeight );
    oscWaveSelector.setBounds(xSpacing, waveSelectorLabel.getHeight()+ySpacing, 90, 20);
    
    fmFreqSlider.setBounds(xSpacing, oscWaveSelector.getBottom() + ySpacing , sliderWidth, sliderHeight);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getHeight()-labelYOffset, fmFreqSlider.getWidth(), labelHeight);
    
    fmDepthSlider.setBounds(fmFreqSlider.getRight(),oscWaveSelector.getBottom() + ySpacing , sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(),  fmFreqSlider.getHeight()-labelYOffset, fmFreqSlider.getWidth(), labelHeight);
}

void OscComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment )
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

