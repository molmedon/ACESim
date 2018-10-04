#include <numeric>

#include "Analysis.hpp"
#include "EventAction.hpp"
#include "EventInformation.hpp"
#include "MPPCHit.hpp"

// create a new UserEventInformat for this event
auto EventAction::BeginOfEventAction(const G4Event* /*event*/) -> void {

    // new event, add the user info
    G4EventManager::GetEventManager()->SetUserInformation(new EventInformation);
}


auto EventAction::EndOfEventAction(const G4Event* event) -> void
{

    // // get an analysis manager
    // auto analysisManager = G4AnalysisManager::Instance();

    // // try and find the hit collections for photon informat
    // if ( fPhotonsDetectedHCID == -1 ) {

    //     // get the photons detected by the MPPC
    //     fPhotonsDetectedHCID
    //         = G4SDManager::GetSDMpointer()->GetCollectionID("MPPC/photonsDetected");

    //     // get the photons produced in the disk
    //     fPhotonsProducedHCID
    //         = G4SDManager::GetSDMpointer()->GetCollectionID("Disk/photonsProduced");
    // }

    // // get the collections for photons produced and detected
    // const auto HCEvent{event->GetHCofThisEvent()}; // all hit collections of this event
    // const auto photonsProduced{static_cast<MPPCHitsCollection*>(HCEvent->GetHC(fPhotonsProducedHCID))};
    // const auto photonsDetected{static_cast<MPPCHitsCollection*>(HCEvent->GetHC(fPhotonsDetectedHCID))};

    // // get the total number of photons produced in this event
    // const auto NPhotonsProduced{GetSum(photonsProduced)};

    // // get the total number of photons produced in this event
    // const auto NPhotonsDetected{GetSum(photonsProduced)};


}


// reset all internal state
auto EventAction::Reset() -> void {

    // these are indexes into HC's so we make these -1, not 0
    fPhotonsDetectedHCID = -1;
    fPhotonsProducedHCID = -1;

    // these are all values so zero these out.
    fEnergyAbs = 0;
    fEnergyGap = 0;
    fTrackLAbs = 0;
    fTrackLGap = 0;
    fAbsHit = 0;
    fGapHit = 0;
    fAbsHitx = 0;
    fAbsHity = 0;
    fGapHitx = 0;
    fGapHity = 0;

}


// sum the values produced in a hits collection
// auto EventAction::GetSum(const G4THitsMap<G4double>* hitsMap) const -> G4double
// {
//     G4double sumValue = 0.;
//     for ( auto it : *hitsMap->GetMap() ) {
//         // hitsMap->GetMap() returns the map of std::map<G4int, G4double*>
//         sumValue += *(it.second);
//     }
//     return sumValue;
// }
