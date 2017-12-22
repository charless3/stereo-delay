/**
 * PluginEditor.h
 * \brief Audio processor editor class for stereo delay VST plugin.
 * \author Chris Harless (chris.harless3@gmail.com)
 *
 * Ideas for new features and improvements:
 *     1. Add separate delay time parameters for each channel.
 *     2. Add optional modulation to the delayed signals (with controls like rate/depth).
 *     3. Add optional fuzz/distortion/noise to the delayed signals (with controls like volume/gain).
 *     4. Add send/receive ports to allow users to process delayed signals with
 *        other VST plugins or algorithms.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"

/**
 * \brief Editor user interface class for a stereo delay VST plugin.
 *
 * This class builds a user interface with three parameters (delay time, feedback, mix)
 * and a bypass button. The parameters can be changed by turning their respective knobs.
 * Currently, this editor supports delay values up to 2 seconds.
 */
class StereoDelayEditor : public AudioProcessorEditor, public SliderListener, public ButtonListener
{
public:

    /**
     * Class constructor.
     *
     * \param[in]  StereoDelayProcessor&  Stereo delay processor
     */
    StereoDelayEditor (StereoDelayProcessor* processor);

    /**
     * Class destructor.
     */
    ~StereoDelayEditor() = default;

    /**
     * Handles the editor window graphics.
     *
     * \param[in]  Graphics&  JUCE Graphics object
     */
    void paint (Graphics& graphics) override;

    /**
     * Handles the placement of editor components at start and after resizing.
     */
    void resized() override;

    /**
     * Accessor for the stereo delay audio processor to enable parameter changes.
     *
     * \return  StereoDelayProcessor*  Pointer to the stereo delay processor
     */
    StereoDelayProcessor* getProcessor() { return static_cast<StereoDelayProcessor*> (getAudioProcessor()); };

    /**
     * Listener method for capturing slider/knob movements.
     *
     * \param[in]  Slider*  Slider that was moved
     */
    void sliderValueChanged (Slider* slider) override;

    /**
     * Listener method for capturing button clicks.
     *
     * \param[in]  Button*  Button that was clicked
     */
    void buttonClicked (Button* button) override;

private:

    Label m_pluginLabel; ///< Plugin name label.
    Label m_delayLabel; ///< Delay knob label.
    Slider m_delayKnob; ///< Knob for adjusting the delay time (msecs).
    Label m_feedbackLabel; ///< Feedback knob label.
    Slider m_feedbackKnob; ///< Knob for adjusting the feedback (%).
    Label m_mixLabel; ///< Mix knob label.
    Slider m_mixKnob; ///< Knob for adjusting the wet/dry mix (%).
    TextButton m_bypassButton; ///< Button for bypassing the effect processor.
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayEditor)
};
