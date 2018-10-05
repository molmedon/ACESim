#pragma once

#include <G4Event.hh>
#include <globals.hh>
#include <G4THitsMap.hh>
#include <G4SDManager.hh>
#include <G4RunManager.hh>
#include <G4UnitsTable.hh>
#include <G4HCofThisEvent.hh>
#include <G4UserEventAction.hh>

///
/// \brief Accumulate information for a single event.
///
class EventAction final : public G4UserEventAction

{
public:

    ///
    /// \brief Initialize variables
    ///
    EventAction() : G4UserEventAction() {
        Reset(); // zero out all variables
    };

    ///
    /// \brief Zero out all variables before starting a new event.
    ///
    auto BeginOfEventAction(const G4Event* event) -> void override;

    ///
    /// \brief Save the information collected in the event.
    ///
    auto EndOfEventAction(const G4Event* event) -> void override;

    // the ID's of the hit collections for the photons detected
    // and the photons produced in this event
    G4int fPhotonsDetectedHCID;
    G4int fPhotonsProducedHCID;

    // these members are used to store information in ACE elements
    G4double  fEnergyAbs;
    G4double  fEnergyGap;
    G4double  fTrackLAbs;
    G4double  fTrackLGap;
    G4double  fAbsHit;
    G4double  fGapHit;
    G4double  fAbsHitx;
    G4double  fAbsHity;
    G4double  fGapHitx;
    G4double  fGapHity;

private:

    ///
    /// \brief Process and save data from the trigger system.
    ///
    /// This must only be called from EndOfEventAction().
    ///
    auto ProcessTriggerHits(const G4Event* event) -> void;

    ///
    /// \brief Process and save data from the ACE elements.
    ///
    /// This must only be called from EndOfEventAction().
    ///
    auto ProcessACEHits(const G4Event* event) -> void;

    ///
    /// \brief Zero-out all member variables.
    ///
    auto Reset() -> void;


    /// \brief Count the number of
    ///
    auto GetPhotonsProduced(const G4Event* event) const -> G4int;

// private:
//     // methods
//     G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
//                                             const G4Event* event) const;


};
