//
/// \file analysis/shared/include/EventAction.hh
/// \brief Definition of the EventAction class
//
//
// $Id: EventAction.hh 68015 2013-03-13 13:27:27Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "HistoManager.hh"
#include "RunAction.hh"

class RunAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, HistoManager*);
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event*);
  virtual void    EndOfEventAction(const G4Event*);

    void EventAtCal(G4ThreeVector, G4int, G4int, G4double, G4double, G4double, G4double);  
    void AddTarget(G4double);
    void AddTargetTile(G4double, G4int);
    void AddSci(G4double);
    void AddSciMod(G4double, G4int);
    void AddGap(G4double , G4double );
    G4int GetEventNo();
private:
 
    RunAction*    fRunAct;
    HistoManager* fHistoManager;


    G4double  fEnergySci, fEnergyGap;
    G4double  fTrackLAbs, fTrackLGap;
 
    G4int     fPrintModulo;
    G4double  fEnergySciTarget;
    G4double  EnergyTargetTile[8];
    G4double  EnergyGapMod[25];
    G4int evtNb;                 

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
