/*
  ==============================================================================

    AdsrData.h
    Created: 16 Apr 2025 11:15:31pm
    Author:  florentspagni

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class AdsrData : public juce::ADSR
{
public:
    void updateADSR(const  float attack, const  float decay,const  float sustain,const  float release);
private:
    juce::ADSR::Parameters adsrParms;
};
