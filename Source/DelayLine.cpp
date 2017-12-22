/**
 * DelayLine.h
 * \brief Delay line processor class.
 * \author Chris Harless (chris.harless3@gmail.com)
 */

#include <cstring>

#include "DelayLine.h"

DelayLine::DelayLine(const int fs, const float delay, const float feedback, const float mix)
    : m_sampleFreq (fs),
      m_delay (delay), m_feedback (feedback), m_mix (mix), m_bypass(),
      m_readPos(), m_writePos(),
      m_delaySamples (floor(fs*1e-3*delay)),
      m_maxDelaySamples (ceil(fs*1e-3*2000)),
      m_maxDelay ((m_maxDelaySamples * 1000.0f) / fs),
      m_delayFraction ((fs*1e-3*delay) - m_delaySamples),
      m_buffer(nullptr)
{
    reset();
}

DelayLine::~DelayLine()
{
    delete [] m_buffer;
}

void DelayLine::reset()
{
    if (m_buffer != nullptr) { delete [] m_buffer; }

    m_buffer = new float [m_maxDelaySamples];
    memset (m_buffer, 0, m_maxDelaySamples*sizeof(float));
    m_readPos = m_writePos = 0;
    setReadPos();
}

float DelayLine::processSample (const float input)
{
    if (m_bypass) { return input; }

    float out = 0;
    if (m_delaySamples == 0)
    {
        out = input;
    }
    else
    {
        // Get the delayed sample value.
        out = m_buffer[m_readPos];
        if (m_readPos == m_writePos && m_delaySamples < 1) { out = input; } // Fractional delay case.
        
        // Get the previous delayed sample value.
        float outPrev = 0;
        if (m_readPos-1 > 0) { outPrev = m_buffer[m_readPos-1]; }
        else { outPrev = m_buffer[m_maxDelaySamples-1]; }
        
        // Calculate the fractional delay value.
        out = (m_delayFraction * outPrev) + ((1 - m_delayFraction) * out);
    }

    // Write the input to the delay buffer.
    m_buffer[m_writePos] = input + (m_feedback * out);
    
    // Set the new read/write positions.
    if (++m_writePos >= m_maxDelaySamples) { m_writePos = 0; }
    if (++m_readPos >= m_maxDelaySamples) { m_readPos = 0; }

    return (m_mix * out) + ((1.0 - m_mix) * input); 
}

void DelayLine::setReadPos()
{
    // Calculate the number of delay samples and the fractional delay.
    float samples = m_sampleFreq*1e-3*m_delay;
    m_delaySamples = floor(samples);
    m_delayFraction = samples - m_delaySamples;
    
    // Calculate the read position.
    m_readPos = m_writePos - m_delaySamples;
    if (m_readPos < 0) { m_readPos += m_maxDelaySamples; }
}
