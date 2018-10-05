#include <G4Cons.hh>
#include <G4Tubs.hh>
#include <G4Orb.hh>
#include <G4Material.hh>
#include <G4AssemblyVolume.hh>

#include "DetectorConstruction.hpp"

using namespace CLHEP;


// construct the trigger system
auto DetectorConstruction::ConstructTriggerSystem() -> G4AssemblyVolume* {

    // the key dimensions of the trigger system
    auto disk_bottom_radius{3.81*cm};
    auto disk_top_radius{3.25*cm};
    auto disk_thickness{0.3*cm};

    // get glass - currently this is borosilicate
    auto glass{G4Material::GetMaterial("BorosilicateGlass")};

    //get air 
    auto air{G4Material::GetMaterial("G4_AIR")};
  
    //get delrin (should be nylon I guess)
    auto delrin{G4Material::GetMaterial("Delrin", 1.41*g/cm3)}; 


    // create the glass disk
    auto disk{new G4Cons("Disk", 0., disk_bottom_radius, // min and max bottom radius
                         0., disk_top_radius, // min and max top radius
                         disk_thickness, // thickness
                         0, 360*degree)}; // a complete revolution

    //create the delrin cratle
//    auto cradle{new G4Cons("Cradle", 0.*mm, 61.25*mm, 0.*mm, 34.81*mm, 15.875*mm, 0, 360*degree)}; 
//    auto cradle{new G4Cons("Cradle", 0.*mm, 61.25*mm, 0.*mm, 37.6795*mm, 15.875*mm, 0, 360*degree)}; 
    auto cradle{new G4Cons("Cradle", 0.*mm, 64.25*mm, 0.*mm, 37.6795*mm, 14.6375*mm, 0, 360*degree)}; 

    //create the air cradle 
    auto airCradle{new G4Cons("airCradle", 0.*mm, 41.00*mm, 0.*mm, 30.1073*mm, 6.1821*mm, 0, 360*degree)};

    //create the air cylinder
    auto airCylinder{new G4Tubs("airCylinder", 0., 5.01*mm, 5.8065*mm, 0, 360*degree)};//(name,inner rad, outer rad, length/2, phi0, span)

    //create glass sphere
    auto glassSphere{new G4Orb("glassSphere", 5.0*mm)};

    //we create the logic volume
    auto diskLV{new G4LogicalVolume(disk, glass, "GlassDisk")};

    auto cradleLV{new G4LogicalVolume(cradle, delrin, "delCradle")};

    auto cradleAirLV{new G4LogicalVolume(airCradle, delrin, "airCradle")};

    auto cylLV{new G4LogicalVolume(airCylinder, air, "airCyl")};

    auto sphereLV{new G4LogicalVolume(glassSphere, glass, "glassSph")};

    // sontruct an assembly volume for the whole trigger system
    auto assembly{new G4AssemblyVolume};

    // construct the remaining elements HERE....

    // the origin of our assembly
    G4ThreeVector location(0, 0, 0);

    //place delrin craddle
    assembly->AddPlacedVolume(cradleLV, location, new G4RotationMatrix(0, 0, 0));

    //place air cradle
    assembly->AddPlacedVolume(cradleAirLV, location, new G4RotationMatrix(0, 0, 0));

    //place the glass disk
    assembly->AddPlacedVolume(diskLV, location, new G4RotationMatrix(0, 0, 0));

    //place delrin craddle
    assembly->AddPlacedVolume(cradleLV, location, new G4RotationMatrix(0, 0, 0));

    //place air cylinder
    location.setX(39.0*mm);
    location.setZ(4.50*mm);
    assembly->AddPlacedVolume(cylLV, location, new G4RotationMatrix(45*deg, 47.00*deg, 90*deg));

    //place glass sphere
    location.setX(39.0*mm);
    location.setZ(4.50*mm);    
    assembly->AddPlacedVolume(sphereLV, location, new G4RotationMatrix(0, 0, 0));

    // and add them to the assembly HERE...


    // and we have built a trigger system!
    return assembly;

}
