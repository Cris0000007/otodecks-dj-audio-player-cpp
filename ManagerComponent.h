#pragma once

#include "JuceHeader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>



class ManagerComponent
{
	public:
		ManagerComponent();

		static std::vector<std::string> returnSongPathsArray();

		// add to file
		static void addNewSong(juce::String path);
		
		// remove to file
		static void removeSong(int rowNumber);

	private:
		// read to file
		static std::vector<std::string> readSongs();
};