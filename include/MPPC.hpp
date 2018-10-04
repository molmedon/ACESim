#pragma once

#include <G4VSensitiveDetector.hh>

#include "MPPCHit.hpp"


///
/// \brief This class represent a MPPC SensitiveDetector object.
///
class MPPC final : public G4VSensitiveDetector {

public:

    ///
    /// \brief Create a new MPPC with a given identifier.
    ///
    MPPC(const G4String& name) : G4VSensitiveDetector(name),
                                 fHCID(-1), fHitsCollection(nullptr) {

        // register the name of HC's of this MPPC
        collectionName.insert("photonsDetected");
    };

    ///
    /// \brief Initialize a new MPPC at the start of an event.
    ///
    auto Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent) -> void;

    ///
    /// \brief Process a hit at a given step.
    ///
    auto ProcessHits(G4Step* step, G4TouchableHistory* history) -> G4bool;

    ///
    /// \brief Process a hit collection at the end of an event.
    ///
    auto EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent) -> void;

private:

    ///
    ///\brief The ID of the below hits collection.
    ///
    G4int fHCID;

    ///
    /// \brief The collection of hits into this MPPC for an event.
    ///
    MPPCHitsCollection* fHitsCollection;

};
