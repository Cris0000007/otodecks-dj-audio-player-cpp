    #include <JuceHeader.h>
    #include "DeckGUI.h"
    #include "MainComponent.h"

    DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& _formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, PlaylistComponent* _playlist)
        : menu(_player), waveformDisplay(_formatManagerToUse, cacheToUse), playlist(_playlist)
    {
        // Styling the cutie buttons ~
        
        // Play / Stop button styling
        playstopBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue); 
        playstopBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(playstopBtn);

        // Load button styling
        loadBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        loadBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(loadBtn);

        //  Play queue button styling
        playQueuedBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        playQueuedBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(playQueuedBtn);

        // Load playlist button styling
        loadPlaylistBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        loadPlaylistBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(loadPlaylistBtn);

        // Sliders initialization (Volume, Speed, and Position)

        // Volume slider styling
        volSlider.setColour(juce::Slider::thumbColourId, juce::Colour(173,216,230));
        addAndMakeVisible(volSlider);

        // Speed slider styling
        speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(173, 216, 230));
        addAndMakeVisible(speedSlider);

        // Position slider styling
        posSlider.setColour(juce::Slider::thumbColourId, juce::Colour(173, 216, 230));
        addAndMakeVisible(posSlider);

        // Mute button styling
        muteBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        muteBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(muteBtn);

        // 1x Speed button styling
        oneSpeedBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        oneSpeedBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(oneSpeedBtn);

        // 2x Speed button styling
        twoSpeedBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        twoSpeedBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(twoSpeedBtn);

        //Adding UI Components to the display
        // menu
        addAndMakeVisible(MenuComponent);
        // waveform
        addAndMakeVisible(waveformDisplay);

        // Adding Listeners to Buttons and Sliders
        // listeners
        playstopBtn.addListener(this);
        loadBtn.addListener(this);
        volSlider.addListener(this);
        speedSlider.addListener(this);
        posSlider.addListener(this);
        playQueuedBtn.addListener(this);
        loadPlaylistBtn.addListener(this);
        muteBtn.addListener(this);
        oneSpeedBtn.addListener(this);
        twoSpeedBtn.addListener(this);

        // Setting slider ranges
        posSlider.setRange(0.0, 1.0);
        speedSlider.setRange(0.05, 2, 0.05);
        volSlider.setRange(0, 1, 0.01);

        // update waveform display position
        startTimer(500);

        // Setting initial default values for sliders
        volSlider.setValue(1);
        speedSlider.setValue(1);

        // Toggle buttons for Mute, Speed x1, and Speed x2
        muteBtn.setClickingTogglesState(true);
        muteBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::skyblue);
        oneSpeedBtn.setClickingTogglesState(true);
        oneSpeedBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::skyblue);
        twoSpeedBtn.setClickingTogglesState(true);
        twoSpeedBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::skyblue);
    }
    /**
    * Destructor - Stops the timer before destroying DeckGUI.
    */
    DeckGUI::~DeckGUI()
    {
        stopTimer();
    }

    void DeckGUI::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colour(224, 255, 255)); //  background
    }

    void DeckGUI::resized()
    {
        // Settings of rowH and columnH
        double rowH = getHeight() / 20;
        double columnW = getWidth() / 16;

        // Set position

        // button
        playstopBtn.setBounds(columnW * 6.3, rowH * 13.5, columnW * 3.5, rowH * 2.5);
        playQueuedBtn.setBounds(columnW * 6.3, rowH * 16, columnW * 3.5, rowH * 1.3);
        muteBtn.setBounds(columnW * 0.73, rowH * 17, columnW * 2.5, rowH);
        oneSpeedBtn.setBounds(columnW * 12.8, rowH * 17, columnW * 1.2, rowH);
        twoSpeedBtn.setBounds(columnW * 14, rowH * 17, columnW * 1.2, rowH);
        loadBtn.setBounds(0, rowH * 19, columnW * 8, rowH * 1.4);
        loadPlaylistBtn.setBounds(columnW * 8, rowH * 19, columnW * 8, rowH * 1.4);

        // slider
        volSlider.setBounds(0, rowH * 13, columnW * 4, rowH * 4);
        volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, columnW * 4, rowH * 0.75);
        speedSlider.setBounds(columnW * 12, rowH * 13, columnW * 4, rowH * 4);
        speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, columnW * 4, rowH * 0.75);
        posSlider.setBounds(columnW * 0.1, rowH * 12, columnW * 15.9, rowH);
        posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, columnW * 4, rowH * 0.75);

        // song display
        MenuComponent.setBounds(0, 0, getWidth(), rowH * 10);

        // waveform display
        waveformDisplay.setBounds(columnW * 0.1, rowH * 9, columnW * 15.8, rowH * 3);
    }
    
    // play/stop button controller
    void DeckGUI::buttonClicked(juce::Button* button)
    {
        if (button == &playstopBtn)
        {
            if (playstopBtn.getButtonText() == "Play")
            {
                playstopBtn.setButtonText("Stop");
                menu->start();
            }
            else
            {
                playstopBtn.setButtonText("Play");
                menu->stop();
            }
        }

        if (button == &loadBtn)
        {
            juce::FileChooser chooser{ "Select a file~" };

            // display folder
            if (chooser.browseForFileToOpen())
            {
                // load into menu display and waveform display
                menu->loadURL(juce::URL{ chooser.getResult() });
                waveformDisplay.loadURL(juce::URL{ chooser.getResult() });

                // update
                MenuComponent.currentSong = chooser.getResult().getFileName();
                MenuComponent.nextSong = "No Song~";
                MenuComponent.prevSong = "No Song~";
                MenuComponent.titleUpdate();

                // clear song titles and paths
                MenuComponent.songTitles.clear();
                MenuComponent.songPaths.clear();

            }

        }

        if (button == &playQueuedBtn)
        {
            // check if any songs are selected
            if (playlist->selectedSong != "Nothing Selected!!")
            {
                // load into menu display and waveform display
                menu->loadURL(juce::URL{ juce::File::File(playlist->selectedSongPath) });
                waveformDisplay.loadURL(juce::URL{ juce::File::File(playlist->selectedSongPath) });

                // update
                MenuComponent.currentSong = playlist->selectedSong;
                MenuComponent.nextSong = "No Song~";
                MenuComponent.prevSong = "No Song~";
                MenuComponent.titleUpdate();

                // clear song titles and paths
                MenuComponent.songTitles.clear();
                MenuComponent.songPaths.clear();
            }
        }

        if (button == &loadPlaylistBtn)
        {
            // if song exist, continue
            if (playlist->songTitles.size() > 0)
            {
                MenuComponent.loadPlaylist(playlist->songTitles, playlist->SongPaths);
            }

        }

        // volume control
        if (button == &muteBtn)
        {
            // check if it is toggled
            if (muteBtn.getToggleState())
            {
                // set volume to 0
                menu->setGain(0);
            }
            else
            {
                // set to current value of slider
                menu->setGain(volSlider.getValue());
            }
        }

        //  control speed
        if (button == &oneSpeedBtn)
        {
            // check if 1x speed is toggled
            if (oneSpeedBtn.getToggleState())
            {
                // set speed to 1x 
                menu->setSpeed(1);
                twoSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification); // ensure only one speed button is toggled
            }
            else
            {
                // if oneSpeedBtn is toggled off, set speed according to the slider value
                menu->setSpeed(speedSlider.getValue());
            }
        }

        if (button == &twoSpeedBtn)
        {
            // check if 2x speed is toggled
            if (twoSpeedBtn.getToggleState())
            {
                // set speed to 2x
                menu->setSpeed(2);
                oneSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
            }
            else
            {
                // if twoSpeedBtn is toggled off, set speed according to the slider value
                menu->setSpeed(speedSlider.getValue());
            }
        }
    }

    // slider functionalities
    void DeckGUI::sliderValueChanged(juce::Slider* slider)
    {
        // changes volume
        if (slider == &volSlider && !muteBtn.getToggleState())
        {
            menu->setGain(slider->getValue());
        }

        // changes the speed if 1x is 'on'
        if (slider == &speedSlider && oneSpeedBtn.getToggleState())
        {
            speedSlider.setValue(1);
            menu->setSpeed(1.0); // or whatever value corresponds to 1x speed
        }

        // changes the speed if 2x is 'off'
        if (slider == &speedSlider && twoSpeedBtn.getToggleState())
        {
            speedSlider.setValue(2);
            menu->setSpeed(2.0);
        }

        // changes position of waveform slider
        if (slider == &posSlider)
        {
            menu->setPositionRelative(slider->getValue());
        }
    }

    void DeckGUI::timerCallback()
    {
        // update waveform position
        waveformDisplay.setPositionRelative(menu->getPositionRelative());
        posSlider.setValue(menu->getPositionRelative());

        // update timer display
        MenuComponent.updateTimer(menu->sendTimer());
    }