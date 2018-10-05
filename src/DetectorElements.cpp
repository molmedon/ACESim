#include <G4Box.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>
#include <G4SubtractionSolid.hh>

#include "DetectorConstruction.hpp"

using namespace CLHEP;


// construct a complete ACE waveguide
auto DetectorConstruction::ConstructWaveguides(G4LogicalVolume* dewarChamber) -> void {

    // the dimensions of the waveguide
    G4double width{1.25*cm};  // x
    G4double length{12.5*cm}; // y
    G4double Cu_thickness{1.0*mm}; // z
    G4double Al_thickness{15.0*mm}; // z
    G4double air_gap{1.27*mm};
    G4double W_thickness{10*mm}; // currently unused

    // get copper, alumina, and tungsten alloy
    auto air{G4Material::GetMaterial("G4_AIR")};
    auto alumina{G4Material::GetMaterial("G4_ALUMINUM_OXIDE")};
    auto copper{G4Material::GetMaterial("G4_Cu")};
    auto tungsten{G4Material::GetMaterial("EF17tungstenAlloy")};

    ////////////////////////////////////////////////////////////
    // COPPER WAVEGUIDE
    // construct cobber waveguide - and hollow it out
    auto CuWaveguideSolidOuter{new G4Box("CopperWaveguideOuter",
                                    (width/2) + Cu_thickness,
                                    (length/2) + Cu_thickness,
                                    (Al_thickness/2) + Cu_thickness)};;
    auto CuWaveguideSolidInner{new G4Box("CopperWaveguideInner",
                                         (width/2),
                                         (length/2),
                                         (Al_thickness/2))};
    // and hollow out the waveguide
    auto CuWaveguideSolid{new G4SubtractionSolid("CopperWaveguideSolid",
                                                 CuWaveguideSolidOuter,
                                                 CuWaveguideSolidInner)};

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

    ////////////////////////////////////////////////////////////
    // create an assembly of everything
    G4ThreeVector location(0, 0, 0);
    auto rotation{new G4RotationMatrix(0, pi/2, 0)};

    // place the tungsten
    location.setY(-(2*air_gap + 2*Cu_thickness + 1.5*Al_thickness + (1.5*W_thickness)/2));
    new G4PVPlacement(rotation, location,
                      ThickTungstenBarLogic, "Tungsten", dewarChamber, false, 0, true);

    // and the first waveguide
    location.setY(air_gap + Cu_thickness + Al_thickness);
    new G4PVPlacement(rotation, location,
                      CuWaveguideLogic, "ACE1_Cu", dewarChamber, false, 1, true);
    new G4PVPlacement(rotation, location,
                      AluminaBarLogic, "ACE1", dewarChamber, false, 1, true);

    // create the central waveguide
    location.setY(0);
    new G4PVPlacement(rotation, location,
                      CuWaveguideLogic, "ACE2_Cu", dewarChamber, false, 2, true);
    new G4PVPlacement(rotation, location,
                      AluminaBarLogic, "ACE2", dewarChamber, false, 2, true);

    // and the last waveguide
    location.setY(-(air_gap + Cu_thickness + Al_thickness));
    new G4PVPlacement(rotation, location,
                      CuWaveguideLogic, "ACE3_Cu", dewarChamber, false, 3, true);
    new G4PVPlacement(rotation, location,
                      AluminaBarLogic, "ACE3", dewarChamber, false, 3, true);

}
