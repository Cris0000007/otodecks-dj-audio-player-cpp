#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
                             : formatManager(_formatManager)
{
}
/**
 * Destructor: Cleans up any allocated resources.
 */
DJAudioPlayer::~DJAudioPlayer()
{
}
/**
 * Prepares the audio player to start playing audio.
 */
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
/**
 * Retrieves the next audio block for playback.
 */
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) 
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
/**
 * Releases allocated audio resources when playback stops.
 */
void DJAudioPlayer::releaseResources() 
{
    transportSource.releaseResources(); 
    resampleSource.releaseResources();
}

/**
 * Loads an audio file from a given URL into the transport source.
 */
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        transportSource.start();
    }
    else
    {
        DBG("File error!");
    }
}

/**
 * Sets the playback volume.
 */
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("Gain should be between 0 to 1");
    }
    else
    {
        transportSource.setGain(gain);
    }

}

/**
 * Sets the playback speed.
 */
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("Speed ratio should be between 0 to 100'");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}
/**
 * Sets the playback position in seconds.
 */
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}
/**
 * Sets the playback position relative to the track's total length (0.0 to 1.0).
 */
void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("Position should be between 0 to 1");
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

/**
 * Starts audio playback.
 */
void DJAudioPlayer::start()
{
    transportSource.start();
}

/**
 * Stops audio playback.
 */
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

/**
 * Returns the current playback position as a percentage of the track's total length.
 */
double DJAudioPlayer::getPositionRelative()
{
    double pos = transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();

    if (isnan(pos)) {
        return 0;
    }
    return pos;
}

/**
 * Returns the current playback time in seconds.
 */
double DJAudioPlayer::sendTimer()
{
    return transportSource.getCurrentPosition();
}

/**
 * Checks if the currently loaded song has finished playing.
 */
bool DJAudioPlayer::isSongFinished()
{
    return transportSource.hasStreamFinished();
}