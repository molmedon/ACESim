#pragma once

///
/// \brief This class records information about particle stepping.
///
/// Currently it only saves reflection and absorption information
/// of photons in the trigger system into TrackInformation class.
///
class SteppingAction final : public G4UserSteppingAction {

public:

    ///
    /// \brief Process a step
    ///
    auto UserSteppingAction(const G4Step*) -> void override;

private:

    // ///
    // /// \brief Check for hits in the ACE system and process.
    // ///
    // auto CheckForACEHits(const G4Step* step) -> void;

};
