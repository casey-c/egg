#include "settingsmanager.h"

/////////////////
/// Singleton ///
/////////////////

/*
 * Returns the singleton if it exists; otherwise, construct it and return.
 */
SettingsManager& SettingsManager::getInstance()
{
    static SettingsManager instance;
    return instance;
}

/*
 * Private constructor will try and load the settings.ini file
 */
SettingsManager::SettingsManager()
{
    loadSettingsFromFile();
}

//////////////////
/// Destructor ///
//////////////////

/*
 * Destructor will complete a final save to the disk.
 */
SettingsManager::~SettingsManager()
{
    saveSettingsToFile();
}

//////////////////////////////////
/// Save and load settings.ini ///
//////////////////////////////////

/*
 * Loads settings from settings.ini. If the file cannot be found in the
 * executable's directory, default settings are loaded and saved into a newly
 * created file.
 */
void SettingsManager::loadSettingsFromFile()
{

}

/*
 * Saves any changes to the file
 */
void SettingsManager::saveSettingsToFile()
{

}
