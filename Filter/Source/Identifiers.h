/*
  ==============================================================================

    Identifiers.h
    Created: 20 Feb 2019 5:09:03pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

namespace IDs {

	#define DECLARE_ID(name) const juce::Identifier name (#name);

	DECLARE_ID(fc) // Center frequency
	DECLARE_ID(Q)  // Quality factor
	DECLARE_ID(Gain)  // Gain

	#undef DECLARE_ID
}