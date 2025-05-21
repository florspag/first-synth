/*
  ==============================================================================

    AdsrData.cpp
    Created: 16 Apr 2025 11:15:31pm
    Author:  florentspagni

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release){
    
    adsrParms.attack = attack;
    adsrParms.decay = decay;
    adsrParms.sustain = sustain;
    adsrParms.release = release;
    
    setParameters(adsrParms);
}
