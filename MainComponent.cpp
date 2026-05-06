#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);// Set default window size

    // Request runtime permissions for audio recording if required
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (0, 2);// Set up audio output with 2 channels
    }

    // Adding GUI components (Decks and Playlist)
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    // Adding playlist component for track management
    addAndMakeVisible(playlistComponent);

    // Register basic audio formats (MP3, WAV, etc.)
    formatManager.registerBasicFormats();

}
/**
 * Destructor: Cleans up and shuts down the audio system.
 */
MainComponent::~MainComponent()
{
    shutdownAudio();

}
/**
 * Prepare to play: Initializes audio sources with sample rate and buffer size.
 */
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

/**
 * Audio callback: Fetches the next block of audio data to be played.
 */
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{   
    mixerSource.getNextAudioBlock(bufferToFill);
}

/**
 * Releases resources when the component is no longer needed.
 */
void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}
/**
 * Paints the background color of the MainComponent.
 */
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(224, 255, 255)); // Light blue background
}

/**
 * Resizes and arranges UI components based on the window size.
 */
void MainComponent::resized()
{
    // Set positions and sizes for deck GUI components
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() * 0.66 );
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() * 0.66);
    
    // Set position for playlist component (bottom area)
    playlistComponent.setBounds(0, getHeight() * 0.66, getWidth(), getHeight() * 0.32);
}

/**
 * Determines whether DeckGUI should accept file drag and drop.
 */
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

/**
 * Handles files dropped into the DeckGUI area.
 */
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    if (files.size() == 1)
    {
        // Load dropped file into the player and waveform display
        menu->loadURL( juce::URL{ juce::File{files[0]} } );
        waveformDisplay.loadURL( juce::URL{ juce::File{files[0]} } );

        // Update playlist and menu component with new song details
        juce::String newSong = playlist->convertSongPathToTitle(juce::File{ files[0] }.getFullPathName().toStdString());
        MenuComponent.currentSong = newSong;
        MenuComponent.nextSong = "No Song";
        MenuComponent.prevSong = "No Song";
        MenuComponent.titleUpdate();
        MenuComponent.songTitles.clear();
        MenuComponent.songPaths.clear();
        
    }
}