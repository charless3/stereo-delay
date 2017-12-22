/**
 * PluginProcessor.h
 * \brief Audio processor class for a stereo delay VST plugin.
 * \author Chris Harless (chris.harless3@gmail.com)
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoDelayProcessor::StereoDelayProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
  : AudioProcessor (BusesProperties()
                    #if ! JucePlugin_IsMidiEffect /// \todo Probably don't need these checks.
                      #if ! JucePlugin_IsSynth
                        .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                        .withOutput ("Output", AudioChannelSet::stereo(), true)
                    #endif
                    ),
    m_numParams(4),
    m_delay(0.0f),
    m_feedback(0.0f),
    m_mix(50.0f),
    m_bypass(false),
	m_delayChannel0(),
	m_delayChannel1()
	
#endif
{}

void StereoDelayProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
    m_delayChannel0.reset();
    m_delayChannel1.reset();
}

void StereoDelayProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
    auto channel0 = buffer.getWritePointer(0);
    auto channel1 = buffer.getWritePointer(1);

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        // Process the first channel.
        channel0[i] = m_delayChannel0.processSample(channel0[i]);                
        
        // Process the second channel. Just copy the first channel for mono output.
        if (getTotalNumInputChannels() == 1 && getTotalNumOutputChannels() == 2) { channel1[i] = channel0[i]; }
        else if (getTotalNumInputChannels() == 2 && getTotalNumOutputChannels() == 2) { channel1[i] = m_delayChannel1.processSample(channel1[i]); }             
    }

    // Clear any additional output channels.
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i) { buffer.clear (i, 0, buffer.getNumSamples()); }
}

void StereoDelayProcessor::processBlockBypassed (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        auto data = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            data[i] = data[i];            
        }
    }
}

float StereoDelayProcessor::getParameter (int param)
{
    switch (param)
    {
        case DELAY:
            return m_delay;
        case FEEDBACK:
            return m_feedback;
        case MIX:
            return m_mix;
        case BYPASS:
            return m_bypass;
        default:
            return 0;
    }
}

void StereoDelayProcessor::setParameter (int param, float val)
{
    switch (param)
    {
        case DELAY:
            m_delay = val;
            m_delayChannel0.setDelay(m_delay);
            m_delayChannel1.setDelay(m_delay);
            break;
        case FEEDBACK:
            m_feedback = val;
            m_delayChannel0.setFeedback(m_feedback);
            m_delayChannel1.setFeedback(m_feedback);
            break;
        case MIX:
            m_mix = val;
            m_delayChannel0.setMix(m_mix);
            m_delayChannel1.setMix(m_mix);
            break;
        case BYPASS:
            m_bypass = static_cast<bool>(val);
            m_delayChannel0.setBypass(m_bypass);
            m_delayChannel1.setBypass(m_bypass);
            break;
        default:
            break;
    }
}

void StereoDelayProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement root ("Root");
    auto child = root.createNewChildElement ("Delay");
    child->addTextElement (String (m_delay));
    child = root.createNewChildElement ("Feedback");
    child->addTextElement (String (m_feedback));
    child = root.createNewChildElement ("Mix");
    child->addTextElement (String (m_mix));
    child = root.createNewChildElement ("Bypass");
    child->addTextElement (String (static_cast<float> (m_bypass)));
    copyXmlToBinary(root, destData);
}

void StereoDelayProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto root = getXmlFromBinary (data, sizeInBytes);
    if (root != nullptr)
    {
        forEachXmlChildElement (*root, child)
        {
            auto text = child->getAllSubText();

            if (child->hasTagName ("Delay")) { setParameter (DELAY, text.getFloatValue()); }
            else if (child->hasTagName ("Feedback")) { setParameter (FEEDBACK, text.getFloatValue()); }
            else if (child->hasTagName ("Mix")) { setParameter (MIX, text.getFloatValue()); }
            else if (child->hasTagName ("Bypass")) { setParameter (BYPASS, static_cast<bool> (text.getFloatValue())); }
        }
        delete root; /// \todo May not need this.
    }
}

AudioProcessorEditor* StereoDelayProcessor::createEditor()
{
    return new StereoDelayEditor (this);
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoDelayProcessor();
}
