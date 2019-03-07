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
class MyFilter
{
public:
    MyFilter(AudioProcessorValueTreeState&);
    ~MyFilter();
	float applyFilter(float sample, int channel);
	float firstOrderLowPass(float sample, int channel);

private:
	AudioProcessorValueTreeState& mParameters;

	//CachedValue<float> mFc;
	//CachedValue<float> mGain;
	//CachedValue<float> mFs;

	float mPrevXh[2] = {0.f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyFilter)
};
