#include "RunAction.hpp"

#include <G4RunManager.hh>
#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() : G4UserRunAction() {

    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);

    // Create analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetFirstHistoId(1);

    // save data to data directory
    // analysisManager->SetHistoDirectoryName("../../data/");
    // analysisManager->SetNtupleDirectoryName("../../data/");

    ////////////////////////////////////////////////////////////////////////////
    ///// PHOTON HIT INFORMATION

    // create a Tree to store data for every photon acquired during the run
    analysisManager->CreateNtuple("PhotonHitData", "Photon Hit Information for every Event");
    analysisManager->CreateNtupleDColumn("Event");
    analysisManager->CreateNtupleDColumn("MPPC");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->CreateNtupleDColumn("Energy");
    // analysisManager->CreateNtupleDColumn("TrackLength");
    // analysisManager->CreateNtupleDColumn("Reflections");
    // analysisManager->CreateNtupleDColumn("TotalInternalReflections");
    analysisManager->FinishNtuple();

    // create another Tree to store data about the number of photons produced and detected
    analysisManager->CreateNtuple("PhotonEffData", "Photon Efficiency Information for Every Event");
    analysisManager->CreateNtupleDColumn("photonsDetected");
    analysisManager->CreateNtupleDColumn("photonsAbsorbed");
    analysisManager->CreateNtupleDColumn("photonsBoundaryAbsorbed");
    analysisManager->CreateNtupleDColumn("photonsProduced");
    analysisManager->FinishNtuple();

    ////////////////////////////////////////////////////////////////////////////
    /// ACE HIT INFORMATION

    // these are copied from PG's original MC verbatim
    analysisManager->CreateH1("h1","Edep in tungsten", 100, 0., 5*GeV);
    analysisManager->CreateH1("h2","Edep in Alumina1", 100, 0., 2*GeV);
    analysisManager->CreateH1("h3","trackL in tungsten", 100, 0., 300*cm);
    analysisManager->CreateH1("h4","trackL in Alumina1", 100, 0., 300*cm);
    analysisManager->CreateH1("h5","Hits in Tungsten 1", 50, 0., 100 );
    analysisManager->CreateH1("h6","Hits in ACE 1", 50, 0., 100 );
    analysisManager->CreateH1("h7","Radial hit profile, ACE 1", 24, 0., 6.0 );
    analysisManager->CreateH1("h8","Radial hit profile, ACE 2", 24, 0., 6.0 );
    analysisManager->CreateH1("h9","Radial hit profile, ACE 3", 24, 0., 6.0 );
    analysisManager->CreateH1("h10","temporal hit profile, ACE 1", 150, 0., 0.15 );
    analysisManager->CreateH1("h11","temporal hit profile, ACE 2", 150, 0., 0.15 );
    analysisManager->CreateH1("h12","temporal hit profile, ACE 3", 150, 0., 0.15 );
    analysisManager->CreateH2("s1","hits in tungsten 1", 60, -6.0, 6.0,60, -6.0, 6.0 );
    analysisManager->CreateH2("s2","excess charge in ACE1", 60, -6.0, 6.0,60, -6.0, 6.0 );
    analysisManager->CreateH2("s3","excess charge in ACE2", 60, -6.0, 6.0,60, -6.0, 6.0 );
    analysisManager->CreateH2("s4","excess charge in ACE3", 60, -6.0, 6.0,60, -6.0, 6.0 );
    analysisManager->CreateH2("s5","total charge in ACE1", 60, -6.0, 6.0,60, -6.0, 6.0 );
    analysisManager->CreateH2("s6","total charge in ACE2", 60, -6.0, 6.0,60, -6.0, 6.0 );
    analysisManager->CreateH2("s7","total charge in ACE3", 60, -6.0, 6.0,60, -6.0, 6.0 );

    ////////////////////////////////////////////////////////////////////////////
    /// PHOTON HIT HISTOGRAMS

    // a histogram of photon detection efficiency in the MPPC's
    analysisManager->CreateH1("PhotonDetectionEfficiency",
                              "Photon Detection Efficiency (detected/produced)", 100, 0.0, 0.05);
    analysisManager->SetH1XAxisTitle(13, "Photon Detection Efficiency (detected/produced)");

    // create a histogram of the time of arrival of photons from the hit time
    analysisManager->CreateH1("PhotonArrivalTime",
                              "Arrival Times for Combined MPPC Outputs", 100, 0.0, 1.0, "ns");
    analysisManager->SetH1XAxisTitle(14, "Photon Arrival Time");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction() {
    delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

auto RunAction::BeginOfRunAction(const G4Run* /*run*/) -> void {

    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Open an output file
    G4String fileName = "ACESim";
    G4cout << "Saving data to " << fileName << G4endl;
    analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

auto RunAction::EndOfRunAction(const G4Run* /*run*/) -> void {


    // get the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    std::cerr << "Entering end of run action!";

    // print histogram statistics
    if ( analysisManager->GetH1(1) ) {
        G4cout << G4endl << " ----> print histograms statistic ";
        if(isMaster) {
            G4cout << "for the entire run " << G4endl << G4endl;
        }
        else {
            G4cout << "for the local thread " << G4endl << G4endl;
        }

        G4cout << " Etungsten : mean = "
               << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
               << " rms = "
               << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;

        G4cout << " EAlumina1 : mean = "
               << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy")
               << " rms = "
               << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl;

        G4cout << " LTungsten : mean = "
               << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length")
               << " rms = "
               << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;

        G4cout << " LAlumina1 : mean = "
               << G4BestUnit(analysisManager->GetH1(4)->mean(), "Length")
               << " rms = "
               << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") << G4endl;

        G4cout << " tungsten output hits : mean = "
               << analysisManager->GetH1(5)->mean()
               << " rms = "
               << analysisManager->GetH1(5)->rms() << G4endl;
        G4cout << " Alumina1 output hits : mean = "
               << analysisManager->GetH1(6)->mean()
               << " rms = "
               << analysisManager->GetH1(6)->rms() << G4endl;

        G4cout << " Photon Detection Efficiency : mean = "
               << 100*analysisManager->GetH1(13)->mean()
               << "% rms = "
               << 100*analysisManager->GetH1(13)->rms() << "%" << G4endl << G4endl;
    }

    std::cerr << "Writing files!";

    // save histograms & ntuple
    //
    analysisManager->Write();
    analysisManager->CloseFile();

    std::cerr << "Leaving end of run action\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
