#include <G4SDManager.hh>

#include "ACEElement.hpp"


///
/// \brief Initialize the ACE element at the start of an event.
///
auto ACEElement::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent) -> void {

    // create a new hits collection and store its ID
    fHitsCollection = new ACEHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (fHCID<0) { // not initialized
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }

    // add the hit collection to this event
    hitsCollectionOfThisEvent->AddHitsCollection(fHCID, fHitsCollection);

}

///
/// \brief Process a hit in the detector at a given step.
///
auto ACEElement::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/) -> G4bool {

    // get ID of the ACE element that was hit
    const auto touchable{step->GetPreStepPoint()->GetTouchable()};
    const auto copyNo{touchable->GetVolume()->GetCopyNo()};

    // get the track
    auto track{step->GetTrack()};

    // only process events that deposit energy
    if (step->GetTotalEnergyDeposit() != 0) {

        // create a new hit in this volume
        auto hit{new ACEHit(copyNo)};

        // energy deposited in this step
        hit->fEnergy = step->GetTotalEnergyDeposit();

        // and the charge
        hit->fCharge = track->GetDefinition()->GetPDGCharge();

        // hit position
        hit->fPos = track->GetPosition();

        // hit time in global time system
        hit->fTime = track->GetGlobalTime();

        // insert total track length
        hit->fLength = track->GetTrackLength();

        // and insert this hit into our hits collection
        this->fHitsCollection->insert(hit);

        hit->Print();

    }


}

///
/// \brief Process a hit collection at the end of an event.
///
auto ACEElement::EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent) -> void {

}
