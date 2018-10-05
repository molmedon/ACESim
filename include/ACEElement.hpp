#pragma once

#include <G4VSensitiveDetector.hh>

#include "ACEHit.hpp"

///
/// \brief A class representing the sensitive parts of a single ACE element.
///
class ACEElement final : public G4VSensitiveDetector {

public:

    ///
    /// \brief Create a new ACE element.
    ///
    ACEElement(const G4String& name) : G4VSensitiveDetector(name),
                                       fHCID(-1), fHitsCollection(nullptr) {
        collectionName.insert("hits");
    };

    ///
    /// \brief Initialize the ACE element at the start of an event.
    ///
    auto Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent) -> void override;

    ///
    /// \brief Process a hit in the detector at a given step.
    ///
    auto ProcessHits(G4Step* step, G4TouchableHistory* history) -> G4bool override;

    ///
    /// \brief Process a hit collection at the end of an event.
    ///
    auto EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent) -> void override;

private:

    ///
    ///\brief The ID of the below hits collection.
    ///
    G4int fHCID;

    ///
    /// \brief The collection of hits into this MPPC for an event.
    ///
    ACEHitsCollection* fHitsCollection;

};
