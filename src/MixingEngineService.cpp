#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
        : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
   decks[0] = nullptr;
   decks[1] = nullptr;
       std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;


}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
  for(int i =0; i < 2 ; i++){
    if(decks[i]){
        delete decks[i];
        decks[i] = nullptr;
    }
   }
   }

   MixingEngineService:: MixingEngineService(const  MixingEngineService& other) :decks(), active_deck(1), auto_sync(false), bpm_tolerance(0){
     PointerWrapper<AudioTrack> clone1 = other.decks[0]->clone();
      PointerWrapper<AudioTrack> clone2 = other.decks[1]->clone();
    decks[0] = clone1.release();
    decks[1] = clone2.release();
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;
   }
   
   MixingEngineService& MixingEngineService :: operator=(const  MixingEngineService& other){
    if(this == &other){
        return *this;
    }
    for(int i =0; i < 2 ; i++){
    if(decks[i]){
        delete decks[i];
        decks[i] = nullptr;
    }
   }
    PointerWrapper<AudioTrack> clone1 = other.decks[0]->clone();
      PointerWrapper<AudioTrack> clone2 = other.decks[1]->clone();
    decks[0] = clone1.release();
    decks[1] = clone2.release();
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;

  return *this;
   }



/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
 std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    std::string title = track.get_title();
    PointerWrapper<AudioTrack> clone_track(track.clone());
    if(!clone_track){
        std::cerr << "[ERROR] Track: \"" << title << "\" failed to clone\n";
    return -1;
    }
    int target ;
   if(!decks[0] && !decks[1]){
    target = 0;
    }
    else{
        target = 1 - active_deck;
    }
    std::cout << "[Deck Switch] Target deck: " << target << std::endl;
    if(decks[target]){
         delete decks[target];
        decks[target] = nullptr;
    }
    clone_track->load();
    clone_track->analyze_beatgrid();
    if (decks[active_deck] != nullptr && auto_sync) {
    if(!can_mix_tracks(clone_track)){
        sync_bpm(clone_track);
    }
}
    decks[target] = clone_track.release();
    std::cout << "[Load Complete] '" 
          << decks[target]->get_title() << "' is now loaded on deck " << target << std::endl;
        if(decks[active_deck] && static_cast<size_t>(target) ==active_deck){
              std::cout << "[Unload] Unloading previous deck " << active_deck<< " (" << decks[active_deck]->get_title() << ")" << std::endl;
            delete decks[active_deck];
            decks[active_deck] = nullptr;
        }
        active_deck = target;
        std::cout << "[Active Deck] Switched to deck " << target << std::endl;


         
    return target;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(!decks[active_deck]){
        return false;
    }
    if (!track){
        return false;
    }
   
     int current_bpm = decks[active_deck]->get_bpm();
      int new_bpm  = track->get_bpm();
      int diff = std::abs(current_bpm - new_bpm);
      return (diff <= bpm_tolerance);
    
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
     if( decks[active_deck] && track){
        int original_bpm = track->get_bpm();
         int active_bpm   = decks[active_deck]->get_bpm();
         int avg = (active_bpm + original_bpm)/2;
         track -> set_bpm(avg);
          std::cout << "[Sync BPM] Syncing BPM from "<< original_bpm << " to " << avg <<std::endl;
              
    }
    
    return;
}

