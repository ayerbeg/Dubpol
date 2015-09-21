//
/// \file analysis/shared/include/SteppingAction.hh
/// \brief Definition of the SteppingAction class
//
//
// $Id: SteppingAction.hh 68015 2013-03-13 13:27:27Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SteppingAction_h
#define SteppingAction_h 1
#include "HistoManager.hh"
#include "G4UserSteppingAction.hh"

#include "EventAction.hh"

class DetectorConstruction;
class EventAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction*, HistoManager*);
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step*);
    void UserTargetAnalysis(const G4Step*);
    void UserCalAnalysis(const G4Step*);
private:
    DetectorConstruction* fDetector;
    HistoManager*         fHistoManager;	 
    EventAction*          fEventAction;  

    
    
    G4bool Entry(G4String Volumen); // True if particle enters volume with name "Volumen"
    G4bool Exit(G4String Volumen); // True if particle leaves volume with name "Volumen"
    
//Target Variables
    G4double edep;
    G4int tevtNo;
    G4int tPDGID;
    G4int tpID;  
    G4int SciTileNo;
    G4int tevtNb;
   
    G4double EnergyTarget, EnergyTemp;
    G4double EnergySciTile[8];

    G4int Tile;
    G4double EDepTile;
    G4int copyNo ;
    G4int fEventNo, fTileNo, fPDGid ;
    G4double  fEnergyPerStep;
    G4double fkineEne;
    G4double    fTheta;
    G4double    fPhi;

//Calorimeter Variables

//per event (the c is for calorimeter)
    G4ThreeVector Position;
    G4int      cPDGID;
    G4int      cpID;
    G4double ckineEne;
    G4double cTotalEne;
    G4double cTheta;
    G4double cPhi;

    G4double x,y,z;
    G4String Process;
    G4int evTemp;
    G4String ActualVolume, NextVolume;

    G4double EnergyGap;
    G4double EnergyGapMod[25];  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
