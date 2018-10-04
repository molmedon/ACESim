#include <G4Cons.hh>
#include <G4Material.hh>
#include <G4AssemblyVolume.hh>

#include "DetectorConstruction.hpp"

using namespace CLHEP;


// construct the trigger system
auto DetectorConstruction::ConstructTriggerSystem() -> G4AssemblyVolume* {

    // the key dimensions of the trigger system
    auto disk_bottom_radius{4.5*cm};
    auto disk_top_radius{3.5*cm};
    auto disk_thickness{1*cm};

    // get glass - currently this is borosilicate
    auto glass{G4Material::GetMaterial("BorosilicateGlass")};

    // create the glass disk
    auto disk{new G4Cons("Disk", 0., disk_bottom_radius, // min and max bottom radius
                         0., disk_top_radius, // min and max top radius
                         disk_thickness, // thickness
                         0, 360*degree)}; // a complete revolution

    // we create the logic volume
    auto diskLV{new G4LogicalVolume(disk, glass, "GlassDisk")};

    // sontruct an assembly volume for the whole trigger system
    auto assembly{new G4AssemblyVolume};

    // construct the remaining elements HERE....

    // the origin of our assembly
    G4ThreeVector location(0, 0, 0);

    // and place the glass disk
    assembly->AddPlacedVolume(diskLV, location, new G4RotationMatrix(0, 0, 0));

    // and add them to the assembly HERE...


    // and we have built a trigger system!
    return assembly;

}
