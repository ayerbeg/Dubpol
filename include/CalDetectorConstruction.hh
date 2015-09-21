//
//
// $Id: Par01DetectorConstruction.hh 77659 2013-11-27 08:57:46Z gcosmo $
//
#ifndef CalDetectorConstruction_h
#define CalDetectorConstruction_h 1

#include "G4LogicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4ThreeVector.hh"

class CalDetectorConstruction
{

public:
  CalDetectorConstruction(G4VPhysicalVolume *physiParent);
  virtual ~CalDetectorConstruction();
  
public:
 
private:
  G4Material* polystyrene;
  G4Material* vaccuum;
  G4Material* iron;
  G4Material* SciTilesMaterial;


  G4int NoOfColumns;
  G4int NoOfRows;

  G4int  NoOfPairs;
  G4double  CalSizeX;
  G4double  CalSizeY;

  G4double  IronThickness;
  G4double  SciThickness;

  G4double  SizeZ; 
  
  G4Box*             CalBox;
  G4LogicalVolume*   CalLog;
  G4VPhysicalVolume* CalPhys;

 
  G4Box*             CalModBox;
  G4LogicalVolume*   CalModLog;
  G4VPhysicalVolume* CalModPhys;


  G4Box*             solidLayer;
  G4LogicalVolume*   logicLayer;
  G4VPhysicalVolume* physiLayer;

  G4Box*              solidIron;   
  G4LogicalVolume*    logicIron;
  G4VPhysicalVolume*  physiIron;
  

  G4Box*             solidSci;
  G4LogicalVolume*   logicSci;
  G4VPhysicalVolume* physiSci;
  
};


#endif

