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
    MPPCHit();

    ///
    /// \brief Initialize a MPPCHit in a given MPPC
    ///
    MPPCHit(const G4int MPPCID);

    // MPPCHit(const MPPCHit& right);
    // virtual ~MPPCHit();

    // operators for assignment and equality
    // const MPPCHit& operator=(const MPPCHit &right);
    // int operator==(const MPPCHit &right) const;

    // allocate/deallocate using G4Allocator
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    ///
    /// \brief Print a basic description of this hit.
    ///
    auto Print() -> void override;

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

///
/// \brief A custom thread-safe allocate for MPPCHits
///
extern G4ThreadLocal G4Allocator<MPPCHit>* MPPCHitAllocator;

///
/// \brief A thread-safe allocater for MPPC hits
///
inline void* MPPCHit::operator new(size_t) {
    if (!MPPCHitAllocator) {
        MPPCHitAllocator = new G4Allocator<MPPCHit>;
    }
    return (void*)MPPCHitAllocator->MallocSingle();
}

///
/// \brief A thread-safe deallocator for MPPCHits
///
inline void MPPCHit::operator delete(void* aHit) {
    MPPCHitAllocator->FreeSingle((MPPCHit*) aHit);
}
