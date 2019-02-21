/*
  ==============================================================================

    Filter.h
    Created: 20 Feb 2019 4:47:37pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Identifiers.h"

//==============================================================================
/*
*/
class Filter
{
public:
    Filter(AudioProcessorValueTreeState&);
    ~Filter();
	float applyFilter(float sample);
	float firstOrderLowPass(float sample);

private:
	AudioProcessorValueTreeState& mParameters;

	CachedValue<float> mFc;
	CachedValue<float> mGain;
	CachedValue<float> mFs;

	float mPrevXh = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
