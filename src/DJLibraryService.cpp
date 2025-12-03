#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>

DJLibraryService::DJLibraryService(const Playlist &playlist)
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo> &library_tracks)
{
    // Todo: Implement buildLibrary method
    for (const auto &cfg : library_tracks)
    {
        AudioTrack *track = nullptr;
        if (cfg.type == "MP3")
        {
            track = new MP3Track(cfg.title, cfg.artists, cfg.duration_seconds, cfg.bpm, cfg.extra_param1, cfg.extra_param2);
            if (track)
            {
                library.push_back(track);
            }
        #ifdef DEBUG    
         std::cout << "MP3Track created: " << cfg.extra_param1 << " kbps" << std::endl;
         #endif
        }
        else if (cfg.type == "WAV")
        {
            track = new WAVTrack(cfg.title, cfg.artists, cfg.duration_seconds, cfg.bpm, cfg.extra_param1, cfg.extra_param2);

            if (track)
            {
                library.push_back(track);
            }
            #ifdef DEBUG
            std::cout << "WAVTrack created: " << cfg.extra_param1 << "Hz/" << cfg.extra_param2 << "bit" << std::endl;
            #endif
        }
    }

    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 *
 */
void DJLibraryService::displayLibrary() const
{
    std::cout << "=== DJ Library Playlist: "
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty())
    {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 *
 * @return Playlist&
 */
Playlist &DJLibraryService::getPlaylist()
{
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 *
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack *DJLibraryService::findTrack(const std::string &track_title)
{
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string &playlist_name,
                                               const std::vector<int> &track_indices)
{
    // Your implementation here
    // For now, add a placeholder to fix the linker error
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    int added_tracks = 0;
    for (int idx : track_indices)
    {
        if (idx <= 0 || static_cast<size_t>(idx) > library.size())
        {
            std::cout << "[WARNING] Invalid track index: " << idx << std::endl;
            continue;
        }
        else
        {
            AudioTrack *original = library[idx - 1];
            if (!original)
            {
                std::cout << "[WARNING] Null track pointer at index: " << idx << std::endl;
                continue;
            }
            PointerWrapper<AudioTrack> clone = original->clone();
            if (!clone)
            {
                std::cout << "[ERROR] Track: \"" << original->get_title() << "\" failed to clone" << std::endl;

                continue;
            }
            clone->load();
            clone->analyze_beatgrid();
            playlist.add_track(clone.release());
            std::cout << "Added '" << original->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;

            added_tracks++;
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << added_tracks << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const
{
    // Your implementation here
    std::vector<std::string> titles;
    std::vector<AudioTrack *> tracks = playlist.getTracks();
    for (AudioTrack *t : tracks)
    {
        if (t)
        {
            titles.push_back(t->get_title());
        }
    }

    return titles;
}
