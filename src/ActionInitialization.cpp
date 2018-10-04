#include "RunAction.hpp"
#include "EventAction.hpp"
// #include "SteppingAction.hpp"
// #include "TrackingAction.hpp"
#include "ActionInitialization.hpp"
#include "PrimaryGeneratorAction.hpp"


// initialization for master threads
auto ActionInitialization::BuildForMaster() const -> void {
    SetUserAction(new RunAction);
}

// initialization for worker threads
auto ActionInitialization::Build() const -> void {

    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(new RunAction);
    SetUserAction(new EventAction);
    // SetUserAction(new SteppingAction);
    // SetUserAction(new TrackingAction);
}
