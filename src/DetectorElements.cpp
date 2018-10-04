#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>

#include "DetectorConstruction.hpp"

using namespace CLHEP;


// construct a complete ACE waveguide
auto DetectorConstruction::ConstructWaveguides() -> G4AssemblyVolume* {

    // the dimensions of the waveguide
    G4double width{1.25*cm};  // x
    G4double length{12.5*cm}; // y
    G4double Cu_thickness{1.0*mm}; // z
    G4double Al_thickness{15.0*mm}; // z
    G4double air_gap{66.5*mm};
    G4double W_thickness{10*mm};

    // get copper, alumina, and tungsten alloy
    auto alumina{G4Material::GetMaterial("G4_ALUMINUM_OXIDE")};
    auto copper{G4Material::GetMaterial("G4_Cu")};
    auto tungsten{G4Material::GetMaterial("EF17tungstenAlloy")};

    ////////////////////////////////////////////////////////////
    // COPPER WAVEGUIDE
    // construct cobber waveguide - this is solid because
    // we later fill it with alumina
    auto CuWaveguideSolid{new G4Box("CopperWaveguide",
                                    (width + Cu_thickness)/2,
                                    (length + Cu_thickness)/2,
                                    (Al_thickness/2) + Cu_thickness)};;

    // construct the logical waveguide volume
    auto CuWaveguideLogic{new G4LogicalVolume(CuWaveguideSolid,
                                              copper, "CopperWaveguide")};

    ////////////////////////////////////////////////////////////
    // ALUMINA BLOCK

    // construct alumina block inside waveguide
    auto AluminaBarSolid{new G4Box("AluminaBar",
                                width/2, length/2, Al_thickness/2)};

    // construct the logical alumina block
    auto AluminaBarLogic{new G4LogicalVolume(AluminaBarSolid,
                                               alumina, "AluminaBar")};

    ////////////////////////////////////////////////////////////
    // TUNGSTEN BAR

    // construct alumina block inside waveguide
    auto TungstenBarSolid{new G4Box("TungstenBar",
                                width/2, length/2, W_thickness/2)};

    // construct the logical alumina block
    auto TungstenBarLogic{new G4LogicalVolume(TungstenBarSolid,
                                               tungsten,"TungstenBar")};

    ////////////////////////////////////////////////////////////
    // FIRST TUNGSTEN BAR

    // construct alumina block inside waveguide
    auto ThickTungstenBarSolid{new G4Box("ThickTungstenBar",
                                    width/2, length/2, (1.5*W_thickness)/2)};

    // construct the logical alumina block
    auto ThickTungstenBarLogic{new G4LogicalVolume(ThickTungstenBarSolid,
                                               tungsten,"ThickTungstenBar")};

    // we create an assesmbled waveguide
    auto waveguide{new G4AssemblyVolume};

    // we have to specify placement and rotation matrices for assembling the waveguide
    G4ThreeVector location(0, 0, 0);
    // geant4 stupidly only takes pointers to rotation matrices
    G4RotationMatrix* rotation{new G4RotationMatrix(0, 0, 0)};

    // construct the first tungsten bar
    location.setZ(air_gap + 2*Cu_thickness + Al_thickness + (1.5*W_thickness)/2);
    waveguide->AddPlacedVolume(ThickTungstenBarLogic, location, rotation);;

    // construct the first waveguide
    location.setZ(air_gap + Cu_thickness + Al_thickness/2);
    waveguide->AddPlacedVolume(CuWaveguideLogic, location, rotation);
    waveguide->AddPlacedVolume(AluminaBarLogic, location, rotation);

    // and construct the third tungsten block
    location.setZ(Cu_thickness + W_thickness/2 + Al_thickness/2.);
    waveguide->AddPlacedVolume(TungstenBarLogic, location, rotation);

    // construct a waveguide at the center
    location.setZ(0);
    waveguide->AddPlacedVolume(CuWaveguideLogic, location, rotation);
    waveguide->AddPlacedVolume(AluminaBarLogic, location, rotation);

    // and construct the third tungsten block
    location.setZ(-(air_gap + 2*Cu_thickness + Al_thickness + W_thickness/2.));
    waveguide->AddPlacedVolume(TungstenBarLogic, location, rotation);

    // and the element assembly
    location.setZ(-(air_gap + Cu_thickness + Al_thickness/2));
    waveguide->AddPlacedVolume(CuWaveguideLogic, location, rotation);
    waveguide->AddPlacedVolume(AluminaBarLogic, location, rotation);

    // and return it so that it can be placed in the world
    return waveguide;

}
