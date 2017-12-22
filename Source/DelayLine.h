/**
 * DelayLine.h
 * \brief Delay line processor class.
 * \author Chris Harless (chris.harless3@gmail.com)
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <cassert>
#include <cstdlib>

/**
 * \class 
 */
class DelayLine
{
public:

    /**
     * Class constructor.
     *
     * \param[in]  int  Sample frequency
     * \param[in]  float  Delay time (msecs)
     * \param[in]  float  Feedback (%)
     * \param[in]  float  Mix (%)
     */
    DelayLine (const int fs = 44100, const float delay = 0, const float feedback = 0, const float mix = 0.5);

    /**
     * Class destructor.
     */
    ~DelayLine();

    /**
     * Resets the delay line by flushing the buffer and initializing the delay parameters.
     */
    void reset();

    /**
     * Calculates the delayed value of the input signal.
     *
     * \param[in]  float  Input data sample
     *
     * \return  float  Output amplitude of the delayed signal
     */
    float processSample (const float input);

    void setReadPos(); ///< Sets the buffer read position based on the delay and size of the buffer.
    void setDelay (float delay) { m_delay = delay; setReadPos(); }; ///< Sets the delay parameter and updates the buffer read position.
    void setFeedback (float feedback) { m_feedback = feedback/100; }; ///< Sets the feedback parameter (0-1).
    void setMix (float mix) { m_mix = mix/100; }; ///< Sets the mix parameter (0-1).
    void setBypass (bool bypass) { m_bypass = bypass; }; ///< Sets the bypass parameter (true = bypass).

private:

    double m_sampleFreq; ///< Audio sample rate.
    float m_delay; ///< Delay time parameter (msecs).
    float m_feedback; ///< Feedback parameter (%).
    float m_mix; ///< Mix parameter (%).
    bool m_bypass; ///< Bypass parameter (true = bypass).

    int m_readPos; ///< Input buffer read position.
    int m_writePos; ///< Output buffer write position.

    int m_delaySamples; ///< Number of samples corresponding to m_delay.
    int m_maxDelaySamples; ///< Maximum number of delayed samples.
    float m_maxDelay; ///< Maximum delay time (currently set to 2 secs).
    float m_delayFraction; ///< Fractional delay time (msecs).

    float* m_buffer; ///< Delayed signal buffer.
};
