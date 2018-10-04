#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4Element.hh>
#include <G4Material.hh>
#include <G4VisExtent.hh>
#include <G4SDManager.hh>
#include <G4UnitsTable.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4PSNofSecondary.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VPrimitiveScorer.hh>
#include <G4SDParticleFilter.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4MultiFunctionalDetector.hh>

#include "MPPC.hpp"
#include "DetectorConstruction.hpp"

using namespace CLHEP;

// span
G4double energySpan[] = {(1240.7*eV)/320.,
                         (1240.7*eV)/340., (1240.7*eV)/360., (1240.7*eV)/380.,
                         (1240.7*eV)/400., (1240.7*eV)/420.,
                         (1240.7*eV)/440., (1240.7*eV)/460., (1240.7*eV)/480.,
                         (1240.7*eV)/500., (1240.7*eV)/520.,
                         (1240.7*eV)/540., (1240.7*eV)/560., (1240.7*eV)/580.,
                         (1240.7*eV)/600., (1240.7*eV)/620.,
                         (1240.7*eV)/640., (1240.7*eV)/660., (1240.7*eV)/680.,
                         (1240.7*eV)/700., (1240.7*eV)/720.,
                         (1240.7*eV)/740., (1240.7*eV)/760., (1240.7*eV)/780.,
                         (1240.7*eV)/800., (1240.7*eV)/820.,
                         (1240.7*eV)/840., (1240.7*eV)/860., (1240.7*eV)/880.,
                         (1240.7*eV)/900., (1240.7*eV)/920.,
                         (1240.7*eV)/940., (1240.7*eV)/960., (1240.7*eV)/980.,
                         (1240.7*eV)/1000.}; // in eV

// min and max energy values
G4double energyRange[] = {energySpan[0], energySpan[34]};

// construct the geometry
auto DetectorConstruction::Construct() -> G4VPhysicalVolume* {

    // create logical world volume
    auto logicWorld{this->CreateWorld()};

    // create physical world
    auto physWorld{new G4PVPlacement(0, // no rotation,
                                     G4ThreeVector(0, 0, 0), // location
                                     logicWorld,
                                     "World",
                                     0, // no mother volume
                                     false, 0, true)};

    // we want to move the elements and the dewer 1/4 of the way
    // to the edge of the world volume, so we need the dimensions
    // of the worldvolume
    auto worldZ{logicWorld->GetSolid()->GetExtent().GetZmax()};

    // construct the ACE assembly
    auto elements{this->ConstructWaveguides()};

    // // and place it at the origin
    G4ThreeVector origin(0, 0, -worldZ/2.);

    // and place it!
    elements->MakeImprint(logicWorld, origin, new G4RotationMatrix(0, 0, 0));

    // construct the ACE assembly
    auto dewer{this->ConstructDewer()};

    // and place it!
    dewer->MakeImprint(logicWorld, origin, new G4RotationMatrix(0, 0, 0));

    // we construct the trigger system
    auto trigger{this->ConstructTriggerSystem()};

    // and place it
    origin.setZ(worldZ/2);
    trigger->MakeImprint(logicWorld, origin, new G4RotationMatrix(0, 0, 0));

    // return the physical world
    return physWorld;

}

// construct all sensitive detectors and fields
auto DetectorConstruction::ConstructSDandField() -> void {


}

// create and return a G4LogicalVolume representing the world
auto DetectorConstruction::CreateWorld() -> G4LogicalVolume* {

    // the dimensions of the world
    auto worldX{25*cm};
    auto worldY{50*cm};
    auto worldZ{50*cm};

    // create solid world
    auto solidWorld{new G4Box("World", worldX, worldY, worldZ)};

    // get NIST manager
    auto manager{G4NistManager::Instance()};

    // get air - defined already
    auto air{manager->FindOrBuildMaterial("G4_AIR")};

    // // create logical world volume
    auto logicWorld(new G4LogicalVolume(solidWorld, air, "logicWorld"));

    // return the logic world
    return logicWorld;

}

// construct the dewer
auto DetectorConstruction::ConstructDewer() -> G4AssemblyVolume* {

    // set these properties to adjust dewer dimensions
    auto outer_radius{200*mm}; // outer radius of dewer
    auto wall_thickness{3*mm}; // wall thickness
    auto inner_radius{150*mm}; // radius of dewer's inner compartment
    auto height{40*cm}; // half heigh of the dewer

    // we get the materials we neeed
    auto lHe2{G4Material::GetMaterial("liquidHelium")};
    auto vacuum{G4Material::GetMaterial("Galactic")};
    auto steel{G4Material::GetMaterial("G4_STAINLESS-STEEL")};

    // construct the main dewer cylinder
    auto dewerSolid{new G4Tubs("Dewer", outer_radius - wall_thickness, outer_radius,
                               height, 0, 360*degree)};

    // and its logical volume
    auto dewerLV{new G4LogicalVolume(dewerSolid, steel, "Dewer")};

    // construct the inner cylinder of vacuum
    auto vacuumSolid{new G4Tubs("DewerVacuum", inner_radius, outer_radius - wall_thickness,
                                height, 0, 360*degree)};

    // and the logical volume
    auto vacuumLV{new G4LogicalVolume(vacuumSolid, vacuum, "DewerVacuum")};

    // and construct the inner dewer cylinder
    auto innerSolid{new G4Tubs("DewerInnerCyl", inner_radius - wall_thickness, inner_radius,
                               height, 0, 360*degree)};

    // and the logical volume
    auto innerLV{new G4LogicalVolume(innerSolid, steel, "DewerInnerCylinder")};

    // and the inner LHe2 chamber
    auto cavitySolid{new G4Tubs("DewerCavity", 0, inner_radius - wall_thickness,
                                height, 0, 360*degree)};

    // and the logical volume
    auto cavityLV{new G4LogicalVolume(cavitySolid, lHe2, "DewerCavity")};

    // create an assembly of them all
    auto dewer{new G4AssemblyVolume};

    // we have to specify placement and rotation matrices for assembling the waveguide
    G4ThreeVector location(0, 0, 0);
    // geant4 stupidly only takes pointers to rotation matrices
    G4RotationMatrix* rotation{new G4RotationMatrix(0, pi/2., 0)};

    // and construct all the elements
    dewer->AddPlacedVolume(dewerLV, location, rotation);;
    dewer->AddPlacedVolume(vacuumLV, location, rotation);;
    dewer->AddPlacedVolume(innerLV, location, rotation);;
    dewer->AddPlacedVolume(cavityLV, location, rotation);;

    // and we are done
    return dewer;


}

// define all the materials we will use in building the world
auto DetectorConstruction::DefineMaterials() const -> void {

    // get the NIST manager
    auto nistManager{G4NistManager::Instance()};

    // Iron material defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_Fe");

    // Nickel material defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_Ni");

    // Tungsten material defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_W");

    // Aluminum Oxide material defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");

    // Copper material defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_Cu");

    // liquid N2 material defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_lN2");

    // liquid N2 material defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    //////////////////////
    // AIR
    auto air{nistManager->FindOrBuildMaterial("G4_AIR")};
    auto MPT{new G4MaterialPropertiesTable()};
    G4double RINDEX[] = {1.000277, 1.000277};
    MPT->AddProperty("RINDEX", energyRange, RINDEX, 2)->SetSpline(true);
    air->SetMaterialPropertiesTable(MPT);
    //////////////////////

    // iron element
    auto elFe{new G4Element("Iron", "Fe", 26., 55.85*g/mole)};

    // tungsten element
    auto elW{new G4Element("Tungsten", "W", 74., 183.84*g/mole)};

    // nickel element
    auto elNi{new G4Element("Nickel", "Ni", 28., 58.6934*g/mole)};

    // oxygen element
    auto elO = new G4Element("Oxygen", "O", 8, 16.00*g/mole);

    // boron element
    auto elB = new G4Element("Boron", "B", 5, 10.8*g/mole);

    // silicon element
    auto elSi = new G4Element("Silicon", "Si", 14, 28.09*g/mole);

    // sodium element
    auto elNa = new G4Element("Sodium", "Na", 11, 22.99*g/mole);

    // aluminium element
    auto elAl = new G4Element("Aluminium", "Al", 13, 26.98*g/mole);

    // liquid argon
    new G4Material("liquidArgon", 18., 39.95*g/mole, 1.390*g/cm3,
                   kStateLiquid, 87.3*kelvin);

    // liquid helium
    new G4Material("liquidHelium", 2, 4.00260*g/mole, 1.390*g/cm3,
                   kStateLiquid, 3.2*kelvin);

    // Vacuum
    new G4Material("Galactic", 1., 1.01*g/mole, universe_mean_density,
                   kStateGas, 2.73*kelvin, 3.e-18*pascal);

    // construct EF17tungstenAlloy
    auto Walloy{new G4Material("EF17tungstenAlloy", 17.0*g/cm3, 3)};
    Walloy->AddElement(elW, 90*perCent);
    Walloy->AddElement(elFe, 3*perCent);
    Walloy->AddElement(elNi, 7*perCent);

    //////////////////////
    // BOROSILICATE GLASS

    // construct a borosilicate glass
    auto borosilicateGlass{new G4Material("BorosilicateGlass", 2.23*g/cm3, 5)};
    borosilicateGlass->AddElement(elO, 54.0*perCent);
    borosilicateGlass->AddElement(elB, 4.0*perCent);
    borosilicateGlass->AddElement(elSi, 38.0*perCent);
    borosilicateGlass->AddElement(elNa, 3*perCent);
    borosilicateGlass->AddElement(elAl, 1*perCent);

    // create material properies table
    auto gMPT{new G4MaterialPropertiesTable()};

    // constant refractive index across the spectrum
    G4double gRINDEX[] = {1.51, 1.51};

    // absorption length att all frequencies
    G4double gABSLENGTH[] = {5.081*mm, 19.574*mm, 43.806*mm, 50.513*mm, 60.779*mm,
                            66.472*mm, 64.247*mm, 62.500*mm, 64.016*mm, 66.725*mm,
                            68.512*mm, 69.094*mm, 68.833*mm, 68.171*mm, 67.544*mm,
                            67.276*mm, 67.335*mm, 67.613*mm, 67.999*mm, 68.385*mm,
                            68.677*mm, 68.880*mm, 69.023*mm, 69.132*mm, 69.247*mm,
                            69.382*mm, 69.546*mm, 69.730*mm, 69.925*mm, 70.126*mm,
                            70.334*mm, 70.541*mm, 70.746*mm, 70.957*mm, 71.169*mm};

    // and assign the properties to the borosilicate glass
    gMPT->AddProperty("RINDEX", energyRange, gRINDEX, 2)->SetSpline(true);
    gMPT->AddProperty("ABSLENGTH", energySpan, gABSLENGTH, 35)->SetSpline(true);

    borosilicateGlass->SetMaterialPropertiesTable(MPT);;

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}
