#pragma once

#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>

#include "G4VUserDetectorConstruction.hh"


///
/// \brief This class constructs the detector geometry.
///
/// The implementations are spread across TriggerGeometry
/// and ElementGeometry while common functions are in DetectorConstruction.
///
class DetectorConstruction final : public G4VUserDetectorConstruction {

public:

    ///
    /// \brief Initialize a new DetectorConstruction.
    ///
    DetectorConstruction() {
        DefineMaterials(); // define all materials before we start building
    }

    ///
    /// \brief Construct the simulation geometry.
    ///
    auto Construct() -> G4VPhysicalVolume* override;

    ///
    /// \brief Construct all sensitive detectors and fields.
    ///
    auto ConstructSDandField() -> void override;

private:

    ///
    /// \brief Define all the materials we will use.
    ///
    auto DefineMaterials() const -> void;

    ///
    /// \brief Create the world volume.
    ///
    auto CreateWorld() -> G4LogicalVolume*;

    ///
    /// \brief Create an assembled waveguide.
    ///
    auto ConstructWaveguides() -> G4AssemblyVolume*;

    ///
    /// \brief Construct the dewer assembly.
    ///
    auto ConstructDewer() -> G4AssemblyVolume*;

    ///
    /// \brief Construct the trigger system
    ///
    auto ConstructTriggerSystem() -> G4AssemblyVolume*;


    G4LogicalVolume*   createSlide();
    G4LogicalVolume*   createBox();
    G4LogicalVolume*   createDelrinBox();
    G4LogicalVolume*   createInterfacePad();
    G4LogicalVolume*   createSiPM();
    G4Material*        createBorosilicateGlass(const G4String name);
};
