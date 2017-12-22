/**
 * PluginEditor.cpp
 * \brief Audio processor editor class for stereo delay VST plugin.
 * \author Chris Harless (chris.harless3@gmail.com)
 */

#include "PluginEditor.h"

StereoDelayEditor::StereoDelayEditor (StereoDelayProcessor* processor)
    : AudioProcessorEditor (processor),
      SliderListener(), ButtonListener(),
      m_pluginLabel ("plugin name", "Stereo Delay"),
      m_delayLabel ("delay label", "Delay"),
      m_delayKnob ("delay knob"),
      m_feedbackLabel ("feedback", "Feedback"),
      m_feedbackKnob ("feedback knob"), 
      m_mixLabel ("mix label", "Mix"),
      m_mixKnob ("mix knob"),
      m_bypassButton ("bypass button")
{
    // Set up the window.
    addAndMakeVisible (m_pluginLabel);
    m_pluginLabel.setFont (22.00f);
    m_pluginLabel.setJustificationType (Justification::centred);
    m_pluginLabel.setEditable (false, false, false);
    setResizable (true, true);
    setSize (550, 300);

    // Set up the delay time control.
    addAndMakeVisible (m_delayLabel);
    m_delayLabel.setFont (18.00f);
    m_delayLabel.setJustificationType (Justification::centred);
    m_delayLabel.attachToComponent (&m_delayKnob, false);
    addAndMakeVisible (m_delayKnob);
    m_delayKnob.setTooltip ("Delay time (msecs)");
    m_delayKnob.setRange (0, 2000, 0.01);
    m_delayKnob.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    m_delayKnob.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    m_delayKnob.setTextValueSuffix (" msecs");
    m_delayKnob.addListener (this);

    // Set up the feedback control.
    addAndMakeVisible (m_feedbackLabel);
    m_feedbackLabel.setTooltip ("Feedback (%)");
    m_feedbackLabel.setFont (18.00f);
    m_feedbackLabel.setJustificationType (Justification::centred);
    m_feedbackLabel.attachToComponent (&m_feedbackKnob, false);
    addAndMakeVisible (m_feedbackKnob);
    m_feedbackKnob.setTooltip ("Feedback");
    m_feedbackKnob.setRange (0, 100, 1);
    m_feedbackKnob.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    m_feedbackKnob.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    m_feedbackKnob.setTextValueSuffix (" %");
    m_feedbackKnob.addListener (this);

    // Set up the mix control.
    addAndMakeVisible (m_mixLabel);
    m_mixLabel.setTooltip ("Wet/dry mix (%)");
    m_mixLabel.setFont (18.00f);
    m_mixLabel.setJustificationType (Justification::centred);
    m_mixLabel.attachToComponent (&m_mixKnob, false);
    addAndMakeVisible (m_mixKnob);
    m_mixKnob.setRange (0, 100, 1);
    m_mixKnob.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    m_mixKnob.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    m_mixKnob.setTextValueSuffix (" %");
    m_mixKnob.addListener (this);

    // Setup a button for bypassing the effect.
    addAndMakeVisible (m_bypassButton);
    m_bypassButton.setButtonText ("Bypass");
    m_bypassButton.setClickingTogglesState (true);
    m_bypassButton.addListener (this);

    // Set values for the controls from the saved processor state.
    m_delayKnob.setValue (processor->getParameter (StereoDelayProcessor::DELAY), dontSendNotification);
    m_feedbackKnob.setValue (processor->getParameter (StereoDelayProcessor::FEEDBACK), dontSendNotification);
    m_mixKnob.setValue (processor->getParameter (StereoDelayProcessor::MIX), dontSendNotification);
    m_bypassButton.setToggleState (static_cast<bool> (processor->getParameter (StereoDelayProcessor::BYPASS)), dontSendNotification);
}

void StereoDelayEditor::paint (Graphics& graphics)
{
    graphics.fillAll (Colour (0xff353131));
    graphics.setColour (Colours::black.withAlpha (0.850f));
}

void StereoDelayEditor::resized()
{
    m_pluginLabel.setBounds (proportionOfWidth(0.25), proportionOfHeight(0.0), proportionOfWidth(0.5), proportionOfHeight(0.2));
    m_delayKnob.setBounds (proportionOfWidth(0.1), proportionOfHeight(0.3), proportionOfWidth(0.2), proportionOfHeight(0.3));
    m_feedbackKnob.setBounds (proportionOfWidth(0.4), proportionOfHeight(0.3), proportionOfWidth(0.2), proportionOfHeight(0.3));
    m_mixKnob.setBounds (proportionOfWidth(0.7), proportionOfHeight(0.3), proportionOfWidth(0.2), proportionOfHeight(0.3));
    m_bypassButton.setBounds (proportionOfWidth (0.4), proportionOfHeight (0.7), proportionOfWidth (0.2), proportionOfHeight (0.1));
}

void StereoDelayEditor::sliderValueChanged (Slider* slider)
{
    auto processor = getProcessor();

    if (slider == &m_delayKnob)
    {
        processor->setParameterNotifyingHost (StereoDelayProcessor::DELAY, m_delayKnob.getValue());
    }
    else if (slider == &m_feedbackKnob)
    {
        processor->setParameterNotifyingHost (StereoDelayProcessor::FEEDBACK, m_feedbackKnob.getValue());
    }
    else if (slider == &m_mixKnob)
    {
        processor->setParameterNotifyingHost (StereoDelayProcessor::MIX, m_mixKnob.getValue());
    }
}

void StereoDelayEditor::buttonClicked (Button* button)
{
    auto processor = getProcessor();

    if (button == &m_bypassButton)
    {
        processor->setParameterNotifyingHost (StereoDelayProcessor::BYPASS, static_cast<float>(m_bypassButton.getToggleState()));
    }
}
