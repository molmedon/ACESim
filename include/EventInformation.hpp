#pragma once

#include <G4VUserEventInformation.hh>

///
/// \brief This class accumulates statistics for an event.
///
class EventInformation final : public G4VUserEventInformation
{
  public:

    ///
    /// \brief Construct a new EventInformation instance.
    ///
    EventInformation() {};

    ///
    /// \brief Print information about this event.
    ///
    auto Print() const -> void override {};

    ///
    /// \brief Increment the number of non-boundary absorbed photons.
    ///
    auto IncAbsorption() -> void {fAbsorptionCount++;}

    ///
    /// \brief Get the number of non-boundary absorbed photons.
    ///
    auto GetAbsorptionCount() -> const G4int {return fAbsorptionCount;}

    ///
    /// \brief Increment the number of boundary absorbed photons.
    ///
    auto IncBoundaryAbsorption() -> void {fBoundaryAbsorptionCount++;}

    ///
    /// \brief Get the number of boundary absorbed photons
    ///
    auto GetBoundaryAbsorptionCount() const -> G4int {return fBoundaryAbsorptionCount;}


  private:

    ///
    /// \brief The number of photons absorbed.
    ///
    G4int fAbsorptionCount;

    ///
    /// \brief The number of photons absorbed at a boundary.
    ///
    G4int fBoundaryAbsorptionCount;


};
