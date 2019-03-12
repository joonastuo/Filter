/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterAudioProcessor::FilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	   mParameters(*this, nullptr)
#endif
{
	initialiseParameters();
}


FilterAudioProcessor::~FilterAudioProcessor()
{
	// Empty destructor
}

//==============================================================================
void FilterAudioProcessor::initialiseParameters()
{
	// Range of filter centre frequency (20 - 20 000 Hz)
	NormalisableRange<float> fcRange(20.f, 20000.f);
	// Range of filter resonance
	NormalisableRange<float> resRange(0.5f, 5.f);
	// Range of filter type selection (0=LP, 1=HP, 2=BP)
	NormalisableRange<float> selectRange(FilterType::lowpass, FilterType::bandpass);
	// Add centre frequency to ValueTree
	mParameters.createAndAddParameter(IDs::filterFrequency, "Freq", String(), fcRange, 1000.f, nullptr, nullptr);
	// Add resonance to ValueTree
	mParameters.createAndAddParameter(IDs::res, "Res", String(), resRange, 1.f, nullptr, nullptr);
	// Add filter type selection to ValueTree
	mParameters.createAndAddParameter(IDs::filterType, "FilterType" , String(), selectRange, FilterType::lowpass, nullptr, nullptr);
	// Initialise ValueTree
	mParameters.state = ValueTree("FilterParameters");
	// Set sampling frequency as a value tree parameter
	// so it can be accessed trought mParameters.state
	mParameters.state.setProperty(IDs::fs, 44100.f, nullptr);
}
//==============================================================================
const String FilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterAudioProcessor::setCurrentProgram (int index)
{
}

const String FilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Set current sampling rate as fs in ValueTree
	mParameters.state.setProperty(IDs::fs, sampleRate, nullptr);
	// Structure containing information needed by the filter
	// passed to the filter with prepare(spec)
	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();
	mStateVariableFilter.prepare(spec);
	// Reset filters processing pipeline
	mStateVariableFilter.reset();
}

void FilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	float fs = mParameters.state[IDs::fs];
	dsp::AudioBlock<float> block(buffer);
	updateFilter();
	mStateVariableFilter.process(dsp::ProcessContextReplacing <float>(block));
}

void FilterAudioProcessor::updateFilter()
{
	const float fc =	   *mParameters.getRawParameterValue(IDs::filterFrequency);
	const float res =	   *mParameters.getRawParameterValue(IDs::res);
	const int filterType = *mParameters.getRawParameterValue(IDs::filterType);
	const float fs =		mParameters.state[IDs::fs];

	mStateVariableFilter.state->setCutOffFrequency(fs, fc, res);

	switch (filterType)
	{
	case lowpass:  mStateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;  break;
	case highpass: mStateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass; break;
	case bandpass: mStateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass; break;
	default: break;
	}
}

//==============================================================================
bool FilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
    return new FilterAudioProcessorEditor (*this);
}

//==============================================================================
void FilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	auto state = mParameters.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void FilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mParameters.state.getType()))
			mParameters.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// Return a reference of filter parameters
AudioProcessorValueTreeState & FilterAudioProcessor::getState()
{
	return mParameters;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterAudioProcessor();
}
