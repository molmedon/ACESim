#include "PhysicsList.hpp"

#include <G4IonPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4OpticalPhysics.hh>
#include <G4StoppingPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4HadronElasticPhysics.hh>
#include <G4HadronPhysicsFTFP_BERT.hh>
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
    RegisterPhysics( new G4HadronElasticPhysics(verbose));
    RegisterPhysics( new G4HadronPhysicsFTFP_BERT(verbose));

    // these two are high precision hadron physics - use these for production runs.
    // RegisterPhysics( new G4HadronElasticPhysicsHP(verbose));
    // RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verbose));


    // and register our optical physics
    RegisterPhysics( new G4OpticalPhysics(verbose) );

}
