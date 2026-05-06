#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse) 
                                : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    // Add change listener to respond when the waveform changes
    audioThumb.addChangeListener(this);
}

// Destructor
WaveformDisplay::~WaveformDisplay()
{
}

// ?? Paints the waveform display
void WaveformDisplay::paint (juce::Graphics& g)
{
    // Set the background color for the waveform display
    g.fillAll(juce::Colour(176, 224, 230)); 
    // Draw a border around the waveform
    g.setColour (juce::Colours::navy);
    g.drawRect (getLocalBounds(), 1);   
    // Set the waveform color
    g.setColour (juce::Colour(25, 25, 112));

    // If a file is loaded, draw the waveform
    if (fileLoaded)
    {
        // Draw the audio waveform
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        // Draw a moving playhead (progress indicator)
        g.setColour(juce::Colours::skyblue);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else
    {
        // Display a message if no file is loaded
        g.setFont(20.0f);
        g.drawText("No file loaded...", getLocalBounds(), juce::Justification::centred, true);
    }

}

// Handles component resizing (empty for now, but useful for future adjustments)
void WaveformDisplay::resized()
{

}

// ?? Loads an audio file from a given URL and updates the waveform display
void WaveformDisplay::loadURL(juce::URL audioURL)
{
    // Clear any previously loaded waveform data
    audioThumb.clear();
    // Load the new audio file
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    // If successful, update the display
    if (fileLoaded)
    { 
        repaint();// Redraw the waveform
    }
    else 
    {
        DBG("Not loaded");// Debugging message if file fails to load
    }
}

// ?? Callback function triggered when the audio thumbnail changes
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();// Redraw the waveform to reflect the changes
}

// ?? Updates the playhead position based on the song's progress
void WaveformDisplay::setPositionRelative(double pos)
{
    // Only update if the position actually changed
    if (position != pos)
    {
        position = pos;
        repaint();// Redraw the waveform with the new position
    }

}