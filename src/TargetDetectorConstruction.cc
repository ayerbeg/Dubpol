//
/// \file analysis/shared/src/TargetDetectorConstruction.cc
/// \brief Implementation of the TargetDetectorConstruction class
//
//
// $Id: TargetDetectorConstruction.cc 85905 2014-11-06 08:51:55Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TargetDetectorConstruction.hh"
#include "Variables.hh"


#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ProductionCuts.hh"
#include "G4ElementTable.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4RegionStore.hh"
#include <cmath>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetDetectorConstruction::TargetDetectorConstruction(G4VPhysicalVolume *physiParent)
{ 

  G4cout<<"<TargetDetectorConstruction::TargetDetectorConstruction>: Entering..."<<G4endl; 
  
  //Some variables

  NoOfSciTiles = iVariables-> NoOfTiles;

  SciTilesSizeX = iVariables-> TilesSizeX;
  SciTilesSizeY = iVariables->TilesSizeY;
  SciTilesThickness = iVariables-> TilesSizeThick-0.5*um;

  SizeZ = SciTilesThickness * NoOfSciTiles; //only a row of tiles

  shift =  iVariables-> TargetShift;
  
  G4ThreeVector TargetPosZ(0, 0, (SizeZ/2- shift   ));

  //--------- Material definition ---------
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();
  
  // Build materials
  G4double density=0.509 *g/cm3;
  // G4Material* lH2_05 = new G4Material("lH2_0.5", density,"G4_lH2");//liquid H2 with a density equivalent to CH2 for number of nucleons

  G4Element* elH=new G4Element("Hydrogen","H",1.,1.01*g/mole); 

  G4Material* lH2_05 = new G4Material("lH2",density, 1);
  lH2_05->AddElement(elH,2); 

  //  G4cout<<lH2_05;
  
  //  polystyrene 
//= lH2_05;
    //     = nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");//i WAS USING POLYSTYRENE, I THINK THIS IS MORE ACCURATE   
      //= nistManager->FindOrBuildMaterial("G4_lH2");
//  = nistManager->FindOrBuildMaterial("G4_C");


  
  
  vaccuum    = nistManager->FindOrBuildMaterial("G4_Galactic");

  SciTilesMaterial = nistManager->FindOrBuildMaterial(iVariables-> TargetMat);


  G4cout<<G4endl<<"....oooOO0OOooo....................TARGET....................oooOO0OOooo......"<<G4endl<<G4endl

  G4cout<<SciTilesMaterial<<G4endl;
  G4cout<<"....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......"<<G4endl<<G4endl;

  
  //--------------------------
  // "Scintillator definition": used in
  // parameterisation below
  //--------------------------

  //
  //First a box. This is just a box to references the scintillators tiles
  // It is 1mm bigger just to avoid overlap with the within elements
  
  TargetBox
    = new G4Box("TargetSolid", SciTilesSizeX/2 +1*mm, SciTilesSizeY/2 +1*mm, SizeZ/2 +1*mm);
  
  // -- Logical volume:
  TargetLog 
    = new G4LogicalVolume(TargetBox,
			  vaccuum,
			  "TargetLogical", 0, 0, 0);
  // -- Placement:
  TargetPhys  
    = new G4PVPlacement(0,
			TargetPosZ,
			"TargetPhysical",
			TargetLog,
			physiParent,
			false,
			0);
  
  //--------------------------------------
  // The Target box is filled with
  // Scintillator tiles:
  //--------------------------------------

// -- Logical volume:
  SciTilesSolid = new G4Box("SciTilesSolid",SciTilesSizeX/2, SciTilesSizeY/2, SciTilesThickness/2);

  SciTilesLog = new G4LogicalVolume(SciTilesSolid, 
				    SciTilesMaterial,
				    "SciTileslLogical", 0, 0, 0);
  

  G4ThreeVector TargetOffset(0, 0, -((NoOfSciTiles-1)*SciTilesThickness)/2);//mainly to move the Target from the initial position   

  for (G4int i=0;i<NoOfSciTiles;i++)	      
    {
      SciTilePhys = new G4PVPlacement
	(0,		//no rotation
	 TargetOffset + G4ThreeVector(0, 0, i*SciTilesThickness),
	 "SciTile",	//its name
	 SciTilesLog,	//its logical volume
	 TargetPhys,       //its mother  
	 false,		//no boolean operation
	 i);	        //copy number
    }      

  //--------- Visualization attributes -------------------------------
   
  G4VisAttributes * calorimeterBoxVisAtt
    = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  calorimeterBoxVisAtt->SetForceWireframe(true);
  TargetLog->SetVisAttributes(calorimeterBoxVisAtt);
  
  G4VisAttributes * crystalVisAtt
    = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  crystalVisAtt->SetForceWireframe(true);
  SciTilesLog->SetVisAttributes(crystalVisAtt);
   
  //------------------------------------------------------------------
  
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetDetectorConstruction::~TargetDetectorConstruction()
{ 
 G4cout<<"<TargetDetectorConstruction::TargetDetectorConstruction>: Finished"<<G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


