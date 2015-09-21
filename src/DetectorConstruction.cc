//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//
/// \file analysis/shared/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
// $Id: DetectorConstruction.cc 85905 2014-11-06 08:51:55Z gcosmo $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"


#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ProductionCuts.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4RegionStore.hh"
#include <cmath>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{ 
  G4cout<<"<DetectorConstruction::DetectorConstruction>: Entering..."<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  G4cout<<"<DetectorConstruction::DetectorConstruction>: Finished"<<G4endl;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* DetectorConstruction::Construct()
{

  //--------- Material definition ---------
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();
  
  // Build materials
  G4Material* air    = nistManager->FindOrBuildMaterial("G4_AIR");
  G4Material* vaccuum    = nistManager->FindOrBuildMaterial("G4_Galactic");

  G4cout << "DetectorConstruction: Building World...." << G4endl;  
  
  //--------------
  // World:
  //--------------
  G4Box *WorldBox= new G4Box("WorldBox",800*cm, 800*cm, 800*cm);
  G4LogicalVolume *WorldLog=new G4LogicalVolume(WorldBox,vaccuum,
                                                "WorldLogical", 0, 0, 0);
  G4PVPlacement *WorldPhys=new G4PVPlacement(0,G4ThreeVector(),
                                             "WorldPhysical",
					     WorldLog,
                                             0,false,0);
  //-----------------------------
  
  //Every element is bult separated for cleanness
 Target = new TargetDetectorConstruction(WorldPhys);
 Calorimeter = new CalDetectorConstruction(WorldPhys);

   
  //-----------------------
  // Returns the pointer to
  // the physical world:
  //-----------------------
  return WorldPhys;
}


void DetectorConstruction::ConstructSDandField()
{/*
  //--------- Sensitive detector -------------------------------------
 
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String calorimeterSDname = "Par01/Calorimeter";
  Par01CalorimeterSD* CalorimeterSD = new Par01CalorimeterSD( calorimeterSDname,
                                                              fnX*fnY,
                                                              "CalCollection" );
  SDman->AddNewDetector( CalorimeterSD );
  fCrystalLog->SetSensitiveDetector(CalorimeterSD); 
   
  G4String hadCalorimeterSDname = "Par01/HadronCalorimeter";
  Par01CalorimeterSD* HadCalorimeterSD = new Par01CalorimeterSD( hadCalorimeterSDname,
                                                                 fnXhad*fnYhad,
                                                                 "HadCollection" );
  SDman->AddNewDetector( HadCalorimeterSD );
  fTowerLog->SetSensitiveDetector(HadCalorimeterSD); 
  
  // --------------- fast simulation ----------------------------
  G4RegionStore* regionStore = G4RegionStore::GetInstance();
   
  G4Region* caloRegion = regionStore->GetRegion("EM_calo_region");
  // builds a model and sets it to the envelope of the calorimeter:
  new Par01EMShowerModel("emShowerModel",caloRegion);
 */   
}

