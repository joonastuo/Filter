/*
  ==============================================================================

    Identifiers.h
    Created: 20 Feb 2019 5:09:03pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

// Parameter IDs used to access values
namespace IDs {
	// JUCE Identifiers
	#define DECLARE_ID(name) const juce::Identifier name (#name);

	DECLARE_ID(fs)		// Sampling frequency

	#undef DECLARE_ID

	// JUCE Strings
	#define DECLARE_ID(name) const juce::String name (#name);

	DECLARE_ID(filterFrequency)
	DECLARE_ID(resonance)
	DECLARE_ID(filterType)

	#undef DECLARE_ID
}

// Parameter names used in labels
namespace NAMEs {
	#define DECLARE_NAME(name) const juce::String name (#name);

	DECLARE_NAME(Freq)
	DECLARE_NAME(Res)
	DECLARE_NAME(Type)

	#undef DECLARE_NAME
}

enum FilterType {
	lowpass,
	highpass,
	bandpass
};
