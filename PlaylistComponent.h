#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "ManagerComponent.h"
#include "MenuComponent.h"


class PlaylistComponent  :  public juce::Component,
                                   juce::TableListBoxModel,
                                   juce::Button::Listener
{
    public:

        PlaylistComponent();
        ~PlaylistComponent();

        void paint (juce::Graphics&) override;
        void resized() override;

        int getNumRows() override;
        void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
        void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

        juce::Component* refreshComponentForCell(int rowNumber, int columnID, bool isRowSelected, juce::Component* existingComponentToUpdate) override;

        void buttonClicked(juce::Button* button) override;

        juce::String selectedSongPath;
        juce::String selectedSong = "No Song";

        void updatePlaylist();

        // converter
        juce::String convertSongPathToTitle(std::string path);

        std::vector<juce::String> songTitles;
        std::vector<std::string> SongPaths;

    private:

        juce::TableListBox tableComponent;

        // queued song
        juce::Label queuedName;
        juce::Label queuedTitle;

        // from loaded playlist
        juce::TextButton addSongBtn{ "ADD NEW SONG" };
        juce::TextButton removeSongBtn{ "REMOVE SONG" };

        // search song
        juce::TextEditor searchBar{ "Search song" };
        juce::TextButton searchBtn{ "Search" };

        juce::AudioFormatManager formatManager;

        // converts song path to song title for readability
        void populateSongTitles();

        // search for songs
        void searchSongs(juce::String input);

        // search functions
        int rowCounter = -1;
        juce::String lastSearch = "";

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
