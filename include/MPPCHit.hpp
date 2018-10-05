#pragma once

#include <G4VHit.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>
#include <G4THitsCollection.hh>

class MPPCHit final : public G4VHit {

public:

    ///
    /// \brief Construct a new MPPCHit
    ///
    MPPCHit() : G4VHit(), fMPPCID(-1), fEnergy(0.), fPos(0.), fTime(0.),
                fLength(0.), fReflections(0), fTotalInternalReflections(0) {}

    ///
    /// \brief Initialize a MPPCHit in a given MPPC
    ///
    MPPCHit(const G4int ID) : G4VHit(), fMPPCID(ID), fEnergy(0.), fPos(0.), fTime(0.),
                              fLength(0.), fReflections(0), fTotalInternalReflections(0) {}

    ///
    /// \brief Print a basic description of this hit.
    ///
    auto Print() -> void {
        G4cout << "  MPPC[" << fMPPCID << "] " << fEnergy << " (eV) at time " << fTime << G4endl;
    };

    // member variables for this MPPC hit
    G4int fMPPCID;       ///< The ID of the MPPC that was hit.
    G4double fEnergy;    ///< The energy that was deposited.
    G4ThreeVector fPos;  ///< The position of the hit in global coords.
    G4double fTime;      ///< The time of the hit in global time.
    G4double fLength;    ///< The length of the photon path before the hit.
    G4int fReflections;  ///< The total number of reflections it underwent.
    G4int fTotalInternalReflections; ///< The total number of TIR it hit.

};

///
/// \brief A Hit collection of MPPCHits
///
using MPPCHitsCollection = G4THitsCollection<MPPCHit>;
