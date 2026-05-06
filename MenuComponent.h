#pragma once

#include <JuceHeader.h>
#include <vector>
#include <iostream>
#include <string>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

class MenuComponent : public   juce::Component,
                                    juce::Button::Listener,
                                    juce::Timer
          
{
public:
    MenuComponent(DJAudioPlayer* _player, WaveformDisplay* _waveformDisplay);
    ~MenuComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button*) override;
    void timerCallback() override;

    std::vector<juce::String> songTitles;
    std::vector<std::string> songPaths;
    juce::String prevSong = "No Song";
    juce::String nextSong = "No Song";
    juce::String currentSong = "No Song";

    // for song list
    int songCounter = 0;

    // for title update
    void titleUpdate();

    // to load playlist
    void loadPlaylist(std::vector<juce::String> songList, std::vector<std::string> pathList);

    // to update song duration
    void updateTimer(double time);
    static juce::String convertSecondsToTimer(double time);

    // song ending
    void songEnds();

private:
    // buttons
    juce::TextButton nextBtn{ "NEXT SONG" };
    juce::TextButton previousBtn{ "PREV SONG" };
    juce::TextButton shuffleBtn{ "SHUFFLE" };
    juce::TextButton repeatBtn{ "REPEAT" };
    
    // titles
    juce::Label prevTitle;
    juce::Label nextTitle;
    juce::Label currentSongNameTitle = currentSong;
    juce::Label timerTitle;

    DJAudioPlayer* menu;
    WaveformDisplay* waveformDisplay;

    // functions
    void changeToNextSong();
    void changeToPrevSong();

    void shuffleList();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};
