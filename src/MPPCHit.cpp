#include "MPPCHit.hpp"

// initialize a thread-safe allocator
G4ThreadLocal G4Allocator<MPPCHit>* MPPCHitAllocator;

// construct a new MPPChit and zero it out
MPPCHit::MPPCHit() : G4VHit(), fMPPCID(-1), fEnergy(0.), fPos(0.), fTime(0.),
                     fLength(0.), fReflections(0), fTotalInternalReflections(0) {}

// construct a new MPPChit and zero it out
MPPCHit::MPPCHit(const G4int ID) : G4VHit(), fMPPCID(ID), fEnergy(0.), fPos(0.), fTime(0.),
                                   fLength(0.), fReflections(0), fTotalInternalReflections(0) {}

// construct a new HIT in a given MPPC hit
// MPPCHit::MPPCHit(const G4int cellID) : G4VHit(),
//                                        fMPPCID(cellID), fEnergy(0.), fPos(0.), fTime(0.),
//                                        fLength(0.), fReflections(0), fTotalInternalReflections(0) {};


// MPPCHit::MPPCHit(const MPPCHit &right)
//     : G4VHit(),
//       fMPPCID(right.fMPPCID),
//       fEnergy(right.fEnergy),
//       fPos(right.fPos),
//       fTime(right.fTime),
//       fLength(right.fLength),
//       fReflections(right.fReflections),
//       fTotalInternalReflections(right.fTotalInternalReflections)
// {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// const MPPCHit& MPPCHit::operator=(const MPPCHit
//                                   &right)
// {
//     fMPPCID = right.fMPPCID;
//     fEnergy = right.fEnergy;
//     fPos = right.fPos;
//     fTime = right.fTime;
//     fLength = right.fLength;
//     fReflections = right.fReflections;
//     fTotalInternalReflections = right.fTotalInternalReflections;
//     return *this;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// int MPPCHit::operator==(const MPPCHit &right) const
// {
//     return (fMPPCID==right.fMPPCID);
// }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// print some basic info about MPPC
void MPPCHit::Print()
{
    G4cout << "  MPPC[" << fMPPCID << "] " << fEnergy << " (eV) at time " << fTime << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
