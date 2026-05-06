# OtoDecks DJ Audio Player

A C++ DJ audio player application developed using the JUCE framework. This project demonstrates audio playback control, playlist management, waveform display, and basic DJ deck functionality.

## Project Overview

OtoDecks is a desktop-based audio player application built with C++ and JUCE. The application allows users to load audio files, control playback, adjust playback settings, manage playlists, and view waveform displays.

This project focuses on applying Object-Oriented Programming concepts, GUI development, event handling, and audio application development.

## Tech Stack

- C++
- JUCE Framework
- Object-Oriented Programming
- GUI Application Development
- Audio Playback

## Key Features

- Load audio files into the application
- Play, pause, and stop audio tracks
- Adjust volume and playback speed
- Display audio waveform visualization
- Manage tracks through a playlist
- Use deck-style controls for DJ-style audio playback

## Project Structure

```text
otodecks-dj-audio-player-cpp/
├── DJ App.jucer
├── Source/
│   ├── Main.cpp
│   ├── MainComponent.cpp
│   ├── MainComponent.h
│   ├── DJAudioPlayer.cpp
│   ├── DJAudioPlayer.h
│   ├── DeckGUI.cpp
│   ├── DeckGUI.h
│   ├── PlaylistComponent.cpp
│   ├── PlaylistComponent.h
│   ├── WaveformDisplay.cpp
│   ├── WaveformDisplay.h
│   ├── MenuComponent.cpp
│   ├── MenuComponent.h
│   ├── ManagerComponent.cpp
│   └── ManagerComponent.h
└── README.md
```

## Main Components

- `DJAudioPlayer`  
  Handles audio loading, playback, volume control, speed control, and playback position control.

- `DeckGUI`  
  Provides the user interface for each audio deck, including buttons, sliders, and waveform-related controls.

- `PlaylistComponent`  
  Manages the track list and allows users to organise and load audio files.

- `WaveformDisplay`  
  Displays the waveform of the loaded audio track.

- `MenuComponent`  
  Supports menu-related interface functions within the application.

- `ManagerComponent`  
  Helps manage the layout and coordination of application components.

- `MainComponent`  
  Combines the main components and controls the overall application interface.

## How to Run

This project requires the JUCE framework and a compatible C++ development environment.

1. Install JUCE and Projucer.
2. Open `DJ App.jucer` using Projucer.
3. Set the JUCE modules path if required.
4. Save and export the project to Visual Studio or Xcode.
5. Build and run the project from the IDE.
6. Load an audio file to test the playback functions.

## What I Learned

Through this project, I practised:

- Building a desktop application using C++
- Applying Object-Oriented Programming concepts
- Using the JUCE framework for audio and GUI development
- Handling user interface events
- Managing audio playback and playlist functions
- Creating waveform display components
- Debugging and testing application features
- Organising source files in a structured C++ project

## Possible Improvements

- Improve the user interface design
- Add drag-and-drop support for audio files
- Add more audio effects
- Improve playlist search and sorting
- Add track duration and metadata display
- Save and reload playlist history

## Note

This project was developed as part of my academic programming practice and portfolio.
