#pragma once

#include "G4VUserActionInitialization.hh"

///
/// This class is responsible for registering all user actions to Geant4.
/// The constructors register primary generator, run, event, stepping
/// and tracking actions during initialization.
///
class ActionInitialization final : public G4VUserActionInitialization {

public:

    ///
    /// \brief This initializes the actions for the master (main) thread.
    ///
    auto BuildForMaster() const -> void override;

    ///
    /// \brief This initializes the action for all the worker threads.
    ///
    auto Build() const -> void override;

};
