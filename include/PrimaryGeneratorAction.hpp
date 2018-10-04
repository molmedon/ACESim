#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

///
/// \brief A class responsible for generating primary particles.
///
/// The properties of the source are controlled via macros, but via
/// code in this class. See macros/beam.mac to see an example for how
/// to set the beam properties.
///
class PrimaryGeneratorAction final : public G4VUserPrimaryGeneratorAction {

public:

    ///
    /// \brief The particle gun used to generate primaries.
    ///
    G4GeneralParticleSource *particleGun;

    ///
    /// \brief Just initialize the particle gun
    ///
    PrimaryGeneratorAction() : particleGun(new G4GeneralParticleSource()) {};

    ///
    /// \brief Delete the gun when we are finished with it.
    ///
    ~PrimaryGeneratorAction() { delete particleGun; };

    ///
    /// \brief This function is called to generate primaries.
    ///
    auto GeneratePrimaries(G4Event* event) -> void {
        this->particleGun->GeneratePrimaryVertex(event);
    };

};
