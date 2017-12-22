/**
 * PluginProcessor.h
 * \brief Audio processor class for a stereo delay VST plugin.
 * \author Chris Harless (chris.harless3@gmail.com)
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "DelayLine.h"

/**
 * \brief Audio processor class for a stereo delay VST plugin.
 *
 * This class processes blocks of audio samples using the parameters from the StereoDelayEditor
 * class and the algorithm in the DelayLine class.
 */
class StereoDelayProcessor : public AudioProcessor
{
public:

    /**
     * Enum for getting and setting parameter values.
     */
    enum Param { DELAY, FEEDBACK, MIX, BYPASS };

    /**
     * Class constructor.
     */
    StereoDelayProcessor();

    /**
     * Class destructor.
     */
    ~StereoDelayProcessor() = default;

    /**
     * Pre-playback initialization of the delay processor.
     *
     * \param[in]  double  Audio sample rate
     * \param[in]  double  Number of samples per processing block
     */
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;

    /**
     * Frees up any remaining resources when the plugin is closed.
     */
    void releaseResources() override {};

    /**
     * Process the audio buffer using the delay line.
     *
     * \note For mono inputs, the output is copied to both channels.
     * \todo The MIDI buffer is currently unused but should be supported in the future.
     *
     * \param[in] AudioSampleBuffer&  Audio buffer
     * \param[in] MidiBuffer&  MIDI buffer
     */
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    /**
     * Process block when the effect is bypassed.
     *
     * \param[in]  AudioSampleBuffer&  Audio buffer
     * \param[in]  MidiBuffer&  MIDI buffer
     */
    void processBlockBypassed (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    int getNumParameters() override { return m_numParams; }; ///< Returns the number of processor parameters.
    float getParameter (int param) override; ///< Gets a specified parameter value.
    void setParameter (int param, float val) override; ///< Sets a specified parameter value based on the index.

    bool hasEditor() const override { return true; }; ///< Indicates whether this plugin has an editor.
    const String getName() const override { return JucePlugin_Name; }; ///< Gets the name of the plugin.

    bool acceptsMidi() const override { return JucePlugin_WantsMidiInput; }; ///< Indicates whether this plugin accepts MIDI input.
    bool producesMidi() const override { return JucePlugin_ProducesMidiOutput; }; ///< Indicates whether this plugin produces MIDI output.
    double getTailLengthSeconds() const override { return 0.0; }; ///< Gets the processor tail length (unused).
    
    int getNumPrograms() override { return 1; }; ///< Gets the number of programs (unused).
    int getCurrentProgram() override { return 0; }; ///< Gets the current program (unused).
    void setCurrentProgram (int /*index*/) override {}; ///< Sets the current program (unused).
    const String getProgramName (int /*index*/) override { return String::empty; }; ///< Gets the name of the program (unused);
    void changeProgramName (int /*index*/, const String& /*newName*/) override {}; ///< Changes the name of the program (unused).

    /**
     * Gets the parameter values that were saved when the plugin was last closed.
     *
     * \param[in]  MemoryBlock&  Saved state data
     */
    void getStateInformation (MemoryBlock& destData) override;

    /**
     * Saves the parameter values to an XML file.
     *
     * \param[in]  void*  Pointer to the data
     * \param[in]  sizeInBytes  Size of the data
     */
    void setStateInformation (const void* data, int sizeInBytes) override;

    /**
     * Creates a new plugin editor.
     *
     * \return  AudioProcessorEditor*  Pointer to the editor
     */
    AudioProcessorEditor* createEditor() override;

private:

    int m_numParams; ///< Number of processor parameters.
    float m_delay; ///< Delay time parameter (msecs).
    float m_feedback; ///< Feedback parameter (%).
    float m_mix; ///< Mix parameter (%).
    bool m_bypass; ///< Bypass parameter (true = bypass).

    DelayLine m_delayChannel0; ///< 
    DelayLine m_delayChannel1; ///< 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayProcessor)
};
