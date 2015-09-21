//
/// \file analysis/shared/src/TargetDetectorConstruction.cc
/// \brief Implementation of the TargetDetectorConstruction class
//
//
// $Id: CalDetectorConstruction.cc 85905 2014-11-06 08:51:55Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "CalDetectorConstruction.hh"
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
#include "G4PVReplica.hh"
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

CalDetectorConstruction::CalDetectorConstruction(G4VPhysicalVolume *physiParent)
{ 

  G4cout<<"<CalDetectorConstruction::CalDetectorConstruction>: Entering..."<<G4endl; 
  
  //Some variables

  NoOfColumns = iVariables->NoOfCol;
  NoOfRows    = iVariables->NoOfRow;

  //The calorimeter tiles are 142x146 mm. For easyness we will consider 150x150mm
  CalSizeX = iVariables->ModuleSizeX;
  CalSizeY = iVariables->ModuleSizeY;

  IronThickness = iVariables->AbsThick;
  SciThickness  = iVariables->SciThick;

  NoOfPairs = iVariables->NoOfPa; //pair Iron-Scintillator

  SizeZ = (IronThickness+SciThickness) * NoOfPairs; //only a row of tiles


  //--------- Material definition ---------
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();
  
  // Build materials (using NIST database)

  polystyrene   = nistManager->FindOrBuildMaterial("G4_POLYSTYRENE");
  vaccuum       = nistManager->FindOrBuildMaterial("G4_Galactic");
  iron          = nistManager->FindOrBuildMaterial("G4_Fe");


  SciTilesMaterial = polystyrene;

  //--------------------------
  // "Calorimeter definition": used in
  // parameterisation below
  //--------------------------

  //
  //First a box. This is just a box to references the scintillators tiles
  // It is 1mm bigger just to avoid overlap with the within elements
  
  G4ThreeVector CalPos(-CalSizeX/2*(NoOfColumns-1), -CalSizeX/2*(NoOfColumns-1), 0);
  G4ThreeVector CalPosZ(0, 0, iVariables->CalShift+SizeZ/2);

  CalBox
    = new G4Box("CalSolid", 
		(CalSizeX/2*NoOfColumns) +1*mm, 
		(CalSizeY/2*NoOfRows) +1*mm, 
		SizeZ/2 +1*mm);
  
  // -- Logical volume:
  CalLog 
    = new G4LogicalVolume(CalBox,
			  vaccuum,
			  "CalLogical", 0, 0, 0);
  // -- Placement:
  CalPhys  
    = new G4PVPlacement(0,
			CalPosZ,
			"CalPhysical",
			CalLog,
			physiParent,
			false,
			0);
  

  //--------------------------
  //The modules of the calorimeter with the Calorimeter as parent 
  // because the world can not be used as slice 
  //--------------------------
  
  CalModBox = new G4Box("SolidCaloModul",		//its name
			   CalSizeX/2, 
			   CalSizeY/2, 
			   SizeZ/2);
  
  CalModLog = new G4LogicalVolume(CalModBox,	//its solid
				  iron,
				  "LogicCaloModul",0,0,0);	//its name
  
  
   
  for (G4int j=0;j<NoOfRows;j++)
    {
      for (G4int i=0;i<NoOfColumns;i++)	      
	{
	  CalModPhys = new G4PVPlacement (0,		//no rotation
					  CalPos+ G4ThreeVector((CalSizeX)*i,(CalSizeY)*j,0),	//at (0,0,0)
					  "CaloModul",	//its name
					  CalModLog,	//its logical volume
					  CalPhys,            //its mother volume
					  false,		//no boolean operation
					  (j*NoOfColumns)+i);	//copy number
	  
	  
	  //with this numeration units make columns, and tenths make rows
	  //e.g. with 15 columns 
	  //0-1-2-3-4-5-6-7-8-9-10-11-12-13-14... first row (i=0->14;j=0*columns)
	  //15-16-17-18-19-20-21-22-23-24-25-26-27-28-29... second row (i=1->14;j=1*colums)
	  
	  
	}
    }
  
  
  
  
  //--------------------------------------
  // The Target box is filled with
  // Iron+Sci tiles:
  //--------------------------------------
  
  //***************************************************************                                 
  // Layer
  //The calorimeter is formed by layers
  //each layer consists in absorber and gap
  //generally absorber=Pb and gap=scintillator
  //***************************************************************                                 
  
  solidLayer = new G4Box("SolidLayer",			//its name
			 CalSizeX/2-2*mm,
			 CalSizeY/2-2*mm, //size
			 (IronThickness+SciThickness)/2);
  
  logicLayer = new G4LogicalVolume(solidLayer,	//its solid
				   vaccuum,
				   "LogicLayer");	//its name
  
  physiLayer = new G4PVReplica("Layer",		//its name
			       logicLayer,	//its logical volume
			       CalModLog,	//its mother
			       kZAxis,		//axis of replication
			       NoOfPairs,	//number of replica
			       (IronThickness+SciThickness) );  //witdth of replica
 			       

  //***************************************************************                                 
  //Iron Tiles
  //***************************************************************
  
  solidIron = new G4Box("SolidIron",
			    CalSizeX/2-2*mm,
			    CalSizeY/2-2*mm,
			    IronThickness/2);
  
  logicIron = new G4LogicalVolume(solidIron,    //its solid
				      iron, //its material
				      "LogicIron"); //name
  

  physiIron = new G4PVPlacement(0,		   //no rotation
				    G4ThreeVector(0.,0.,-SciThickness/2),  //its position
				    logicIron,     //its logical volume		    
				    "Iron", //its name
				    logicLayer,        //its mother
				    false,             //no boulean operat
				    0);                //copy number
  

  //***************************************************************                                 
  // Scintillator Tiles
  //***************************************************************
    
  solidSci = new G4Box("SolidSci",
		       CalSizeX/2-2*mm,
		       CalSizeY/2-2*mm,
		       SciThickness/2);
  
	  
  logicSci = new G4LogicalVolume(solidSci,
				 SciTilesMaterial,
				 "LogicSci");
  
  physiSci = new G4PVPlacement(0,                      //no rotation
			       G4ThreeVector(0.,0.,IronThickness/2),   //its position
			       logicSci,               //its logical volume	       
			       "Sci", //its name
			       logicLayer,             //its mother
			       false,                  //no boulean operat
			       0);                     //copy number















  //--------- Visualization attributes -------------------------------
  G4VisAttributes * calorimeterBoxVisAtt
    = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  calorimeterBoxVisAtt->SetForceWireframe(true);
  CalModLog->SetVisAttributes(calorimeterBoxVisAtt);

  G4VisAttributes * crystalVisAtt
    = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  crystalVisAtt->SetForceWireframe(true);
  CalLog->SetVisAttributes(crystalVisAtt);
  
  G4VisAttributes * LayerVisAtt
    = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  LayerVisAtt->SetForceWireframe(true);
  logicLayer->SetVisAttributes(LayerVisAtt);
						

  G4VisAttributes * IronVisAtt
    = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  IronVisAtt->SetForceWireframe(true);
  logicIron->SetVisAttributes(IronVisAtt);

  G4VisAttributes * SciVisAtt
    = new G4VisAttributes(G4Colour(.0,.0,1.0));
  SciVisAtt->SetForceWireframe(true);
  logicSci->SetVisAttributes(SciVisAtt);


		 
  //------------------------------------------------------------------
				 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalDetectorConstruction::~CalDetectorConstruction()
{ 
 G4cout<<"<CalDetectorConstruction::CalDetectorConstruction>: Finished"<<G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


