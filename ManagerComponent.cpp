#include "ManagerComponent.h"

ManagerComponent::ManagerComponent()
{
    readSongs();// Load stored songs from the CSV file
}

/**
 * Returns the list of song file paths stored in the playlist.
 */
std::vector<std::string> ManagerComponent::returnSongPathsArray()
{
    std::vector<std::string> SongPaths = readSongs();
    return SongPaths;
}

/**
 * Reads stored song paths from the Songs.csv file.
 */
std::vector<std::string> ManagerComponent::readSongs()
{
    std::string line;
    std::vector<std::string> songs;
    juce::String filePath = juce::File::getCurrentWorkingDirectory().getFullPathName();
    filePath.toStdString();
    juce::File songsFile(filePath + "\\Songs.csv");

    // Create Songs.csv file if it does not exist
    if (!songsFile.exists())
    {
        songsFile.create();
    }
    // Open and read CSV file
    juce::FileInputStream csvFile(songsFile);

    if (csvFile.openedOk())
    {
        while (!csvFile.isExhausted())
        {
            try {
                juce::String juceLine = csvFile.readNextLine();
                line = juceLine.toStdString();
                songs.push_back(line);
            }
            catch (const std::exception& e)
            {
                DBG("ManagerComponent::bad");
            }
        }
    }
    return songs;
}

/**
 * Adds a new song file path to the Songs.csv file.
 */
void ManagerComponent::addNewSong(juce::String path)
{
    std::string addingSong = path.toStdString() + "\n";
    std::ofstream csvFile(".\\Songs.csv", std::ios::app);
    csvFile << addingSong;
    csvFile.close();
}

/**
 * Removes a song from the playlist by its row index.
 */
void ManagerComponent::removeSong(int rowNumber)
{
    std::string line;
    int lineIndex = 0;
    std::ifstream oriFile;
    oriFile.open(".\\Songs.csv", std::ios::app);
    std::ofstream replaceFile;
    replaceFile.open(".\\Replacement.csv", std::ios::app);

    // Copy all lines except the one to be removed
    while (getline(oriFile, line))
    {
        if (lineIndex != rowNumber)
        {
            replaceFile << line << std::endl; 
        }
        // Increase line index
        lineIndex++;
    }

    // Close files and update playlist file
    replaceFile.close();
    oriFile.close();
    remove(".\\Songs.csv");// Delete the original file
    rename(".\\Replacement.csv", ".\\Songs.csv");// Rename temporary file to original
}
