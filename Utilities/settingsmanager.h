#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H


/*
 * The SettingsManager is a singleton across all instances of EGG windows. It
 * manages all the options, settings, recent files, and any other helpful
 * features that may need to be accessed across EGG instances.
 *
 * SettingsManager will attempt to load from settings.ini when the application
 * first starts up. If no settings file is found, a default settings.ini is
 * generated and saved to the executable directory.
 *
 * SettingsManager maintains a globally accessable database of various flags
 * and operations that the application may need to access.
 */
class SettingsManager
{
public:
    /* Obtain the singleton */
    static SettingsManager& getInstance();

    /* Destructor */
    ~SettingsManager();

    /* Save / load settings.ini */
    void loadSettingsFromFile();
    void saveSettingsToFile();

    /* Delete non-usable functions */
    SettingsManager(SettingsManager const&) = delete;
    void operator=(SettingsManager const&) = delete;

private:
    /* Private constructor */
    SettingsManager();
};

#endif // SETTINGSMANAGER_H
