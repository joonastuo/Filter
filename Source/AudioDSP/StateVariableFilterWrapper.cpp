/*
  ==============================================================================

    StateVariableFilterWrapper.cpp
    Created: 26 Mar 2019 3:15:50pm
    Author:  Joonas

  ==============================================================================
*/

#include "StateVariableFilterWrapper.h"

StateViableFilterWrapper::StateViableFilterWrapper(AudioProcessorValueTreeState& state)
	: mState(state)
{
	// Empty constructor
}

StateViableFilterWrapper::~StateViableFilterWrapper()
{
	// Empty destructor
}

void StateViableFilterWrapper::prepare(dsp::ProcessSpec spec)
{
	mStateVariableFilter.prepare(spec);
	mStateVariableFilter.reset();
}

void StateViableFilterWrapper::process(AudioBuffer<float>& buffer)
{
	dsp::AudioBlock<float> block(buffer);
	update();
	mStateVariableFilter.process(dsp::ProcessContextReplacing <float>(block));
}

void StateViableFilterWrapper::update()
{
	const float fc =	   *mState.getRawParameterValue(IDs::filterFrequency);
	const float res =	   *mState.getRawParameterValue(IDs::resonance);
	const int filterType = static_cast<int>(*mState.getRawParameterValue(IDs::filterType));
	const float fs =		mState.state[IDs::fs];

	mStateVariableFilter.state->setCutOffFrequency(fs, fc, res);

	switch (filterType)
	{
	case lowpass:  mStateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;  break;
	case highpass: mStateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass; break;
	case bandpass: mStateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass; break;
	default: break;
	}
}
