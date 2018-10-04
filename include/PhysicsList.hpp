#pragma once

#include <G4VModularPhysicsList.hh>


///
/// \brief The physics used by ACE. Since we neew
/// to simulate both optical and particle physics, we
/// have to build a combo physics list.
///
/// The currently enabled physics is:
///   1) Hadron physics with FTFP and BERT with high
///      precision neutrion tracking ('_HP')
///   2) Standard and extra EM physics
///   3) Decay physics
///   4) Stoping physics
///   5) Ion physics
///   6) Neutrino Tracking
///   7) Optical Physics
///
class PhysicsList : public G4VModularPhysicsList {

public:

    ///
    /// \brief Instantiate the necessary modules for our list.
    ///
    PhysicsList();

};
