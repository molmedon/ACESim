#pragma once

#include <G4VHit.hh>
#include <G4Color.hh>
#include <G4Circle.hh>
#include <G4Allocator.hh>
#include <G4VisManager.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
#include <G4THitsCollection.hh>


///
/// \brief Represents a hit in an ACE element.
///
class ACEHit final : public G4VHit {

public:

    ///
    /// \brief Construct a new ACEHit
    ///
    ACEHit()  : G4VHit(), fACENo(-1), fEnergy(0.), fCharge(0.),
                fPos(0.), fTime(0.), fLength(0.)  {}

    ///
    /// \brief Initialize a ACEHit in a given ACE
    ///
    ACEHit(const G4int ACENo) : G4VHit(), fACENo(ACENo), fEnergy(0.), fCharge(0.),
                                fPos(0.), fTime(0.), fLength(0.)  {}

    ///
    /// \brief Print a basic description of this hit.
    ///
    auto Print() -> void override {
        G4cout << "Hit in ACE " << fACENo << " with Edep=" << fEnergy << G4endl;

    }

    ///
    /// \brief Draw a yellow marker at every hit in an ACE element.
    ///
    auto Draw() -> void override {

        // get a copy of the vis manager
        auto visManager = G4VVisManager::GetConcreteInstance();
        if (! visManager) return; // vis is disabled

        // create a yellow circle, and draw it to the vis!
        G4Circle circle(fPos);
        circle.SetScreenSize(3);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,1.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        visManager->Draw(circle);
    }

    // member variables for this ACE hit
    G4int fACENo;        ///< The ID of the ACE element that was hiat.
    G4double fEnergy;    ///< The energy that was deposited.
    G4double fCharge;    ///< The PDG charge of the particle that hit.
    G4ThreeVector fPos;  ///< The position of the hit in global coords.
    G4double fTime;      ///< The time of the hit in global time.
    G4double fLength;    ///< The track length before the hit.

};


///
/// \brief A Hit collection of ACEHits
///
using ACEHitsCollection = G4THitsCollection<ACEHit>;
