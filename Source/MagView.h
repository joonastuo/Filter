/*
  ==============================================================================

    MagView.h
    Created: 20 Feb 2019 4:48:07pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MagView    : public Component
{
public:
    MagView(AudioProcessorValueTreeState& vt);
    ~MagView();

    void paint (Graphics&) override;
    void resized() override;

private:
	AudioProcessorValueTreeState& mParameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagView)
};
