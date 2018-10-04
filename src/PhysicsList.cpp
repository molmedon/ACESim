#include "PhysicsList.hpp"

#include <G4IonPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4OpticalPhysics.hh>
#include <G4StoppingPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4NeutronTrackingCut.hh>
#include <G4HadronElasticPhysicsHP.hh>
#include <G4HadronPhysicsFTFP_BERT_HP.hh>

// construct our physics list
PhysicsList::PhysicsList() {

    // whether to be verbose when printing physics list messages
    const G4int verbose{false}; SetVerboseLevel(verbose);

    // EM Physics
    RegisterPhysics( new G4EmStandardPhysics(verbose) );

    // Synchroton Radiation & GN Physics
    RegisterPhysics( new G4EmExtraPhysics(verbose) );

    // Decays
    RegisterPhysics( new G4DecayPhysics(verbose) );

    // Hadron Physics
    RegisterPhysics( new G4StoppingPhysics(verbose) );
    RegisterPhysics( new G4IonPhysics(verbose) );
    RegisterPhysics( new G4HadronElasticPhysicsHP(verbose));
    RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verbose));

    // Neutron tracking cut
    RegisterPhysics( new G4NeutronTrackingCut(verbose) );

    // optical
    RegisterPhysics( new G4OpticalPhysics(verbose) );

}
