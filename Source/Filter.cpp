/*
  ==============================================================================

    Filter.cpp
    Created: 20 Feb 2019 4:47:37pm
    Author:  Joonas

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

//==============================================================================
Filter::Filter(AudioProcessorValueTreeState& vt) : mParameters(vt)
{
}

Filter::~Filter()
{
}

float Filter::applyFilter(float sample)
{
	return 0.0f;
}
