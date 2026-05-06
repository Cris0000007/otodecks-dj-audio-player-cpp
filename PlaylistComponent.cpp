#include <JuceHeader.h>
#include "PlaylistComponent.h"


PlaylistComponent::PlaylistComponent()
{
    //Fetch stored song paths from ManagerComponent
    SongPaths = ManagerComponent::returnSongPathsArray();
    populateSongTitles();
    
    // ?? Setting up the song list table (UI)
    tableComponent.setColour(juce::ListBox::backgroundColourId, juce::Colour(176,224,230)); // table background
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colours::skyblue); // Set background color (header)
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::outlineColourId, juce::Colours::navy); // header border
    tableComponent.setColour(juce::ListBox::outlineColourId, juce::Colours::navy); // table border
    // Adding table columns (Song title, Duration, Add to queue)
    tableComponent.getHeader().addColumn("Song", 1, 200);
    tableComponent.getHeader().addColumn("Duration", 2, 200);
    tableComponent.getHeader().addColumn("Add songs", 3, 400);
    tableComponent.setModel(this);
    // Making UI components visible
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(queuedName);
    addAndMakeVisible(queuedTitle);

    // ?? Add Song button styling
    addSongBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
    addSongBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
    addAndMakeVisible(addSongBtn);

    // ?? Remove Song button styling
    removeSongBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
    removeSongBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
    addAndMakeVisible(removeSongBtn);

    // ?? Search bar styling
    searchBar.setColour(juce::TextEditor::textColourId, juce::Colour(0, 0, 128));
    searchBar.setColour(juce::TextEditor::backgroundColourId, juce::Colours::skyblue);
    searchBar.setColour(juce::TextEditor::outlineColourId, juce::Colours::navy);
    addAndMakeVisible(searchBar);

    // ?? Search button styling
    searchBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue); // Set button color
    searchBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy); // Set text color
    addAndMakeVisible(searchBtn);

    // Adding button event listeners
    addSongBtn.addListener(this);
    removeSongBtn.addListener(this);

    // Allows loading different file formats
    formatManager.registerBasicFormats();
}

// Destructor (cleanup)
PlaylistComponent::~PlaylistComponent()
{

}
// ?? Paint function (Draws the background and UI layout)
void PlaylistComponent::paint (juce::Graphics& g)
{
    // Background color
    g.fillAll(juce::Colour(224, 255, 255)); 

    // Creating a rectangular background for the queue section
    g.setColour(juce::Colour(135, 206, 235));
    juce::Rectangle<float> prepareArea(getWidth() * 0.7, getHeight() * 0.72, getWidth() * 0.28, getHeight() * 0.31);
    g.fillRect(prepareArea);

    // ?? Display the queued song name
    queuedName.setColour(juce::Label::textColourId, juce::Colours::navy);
    queuedName.setFont(20.0f);
    queuedName.setText(selectedSong, juce::dontSendNotification);
    queuedName.setJustificationType(juce::Justification::centred);
    queuedName.setBounds(getWidth() * 0.605, getHeight() * 0.85, getWidth() * 0.48, getHeight() * 0.1);

    // ?? Queue title header
    queuedTitle.setColour(juce::Label::textColourId, juce::Colours::navy);
    queuedTitle.setFont(14.0f);
    queuedTitle.setText("SONG IN QUEUE", juce::dontSendNotification);
    queuedTitle.setJustificationType(juce::Justification::centred);
    queuedTitle.setBounds(getWidth() * 0.65, getHeight() * 0.75, getWidth() * 0.39, getHeight() * 0.1);

    // Button & Search bar positions
    addSongBtn.setBounds(getWidth() * 0.5, getHeight() * 0.74, getWidth() * 0.19, getHeight() * 0.11);
    removeSongBtn.setBounds(getWidth() * 0.5, getHeight() * 0.87, getWidth() * 0.19, getHeight() * 0.11 );
    searchBar.setBounds(getWidth() * 0, getHeight() * 0.72, getWidth() * 0.24, getHeight() * 0.12);
    searchBtn.setBounds(getWidth() * 0.243, getHeight() * 0.72, getWidth() * 0.11, getHeight() * 0.12);

    searchBtn.addListener(this);
}

// ?? Resizing UI elements
void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight() * 0.67 );
    tableComponent.getHeader().setColumnWidth(1, getWidth() / 4);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 4);
    tableComponent.getHeader().setColumnWidth(3, getWidth() / 4);
}

// ?? Returns the number of rows in the song table
int PlaylistComponent::getNumRows()
{
    return songTitles.size();
}

// ?? Styles row background colors
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colour(224, 255, 255));
    }
    else
    {
        g.fillAll(juce::Colours::skyblue);
    }
}
// ?? Styles song titles & duration cells
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 1)
    {
        g.drawText(songTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }

    if (columnId == 2)
    {
        juce::File filepath = SongPaths[rowNumber]; // get file path
        int lengthInSeconds = 0;

        if (auto reader = formatManager.createReaderFor(filepath))
        {
            lengthInSeconds = reader->lengthInSamples / reader->sampleRate;

            
            delete reader;// Prevent memory leak
        }
        else
        {
            g.drawText("Unknown", 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }

        juce::String formattedTime = MenuComponent::convertSecondsToTimer(lengthInSeconds);

        g.drawText(formattedTime, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

// ?? Handles song selection & "Add to Queue" button
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnID, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
    if (columnID == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "Add to Queue" };
            juce::String id(std::to_string(rowNumber));
            btn->setComponentID(id);
            btn->setBounds(0, 0, 100, 22);
            btn->setColour(juce::TextButton::buttonColourId, juce::Colours::white);
            btn->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}


// ?? Populates song titles from song paths
void PlaylistComponent::populateSongTitles()
{
    juce::String title;

    for (std::string& s : SongPaths)
    {
        title = convertSongPathToTitle(s);
        songTitles.push_back(title);
    }
}

// ?? Converts song path to its title
juce::String PlaylistComponent::convertSongPathToTitle(std::string path)
{
    juce::File juceFilePath(path);
    juce::String songTitle = juceFilePath.getFileNameWithoutExtension();
    return songTitle;
}

// ?? Updates the playlist UI
void PlaylistComponent::updatePlaylist()
{
    // cleanse
    SongPaths = ManagerComponent::returnSongPathsArray();
    songTitles.clear();
    populateSongTitles();
    tableComponent.updateContent();
}

// ?? Searches for songs based on input
void PlaylistComponent::searchSongs(juce::String input)
{
    // check for latest search
    if (input != lastSearch)
    {
        rowCounter = -1;
    }

    lastSearch = input;

    // searching
    for (int i = rowCounter + 1; i < songTitles.size(); ++i)
    {
        if (songTitles[i].containsIgnoreCase(input))
        {
            tableComponent.selectRow(i, false, true);
            rowCounter = i;            
            return;
        }

    }
    // resets counter if search is invalid
    rowCounter = -1;
}

// ?? Handles button clicks (Add/Remove/Search)
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button->getButtonText() == "Add to Queue")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        selectedSongPath = SongPaths[id];
        selectedSong = convertSongPathToTitle(selectedSongPath.toStdString());
        queuedName.setText("SELECTED SONG : " + selectedSong, juce::dontSendNotification);
    }

    if (button == &addSongBtn)
    {
        // Add song
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            ManagerComponent::addNewSong(chooser.getResult().getFullPathName());
            updatePlaylist();
        }
    }

    if (button == &removeSongBtn)
    {
        // Remove song
        int songRowNo = tableComponent.getSelectedRow();
        ManagerComponent::removeSong(songRowNo);
        updatePlaylist();
    }

    if (button == &searchBtn)
    {
        // Search songs
        searchSongs(searchBar.getText());
    }
}

