/*
  ==============================================================================

    Identifiers.h
    Created: 20 Feb 2019 5:09:03pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

namespace IDs {
	// JUCE Identifiers
	#define DECLARE_ID(name) const juce::Identifier name (#name);

	DECLARE_ID(fs)		// Sampling frequency

	#undef DECLARE_ID

	// JUCE Strings
	#define DECLARE_ID(name) const juce::String name (#name);

	DECLARE_ID(filterFrequency)
	DECLARE_ID(res)
	DECLARE_ID(filterType)

	#undef DECLARE_STR
}

enum FilterType {
	lowpass,
	highpass,
	bandpass
};
