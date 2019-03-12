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
	//  Process sample
	float process(float sample, int channel);
	// Filters
	float lowPass(float sample, int channel);
	float highPass(float sample, int channel);
	float bandPass(float sample, int channel);

private:
	AudioProcessorValueTreeState& mParameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyFilter)
};
