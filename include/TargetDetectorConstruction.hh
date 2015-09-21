//
// $Id: Par01DetectorConstruction.hh 77659 2013-11-27 08:57:46Z gcosmo $
//
#ifndef TargetDetectorConstruction_h
#define TargetDetectorConstruction_h 1

#include "G4LogicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4ThreeVector.hh"

class TargetDetectorConstruction
{

public:
  TargetDetectorConstruction(G4VPhysicalVolume *physiParent);
  virtual ~TargetDetectorConstruction();
  
public:
 
private:
  G4Material* polystyrene;
  G4Material* vaccuum;
  G4Material* SciTilesMaterial;

  G4int  NoOfSciTiles;

  G4double  shift;
  
  G4double  SciTilesSizeX;
  G4double  SciTilesSizeY;
  G4double  SciTilesThickness;
  G4double  SizeZ; 
  
  G4Box*             TargetBox;
  G4LogicalVolume*   TargetLog;
  G4VPhysicalVolume* TargetPhys;

 
  G4Box*             SciTilesSolid;
  G4LogicalVolume*   SciTilesLog;
  G4VPhysicalVolume* SciTilePhys;
};


#endif

