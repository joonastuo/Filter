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
Filter::Filter(AudioProcessorValueTreeState& vt) : mParameters(vt)
{
	mFs.referTo(mParameters.state, IDs::fs, nullptr);
	mFc.referTo(mParameters.state, IDs::fc, nullptr);
	mGain.referTo(mParameters.state, IDs::gain, nullptr);
}

Filter::~Filter()
{
}

float Filter::applyFilter(float sample)
{
	return firstOrderLowPass(sample);
}

float Filter::firstOrderLowPass(float sample)
{
	float V0 = pow(10, mGain / 20);
	float H0 = V0 - 1;
	float c;
	if (mGain > 0)
		c = (tan(M_PI * mFc / mFs) - 1) / (tan(M_PI * mFc / mFs) + 1);
	else
		c = (tan(M_PI * mFc / mFs) - V0) / (tan(M_PI * mFc / mFs) + V0);
	float xh = sample - c * mPrevXh;
	float y1 = c * xh + mPrevXh;
	mPrevXh = xh;
	return (H0 / 2) * (sample + y1) + sample;
}
