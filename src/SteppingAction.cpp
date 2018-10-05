#include <G4Step.hh>
#include <G4Track.hh>
#include <G4Event.hh>
#include <G4StepPoint.hh>
#include <G4TrackStatus.hh>
#include <G4EventManager.hh>
#include <G4ProcessManager.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4UserSteppingAction.hh>

#include "SteppingAction.hpp"
#include "EventInformation.hpp"
#include "TrackInformation.hpp"


// process a step
auto SteppingAction::UserSteppingAction(const G4Step * step) -> void {

    // get the track
    G4Track* track = step->GetTrack();

    // get the post volume to look at boundary effects
    G4StepPoint *thePostPoint = step->GetPostStepPoint();
    G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();

    // check if we are out of world
    if (!thePostPV) {
        return;
    }

    // get track information storage
    auto *trackInformation{(TrackInformation*)track->GetUserInformation()};

    // get event information storage
    auto *eventInformation{(EventInformation*)G4EventManager::GetEventManager()
            ->GetConstCurrentEvent()->GetUserInformation()};

    // make sure we only track optical photons
    G4ParticleDefinition* particleType = track->GetDefinition();
    if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
        //Was the photon absorbed by the absorption process
        if(thePostPoint->GetProcessDefinedStep()->GetProcessName() == "OpAbsorption") {
            trackInformation->AddTrackStatusFlag(absorbed);
            eventInformation->IncAbsorption();
        }
    } else {
        // we then check whether these are hits in ACE
        // this->CheckForACEHits(step);
        return;
    }

    // define variables for optical boundary processses
    G4OpBoundaryProcessStatus boundaryStatus=Undefined;

    // static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;
    // search for the boundary
    G4OpBoundaryProcess* boundary{NULL};

    //find the boundary process only once
    if(!boundary){
        G4ProcessManager* pm
            = step->GetTrack()->GetDefinition()->GetProcessManager();
        G4int nprocesses = pm->GetProcessListLength();
        G4ProcessVector* pv = pm->GetProcessList();
        G4int i;
        for( i=0;i<nprocesses;i++){
            if((*pv)[i]->GetProcessName()=="OpBoundary"){
                boundary = (G4OpBoundaryProcess*)(*pv)[i];
                break;
            }
        }
    }

    // get status of optical boundary
    boundaryStatus=boundary->GetStatus();

    // see what case the boundary is
    switch(boundaryStatus){

    case Absorption:
        trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
        eventInformation->IncBoundaryAbsorption();
        break;
    case Detection:
        break;
    case TotalInternalReflection:
        trackInformation->IncTotalInternalReflections();
        break;
    case FresnelReflection:
        trackInformation->IncReflections();
        break;
    case LambertianReflection:
        trackInformation->IncReflections();
        break;
    case LobeReflection:
        trackInformation->IncReflections();
        break;
    case SpikeReflection:
        trackInformation->IncReflections();
        break;
    case BackScattering:
    default:
        break;
    }
}
