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
	//mFs.referTo(mParameters.state, IDs::fs, nullptr);
	//mFc.referTo(mParameters.state, IDs::fc, nullptr);
	//mGain.referTo(mParameters.state, IDs::gain, nullptr);
}

MyFilter::~MyFilter()
{
}

float MyFilter::applyFilter(float sample, int channel)
{
	return firstOrderLowPass(sample, channel);
}

float MyFilter::firstOrderLowPass(float sample, int channel)
{
	float* fcPointer = mParameters.getRawParameterValue("fc");
	float* gainPointer = mParameters.getRawParameterValue("res");
	float fc = *fcPointer;
	float gain = *gainPointer;
	float fs = mParameters.state[IDs::fs];

	float V0 = pow(10.f, gain / 20.f);
	float H0 = V0 - 1.f;
	float c;
	if (gain > 0)
		c = (tan(M_PI * fc / fs) - 1.f) / (tan(M_PI * fc / fs) + 1.f);
	else
		c = (tan(M_PI * fc / fs) - V0) / (tan(M_PI * fc / fs) + V0);
	float xh = sample - c * mPrevXh[channel];
	float y1 = c * xh + mPrevXh[channel];
	mPrevXh[channel] = xh;
	return H0 * .5f * (sample + y1) + sample;
}
