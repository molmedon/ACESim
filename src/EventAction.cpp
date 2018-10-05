#include <numeric>

#include "MPPCHit.hpp"
#include "Analysis.hpp"
#include "EventAction.hpp"
#include "EventInformation.hpp"


// create a new UserEventInformat for this event
auto EventAction::BeginOfEventAction(const G4Event* /*event*/) -> void {

    // new event, add the user info
    G4EventManager::GetEventManager()->SetUserInformation(new EventInformation);

    // and reset all variables
    this->Reset();
}

auto EventAction::EndOfEventAction(const G4Event* event) -> void {

    // process trigger/photon information
    // this->ProcessTriggerHits(event);

    // and process info about hits in the ACE elements
    // this->ProcessACEHits(event);

}

// save and process all events from the detector system
auto EventAction::ProcessACEHits(const G4Event* event) -> void {

    // get analysis manager
    auto analysisManager{G4AnalysisManager::Instance()};

    // find the hit collections for the three ACE elements

     // fill histograms
    analysisManager->FillH1(1, fEnergyAbs);
    analysisManager->FillH1(2, fEnergyGap);
    analysisManager->FillH1(3, fTrackLAbs);
    analysisManager->FillH1(4, fTrackLGap);
    analysisManager->FillH1(5, fAbsHit);
    analysisManager->FillH1(6, fGapHit);

    // and print info about ACE elements
    auto eventID{event->GetEventID()};
    auto printModulo{G4RunManager::GetRunManager()->GetPrintProgress()};
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
        G4cout << "ACE ELEMENTS - Event: " << eventID << G4endl;

        G4cout
            << "   Tungsten: total energy: " << std::setw(7)
            << G4BestUnit(fEnergyAbs,"Energy")
            << "       total track length: " << std::setw(7)
            << G4BestUnit(fTrackLAbs,"Length")
            << "       total particles leaving: " << std::setw(7)
            << (fAbsHit)
            << G4endl
            << "        Alumina1: total energy: " << std::setw(7)
            << G4BestUnit(fEnergyGap,"Energy")
            << "       total track length: " << std::setw(7)
            << G4BestUnit(fTrackLGap,"Length")
            << "       total particles leaving: " << std::setw(7)
            << (fGapHit)
            << G4endl << G4endl;
    }

}

// save and process all events from the trigger system
auto EventAction::ProcessTriggerHits(const G4Event* event) -> void
{

    // get an analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // try and find the hit collections for photon informat
    if ( fPhotonsDetectedHCID == -1 ) {

        // get the photons detected by the MPPC
        fPhotonsDetectedHCID
            = G4SDManager::GetSDMpointer()->GetCollectionID("MPPC/photonsDetected");

        // // get the photons produced in the disk
        fPhotonsProducedHCID
            = G4SDManager::GetSDMpointer()->GetCollectionID("glassDisk/photonsProduced");
    }

    // count the numbr of photons produced
    const auto photonsProduced{GetPhotonsProduced(event)};

    // get the collections for photons produced and detected
    const auto HCEvent{event->GetHCofThisEvent()}; // all hit collections of this event

    // check that we actually got some photons
    if (photonsProduced <= 0) {
            G4cerr << "No Cerenkov photons were produced in the radiator...";
            G4cerr << " Something is wrong. Quitting..." << G4endl;
            throw std::exception();
    }

    // hit collection for photons in SiPM
    auto photonsDetectedHC{static_cast<MPPCHitsCollection*>(HCEvent->GetHC(fPhotonsDetectedHCID))};

    // this is the number of photons detected
    auto photonsDetected{photonsDetectedHC->entries()};

    // process photon that hit in the MPPC
    for (auto i=0; i < photonsDetected; i++) {

        // extract the hit
        auto hit = (*photonsDetectedHC)[i];

        // save SiPM ID
        auto MPPCID{hit->fMPPCID};
        analysisManager->FillNtupleDColumn(0, 0, MPPCID);

        // arrival location of photon
        // auto Pos = hit->GetPos()/CLHEP::mm;

        // arrival time of photon
        auto time{hit->fTime/CLHEP::ns};
        analysisManager->FillNtupleDColumn(0, 1, time);
        analysisManager->FillH1(14, time); // histogram of combined arrival times

        // photon energy
        auto energy{hit->fEnergy/CLHEP::eV};
        analysisManager->FillNtupleDColumn(0, 2, energy);

        // get total track length
        auto length{hit->fLength/CLHEP::mm};
        analysisManager->FillNtupleDColumn(0, 3, length);

        // number of reflections
        auto reflections{hit->fReflections};
        analysisManager->FillNtupleDColumn(0, 4, reflections);

        // number of total internal reflections
        auto totalReflections{hit->fTotalInternalReflections};
        analysisManager->FillNtupleDColumn(0, 5, totalReflections);

        // save the row the for Ntuple:0
        analysisManager->AddNtupleRow(20);

    }

    // fill histogram of detection efficiency
    analysisManager->FillH1(13, photonsDetected/photonsProduced);

    // additional event information
    auto eventInformation{(EventInformation*)event->GetUserInformation()};

    // extract photons that were absorpbed and boundary absorped
    auto photonsAbsorbed{eventInformation->GetAbsorptionCount()};
    auto photonsBoundaryAbsorbed{eventInformation->GetBoundaryAbsorptionCount()};

    // fill ntuple for the event-level information
    analysisManager->FillNtupleDColumn(1, 0, photonsDetected);
    analysisManager->FillNtupleDColumn(1, 1, photonsAbsorbed);
    analysisManager->FillNtupleDColumn(1, 2, photonsBoundaryAbsorbed);
    analysisManager->FillNtupleDColumn(1, 3, photonsProduced);
    analysisManager->AddNtupleRow(1);

    //print per event (modulo n)
    //
    auto eventID = event->GetEventID();
    auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
        G4cout << "---> End of event: " << eventID << G4endl << G4endl;
        G4cout << "TRIGGER - Event: " << eventID << G4endl;
        G4cout << "Photons Produced: " << photonsProduced << G4endl;
        G4cout << "Photons Detected: " << photonsDetected << G4endl;
        G4cout << "Detection Efficiency: " << G4double(photonsDetected)/G4double(photonsProduced) << G4endl;
        G4cout << G4endl;
    }

}


// reset all internal state
auto EventAction::Reset() -> void {

    // these are indexes into HC's so we make these -1, not 0
    fPhotonsDetectedHCID = -1;
    fPhotonsProducedHCID = -1;

    // these are all values so zero these out.
    fEnergyAbs = 0;
    fEnergyGap = 0;
    fTrackLAbs = 0;
    fTrackLGap = 0;
    fAbsHit = 0;
    fGapHit = 0;
    fAbsHitx = 0;
    fAbsHity = 0;
    fGapHitx = 0;
    fGapHity = 0;

}

// get the number of photons produced from a hit collection
auto EventAction::GetPhotonsProduced(const G4Event* event) const -> G4int {

    // get the collections for photons produced and detected
    const auto HCEvent{event->GetHCofThisEvent()}; // all hit collections of this event

    // convert it into a map
    const auto hitsMap{static_cast<G4THitsMap<G4double>*>(HCEvent->GetHC(fPhotonsProducedHCID))};

    G4double sumValue = 0.;
    for ( auto it : *hitsMap->GetMap() ) {
        // hitsMap->GetMap() returns the map of std::map<G4int, G4double*>
        sumValue += *(it.second);
    }
    return sumValue;

}
