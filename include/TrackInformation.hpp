#pragma once

#include "G4VUserTrackInformation.hh"

///
/// \brief An enum of different track status codes that we need.
///
enum TrackStatus { active=1, hitSiPM=2, absorbed=4, boundaryAbsorbed=8,
                   inactive=14};

/*TrackStatus:
  active: still being tracked
  hitSiPM: stopped by being detected in a SiPM
  absorbed: stopped by being absorbed with G4OpAbsorbtion
  boundaryAbsorbed: stopped by being aborbed with G4OpAbsorbtion at a boundary
  inactive: track is stopped for some reason
  -This is the sum of all stopped flags so can be used to remove stopped flags

*/


///
/// \brief This class stores information about a track.
///
class TrackInformation final : public G4VUserTrackInformation {

public:

    ///
    /// \brief Construct a new track action with default values.
    ///
    TrackInformation() : fStatus(active), fReflections(0),
                         fTotalInternalReflections(0) {};

    ///
    /// \brief Set the track status flag.
    ///
    /// This does not check the validity of status flags.
    ///
    auto SetTrackStatusFlags(const int s) -> void {fStatus=s;};

    //Does a smart add of track status flags (disabling old flags that conflict)
    //If s conflicts with itself it will not be detected
    auto AddTrackStatusFlag(G4int s) -> void {
        if(s&active) //track is now active
            fStatus&=~inactive; //remove any flags indicating it is inactive
        else if(s&inactive) //track is now inactive
            fStatus&=~active; //remove any flags indicating it is active
        fStatus|=s; //add new flags
    };

    // get the track status
    auto GetTrackStatus() const -> int {return fStatus;}

    // get and increment the total number of NON-TIR reflections
    auto IncReflections() -> void {fReflections++;}
    auto GetReflectionCount() const -> G4int {return fReflections;}

    // get and increment the total number of total internal reflections
    auto IncTotalInternalReflections() -> void {fTotalInternalReflections++;}
    auto GetTotalInternalReflectionCount() const -> G4int {return fTotalInternalReflections;}

    ///
    /// \brief Print some basic information about the information object.
    ///
    inline auto Print() const -> void override {};

private:

    ///
    /// \brief The status of this track.
    ///
    int fStatus;

    ///
    /// \brief The total number of non-TIR reflections in this track.
    ///
    G4int fReflections;

    ///
    /// \brief The total number of TIR reflections in this track.
    ///
    G4int fTotalInternalReflections;
};
