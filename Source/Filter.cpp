/*
  ==============================================================================

    Filter.cpp
    Created: 20 Feb 2019 4:47:37pm
    Author:  Joonas

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================
MyFilter::MyFilter(AudioProcessorValueTreeState& vt) : mParameters(vt)
{
	// Empty constructor
}

MyFilter::~MyFilter()
{
	// Empty destructor
}

 // Process sample
float MyFilter::process(float sample, int channel)
{
	int filterType = *mParameters.getRawParameterValue("selectFilter");

	switch (filterType)
	{
		case 0:
			return lowPass(sample, channel);
		case 1:
			return highPass(sample, channel);
		case 2 :
			return bandPass(sample, channel);
		default:
			return 0.0f;
	}
}

float MyFilter::lowPass(float sample, int channel)
{
	float fc = *mParameters.getRawParameterValue("fc");
	float res = *mParameters.getRawParameterValue("res");
	float fs = mParameters.state[IDs::fs];
	return 0.0f;
}

float MyFilter::highPass(float sample, int channel)
{
	float fc = *mParameters.getRawParameterValue("fc");
	float res = *mParameters.getRawParameterValue("res");
	float fs = mParameters.state[IDs::fs];
	return 0.0f;
}

float MyFilter::bandPass(float sample, int channel)
{
	float fc = *mParameters.getRawParameterValue("fc");
	float res = *mParameters.getRawParameterValue("res");
	float fs = mParameters.state[IDs::fs];
	return 0.0f;
}
