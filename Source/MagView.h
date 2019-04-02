/*
  ==============================================================================

    MagView.h
    Created: 20 Feb 2019 4:48:07pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Identifiers.h"

//==============================================================================
/*
*/
class MagView : public Component, public Timer
{
public:
	MagView(AudioProcessorValueTreeState& vt);
	~MagView();

	void paint(Graphics&) override;
	void paintMarkers(Graphics& g, float scaleX, float minX, float fs);
	void resized() override;
	void updateFilter();
	void timerCallback() override;
	void calcMagResponse();
	enum
	{
		fftOrder = 14,
		fftSize = 1 << fftOrder
	};

private:
	// Reference of plugin parameters
	AudioProcessorValueTreeState& mParameters;
	// Filter to draw magnitude impulse response
	dsp::StateVariableFilter::Filter<float> mStateVariableFilter;
	dsp::FFT mFFT;
	float mImpulse[fftSize] = { 0.0f };
	float mFilteredImpulse[fftSize * 2] = { 0.0f };
	float mOldFc = 0;
	float mOldRes = 0;
	float mOldSelect = -1;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagView)
};
