#include <G4SDManager.hh>
#include <G4OpticalPhoton.hh>

#include "MPPC.hpp"
#include "TrackInformation.hpp"

///
/// \brief Initialize a new MPPC at the start of an event.
///
auto MPPC::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent) -> void {

    // create a new hits collection and store its ID
    fHitsCollection = new MPPCHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (fHCID<0) { // not initialized
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }

    // add the hit collection to this event
    hitsCollectionOfThisEvent->AddHitsCollection(fHCID, fHitsCollection);

}

///
/// \brief Process a hit at a given step.
///
auto MPPC::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/) -> G4bool {

    // we only want to process opticalphotons
    if (step->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
        return false;

    // get ID of MPPC that is hit
    const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
    const G4int MPPCID = touchable->GetVolume()->GetCopyNo();

    // get the track
    G4Track* track = step->GetTrack();

    // make sure we only save opticalphotons that deposit energy
    if ((track->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) &&
        (step->GetTotalEnergyDeposit() != 0)) {

        // create hit object
        MPPCHit* hit{new MPPCHit(MPPCID)};

        // photon energy
        hit->fEnergy = step->GetTotalEnergyDeposit();

        // hit position
        hit->fPos = track->GetPosition();

        // hit time in global time system
        hit->fTime = track->GetLocalTime();

        // insert total track length
        hit->fLength = track->GetTrackLength();

        // get track information storage
        const auto *trackInformation{(TrackInformation*)track->GetUserInformation()};

        // total number of reflections
        hit->fReflections = trackInformation->GetReflectionCount();

        // total number of total internal reflections
        hit->fTotalInternalReflections = trackInformation->GetTotalInternalReflectionCount();

        // insert the hit into the hits collection
        fHitsCollection->insert(hit);

    }

    // and everything worked correctly
    return true;

}

///
/// \brief Process a hit collection at the end of an event.
///
auto MPPC::EndOfEvent(G4HCofThisEvent* /*hitsCollectionOfThisEvent*/) -> void {


}
