/*
  ==============================================================================

    StateVariableFilterWrapper.h
    Created: 26 Mar 2019 3:15:50pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Identifiers.h"

class StateViableFilterWrapper
{
public:
	StateViableFilterWrapper(AudioProcessorValueTreeState& state);
	~StateViableFilterWrapper();
	void prepare(dsp::ProcessSpec spec);
	void process(AudioBuffer<float>& buffer);
	void update();

private:
	AudioProcessorValueTreeState& mState;
	dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, 
							 dsp::StateVariableFilter::Parameters <float>> mStateVariableFilter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateViableFilterWrapper)
};
