#include <string>

#include <G4UImanager.hh>
#include <G4RunManager.hh>
#include <G4UIExecutive.hh>
#include <G4MTRunManager.hh>
#include <G4VisExecutive.hh>
#include <G4ScoringManager.hh>

#include "PhysicsList.hpp"
#include "CommandLineParser.hpp"
#include "ActionInitialization.hpp"
#include "DetectorConstruction.hpp"


//
// This is the main program driver for starting binaries.
//
auto main(int argc, char** const argv) -> int {

    // parse the command line arguments
    const auto oargs = parseCommandLine(argc, argv);

    // if an error occured, just return
    if (!oargs)
        return 1;

    // otherwise, save the variable map and continue
    const auto args = *oargs; // `args` contains the parsed command line options

    // choose a better random number generator
    // G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // construct the default run manager
    auto runManager{new G4RunManager};
    // runManager->SetNumberOfThreads(args["num-threads"].as<int>());

    // we must IMMEDIATELY construct the scoring manager
    auto scoringManager{G4ScoringManager::GetScoringManager()};
    (void)scoringManager; // so the compiler doesn't complain

    // register our physics list
    runManager->SetUserInitialization(new PhysicsList);

    // register geometry
    runManager->SetUserInitialization(new DetectorConstruction);

    // set action generator
    runManager->SetUserInitialization(new ActionInitialization);

    // initialize the Geant4 kernel
    runManager->Initialize();

    // get pointer to UI manager
    G4UImanager* UIManager{G4UImanager::GetUIpointer()};

    // allow macros to be run from the macros directory
    UIManager->ApplyCommand("/control/macroPath macros ");

    // create vis manager
    G4VisManager* visManager{new G4VisExecutive};
    visManager->Initialize();

    // the user provided a macro
    if (args["macro"].as<std::string>() != "") {
        UIManager->ApplyCommand("/control/execute "+ args["macro"].as<std::string>());
    }

    // we open an interactive session
    G4UIExecutive* ui{new G4UIExecutive(argc, argv)};
    ui->SessionStart();

    // cleanup the run manager
    delete runManager;

    // and we are done
    return 0;

}
