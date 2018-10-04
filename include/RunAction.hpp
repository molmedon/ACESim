#pragma once

#include <G4Run.hh>
#include <G4UserRunAction.hh>

#include "Analysis.hpp"


///
/// \brief This class accumulates information across an entire run.
///

class RunAction final : public G4UserRunAction {

public:

    ///
    /// \brief Initialize the analysisManager and data collections.
    ///
    RunAction();

    ///
    /// \brief Delete the analysis manager when we are finished running.
    ///
    ~RunAction();

    ///
    /// \brief Initialize before the first run (open files)
    ///
    auto BeginOfRunAction(const G4Run*) -> void override;

    ///
    /// \brief Printout run statistics and save data files.
    ///
    auto EndOfRunAction(const G4Run*) -> void override;
};
