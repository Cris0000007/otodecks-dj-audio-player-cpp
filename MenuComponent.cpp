    #include <JuceHeader.h>
    #include "MenuComponent.h"
    #include "random"

    MenuComponent::MenuComponent(DJAudioPlayer* _player, WaveformDisplay* _waveformDisplay)
                    : menu(_player), waveformDisplay(_waveformDisplay)
    {
        // ?? Setting up the UI buttons with colors and adding them to the display
        // Next song button
        nextBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        nextBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(nextBtn);

        // Previous song button
        previousBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        previousBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(previousBtn);
        
        // Shuffle button (randomizes song order)
        shuffleBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        shuffleBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(shuffleBtn);

        // Repeat button (toggles song repeat)
        repeatBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::skyblue);
        repeatBtn.setColour(juce::TextButton::textColourOffId, juce::Colours::navy);
        addAndMakeVisible(repeatBtn);

        // Labels to display song information
        
        // Previous song label
        prevTitle.setColour(juce::Label::textColourId, juce::Colours::navy);
        addAndMakeVisible(prevTitle);

        // Next song label
        nextTitle.setColour(juce::Label::textColourId, juce::Colours::navy);
        addAndMakeVisible(nextTitle);

        // Current song label
        currentSongNameTitle.setColour(juce::Label::textColourId, juce::Colours::navy);
        addAndMakeVisible(currentSongNameTitle);

        // Timer label (displays song duration)
        timerTitle.setColour(juce::Label::textColourId, juce::Colours::navy);
        addAndMakeVisible(timerTitle);

        // ?? Making the repeat button toggleable (ON/OFF state)
        repeatBtn.setClickingTogglesState(true);
        repeatBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(230, 230, 250));
        repeatBtn.setColour(juce::TextButton::textColourOnId, juce::Colours::black);


        // ?? Adding event listeners for button clicks
        nextBtn.addListener(this);
        previousBtn.addListener(this);
        shuffleBtn.addListener(this);

        startTimer(1);// ?? Start a timer that checks if a song has ended
    }

    // Destructor
    MenuComponent::~MenuComponent()
    {
        menu->releaseResources();
        stopTimer();
    }
    // ?? Paint function (draws the UI background and layout)
    void MenuComponent::paint (juce::Graphics& g)
    {
        // Setting background color
        g.fillAll(juce::Colour(224, 255, 255)); 

        // Drawing a border for the menu
        g.setColour(juce::Colours::navy);
        juce::Rectangle<float> prepareBorder(getWidth() * 0.01, getHeight() * 0.05, getWidth() * 0.98, getHeight() * 0.84);
        g.fillRect(prepareBorder);

        // Setting menu area color
        g.setColour(juce::Colours::skyblue);
        juce::Rectangle<float> prepareArea(getWidth() * 0.02, getHeight() * 0.07, getWidth() * 0.96, getHeight() * 0.8);
        g.fillRect(prepareArea);
    }

    // ?? Layout positioning of UI elements
    void MenuComponent::resized()
    {
        // specific settings of rowH and columnH
        double rowH = getHeight() / 20;
        double columnW = getWidth() / 16;

        // Positioning buttons
        nextBtn.setBounds(columnW * 10.5, rowH * 11, columnW * 4.5, rowH * 3);
        previousBtn.setBounds(columnW * 1.1, rowH * 11, columnW * 4.5, rowH * 3);
        shuffleBtn.setBounds(columnW * 6.8, rowH * 12.5, columnW * 2.7, rowH * 2);
        repeatBtn.setBounds(columnW * 6.8, rowH * 15, columnW * 2.7, rowH * 2);

        // Positioning song labels
        prevTitle.setColour(1, juce::Colours::navy);
        prevTitle.setFont(14.0f);
        prevTitle.setText(prevSong, juce::dontSendNotification);
        prevTitle.setJustificationType(juce::Justification::centred);
        prevTitle.setBounds(getWidth() * 0.06, getHeight() * 0.7, getWidth() * 0.3, getHeight() * 0.1);
        nextTitle.setColour(1, juce::Colours::navy);
        nextTitle.setFont(14.0f);
        nextTitle.setText(nextSong, juce::dontSendNotification);
        nextTitle.setJustificationType(juce::Justification::centred);
        nextTitle.setBounds(getWidth() * 0.64, getHeight() * 0.7, getWidth() * 0.3, getHeight() * 0.1);
        currentSongNameTitle.setColour(1, juce::Colours::navy);
        currentSongNameTitle.setFont(28.0f);
        currentSongNameTitle.setText(currentSong, juce::dontSendNotification);
        currentSongNameTitle.setJustificationType(juce::Justification::centred);
        currentSongNameTitle.setBounds(getWidth() * 0.1, getHeight() * 0.1, getWidth() * 0.8, getHeight() * 0.2);
        timerTitle.setColour(1, juce::Colours::navy);
        timerTitle.setFont(28.0f);
        timerTitle.setText("0:00", juce::dontSendNotification);
        timerTitle.setJustificationType(juce::Justification::centred);
        timerTitle.setBounds(getWidth() * 0.1, getHeight() * 0.3, getWidth() * 0.8, getHeight() * 0.2);
    }

    // ?? Update song titles
    void MenuComponent::titleUpdate()
    {
        nextTitle.setText(nextSong, juce::dontSendNotification);
        currentSongNameTitle.setText(currentSong, juce::dontSendNotification);
        prevTitle.setText(prevSong, juce::dontSendNotification);
    }

    // ?? Load playlist into the menu
    void MenuComponent::loadPlaylist(std::vector<juce::String> songList, std::vector<std::string> pathList)
    {
        songTitles = songList;
        songPaths = pathList;

        // Reset
        songCounter = 0;

        if (currentSong == "No Song")
        {
            // Load all respective songs
            prevSong = songTitles.at(songTitles.size() - 1);
            nextSong = songTitles.at(songCounter + 1);
            currentSong = songTitles.at(songCounter);

            // Load the first song into the player and waveform
            menu->loadURL(juce::URL{ juce::File::File(songPaths.at(0)) });
            waveformDisplay->loadURL(juce::URL{ juce::File::File(songPaths.at(0)) });
            menu->stop();
        }
        else
        {
            songCounter = -1;
            prevSong = songTitles.at(songTitles.size() - 1);
            nextSong = songTitles.at(0);
        }

        titleUpdate();

    }

    void MenuComponent::changeToNextSong()
    {
        if (songTitles.size() >= 3)
        {
            if (songCounter == 0)
            {
                // updates
                prevSong = songTitles.at(0);
                nextSong = songTitles.at(2);
                currentSong = songTitles.at(1);

                // updates counter
                songCounter = 1;
            }
            else if (songCounter == songTitles.size() - 1)
            {
                prevSong = songTitles.at(songTitles.size() - 1);
                nextSong = songTitles.at(1);
                currentSong = songTitles.at(0);

                songCounter = 0;
            }
            else if (songCounter == songTitles.size() - 2)
            {
                prevSong = songTitles.at(songTitles.size() - 2);
                nextSong = songTitles.at(0);
                currentSong = songTitles.at(songTitles.size() - 1);

                songCounter++;
            }
            else if (songCounter == -1)
            {
                prevSong = songTitles.at(songTitles.size() - 1);
                nextSong = songTitles.at(1);
                currentSong = songTitles.at(0);

                songCounter = 0;
            }
            else
            {
                prevSong = songTitles.at(songCounter);
                nextSong = songTitles.at(songCounter + 2);
                currentSong = songTitles.at(songCounter + 1);
                songCounter++;
            }
        }
        else if (songTitles.size() == 2)
        {
            if (songCounter == 0)
            {
                prevSong = songTitles.at(0);
                nextSong = songTitles.at(0);
                currentSong = songTitles.at(1);

                songCounter = 1;
            }
            else if (songCounter == -1)
            {
                prevSong = songTitles.at(1);
                nextSong = songTitles.at(1);
                currentSong = songTitles.at(0);

                songCounter = 0;
            }
            else
            {
                prevSong = songTitles.at(1);
                nextSong = songTitles.at(1);
                currentSong = songTitles.at(0);

                songCounter = 0;
            }
        }
        else if (songTitles.size() == 1)
        {

            prevSong = "No Song";
            nextSong = "No Song";
            currentSong = songTitles.at(0);
        }
        else if (songTitles.size() == 0)
        {
            return;
        }
        titleUpdate();

        // load and start track
        menu->loadURL(juce::URL{ juce::File::File(songPaths.at(songCounter)) });
        waveformDisplay->loadURL(juce::URL{ juce::File::File(songPaths.at(songCounter)) });
    }

    void MenuComponent::changeToPrevSong()
    {
        if (songTitles.size() >= 3)
        {
            if (songCounter == 0)
            {
                prevSong = songTitles.at(songTitles.size() - 2);
                nextSong = songTitles.at(0);
                currentSong = songTitles.at(songTitles.size() - 1);

                songCounter = songTitles.size() - 1;
            }
            else if (songCounter == 1)
            {
                prevSong = songTitles.at(songTitles.size() - 1);
                nextSong = songTitles.at(1);
                currentSong = songTitles.at(0);

                songCounter = 0;
            }
            else if (songCounter == songTitles.size() - 1)
            {
                prevSong = songTitles.at(songTitles.size() - 3);
                nextSong = songTitles.at(songTitles.size() - 1);
                currentSong = songTitles.at(songTitles.size() - 2);

                songCounter--;
            }
            else if (songCounter == -1)
            {
                prevSong = songTitles.at(songTitles.size() - 2);
                nextSong = songTitles.at(0);
                currentSong = songTitles.at(songTitles.size() - 1);

                songCounter = songTitles.size() - 1;
            }
            else
            {
                prevSong = songTitles.at(songCounter - 2);
                nextSong = songTitles.at(songCounter);
                currentSong = songTitles.at(songCounter - 1);

                songCounter--;
            }
        }
        else if (songTitles.size() == 2)
        {
            if (songCounter == 0)
            {
                prevSong = songTitles.at(0);
                nextSong = songTitles.at(0);
                currentSong = songTitles.at(1);

                songCounter = 1;
            }
            else if (songCounter == -1)
            {
                prevSong = songTitles.at(0);
                nextSong = songTitles.at(0);
                currentSong = songTitles.at(1);

                songCounter = 1;
            }
            else
            {
                prevSong = songTitles.at(1);
                nextSong = songTitles.at(1);
                currentSong = songTitles.at(0);

                songCounter = 0;
            }
        }
        else if (songTitles.size() == 1)
        {
            prevSong = "No Song";
            nextSong = "No Song";
            currentSong = songTitles.at(0);
        }
        else if (songTitles.size() == 0)
        {
            return;
        }
        titleUpdate();
        menu->loadURL(juce::URL{ juce::File::File(songPaths.at(songCounter)) });
        waveformDisplay->loadURL(juce::URL{ juce::File::File(songPaths.at(songCounter)) });
    }

    void MenuComponent::updateTimer(double time)
    {
        juce::String timeAsString = convertSecondsToTimer(time);
        timerTitle.setText(timeAsString, juce::NotificationType::dontSendNotification);
    }

    // time formatting
    juce::String MenuComponent::convertSecondsToTimer(double time)
    {
        int sampleTime = floor(time);
        // labelling time
        int hours = sampleTime / 3600;
        int minutes = (sampleTime % 3600) / 60;
        int seconds = sampleTime % 60;

        juce::String formattedTime;

        if (hours != 0)
        {
            juce::String hoursInString(hours);
            if (hoursInString.length() == 1)
            {
                hoursInString = "0" + hoursInString;
            }
            juce::String minutesInString(minutes);
            if (minutesInString.length() == 1)
            {
                minutesInString = "0" + minutesInString;
            }
            juce::String secondsInString(seconds);
            if (secondsInString.length() == 1)
            {
                secondsInString = "0" + secondsInString;
            }
            formattedTime = hoursInString + ':' + minutesInString + ':' + secondsInString ;
            return formattedTime;
        }
        else
        {
            juce::String minutesInString(minutes);
            if (minutesInString.length() == 1)
            {
                minutesInString = "0" + minutesInString;
            }
            juce::String secondsInString(seconds);
            if (secondsInString.length() == 1)
            {
                secondsInString = "0" + secondsInString;
            }
            formattedTime = minutesInString + ':' + secondsInString;
            return formattedTime;
        }
    }
    // ?? Shuffle the playlist randomly
    void MenuComponent::shuffleList()
    {
        // shuffle
        std::random_device rd;
        std::mt19937 shuffler(rd());
        std::shuffle(songPaths.begin(), songPaths.end(), shuffler);
        songTitles.clear();

        juce::File temporaryPath;

        for (std::string& t : songPaths)
        {
            temporaryPath = juce::File(t);
            songTitles.push_back(temporaryPath.getFileNameWithoutExtension());
        }
        // Reload shuffled playlist
        loadPlaylist(songTitles, songPaths);
    }

    // ? Play next song when the current song ends
    void MenuComponent::songEnds()
    {
        if (!repeatBtn.getToggleState())
        {
            changeToNextSong();
        }
        else
        {
            // repeat the song
            menu->setPositionRelative(0.00);
            menu->start();

        }
    }

    // ?? Handling button clicks (Next, Previous, Shuffle)
    void MenuComponent::buttonClicked(juce::Button* button)
    {
        if (button == &nextBtn)
        {
            changeToNextSong();
        }
        if (button == &previousBtn)
        {
            changeToPrevSong();
        }
        if (button == &shuffleBtn)
        {
            if (songTitles.size() > 0)
            {
                shuffleList();
            }
        }
    }

    // ? Timer callback: Checks if the song has ended
    void MenuComponent::timerCallback()
    {
        if (currentSong != "No Song")
        {
            if (menu->isSongFinished())
            {
                songEnds();
            }
        }
    }