#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "MenuComponent.h"

class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& _formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, PlaylistComponent* _playlist);
    ~DeckGUI();

    void paint(juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button*) override;

    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;

private:
    // Buttons
    juce::TextButton playstopBtn{ "STOP" };
    juce::TextButton loadBtn{ "LOAD SONG" };
    juce::TextButton playQueuedBtn{ "PLAY QUEUE" };
    juce::TextButton muteBtn{ "MUTE" };
    juce::TextButton oneSpeedBtn{ "1X" };
    juce::TextButton twoSpeedBtn{ "2X" };
    juce::TextButton loadPlaylistBtn{ "LOAD PLAYLIST" };

    // Sliders
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    // Menu, Playlist and Waveform
    DJAudioPlayer* menu;
    PlaylistComponent* playlist;
    WaveformDisplay waveformDisplay;

    MenuComponent MenuComponent{ menu, &waveformDisplay };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
