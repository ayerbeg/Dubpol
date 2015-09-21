//
/// \file analysis/shared/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
//
// $Id: EventAction.cc 68015 2013-03-13 13:27:27Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"
#include "G4Event.hh"
#include <CLHEP/Units/SystemOfUnits.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run, HistoManager* histo)
:G4UserEventAction(),
 fRunAct(run),fHistoManager(histo),
 fEnergySci(0.), fEnergyGap(0.),
 fTrackLAbs(0.), fTrackLGap(0.),
 fPrintModulo(0),fEnergySciTarget(0)
{
 fPrintModulo = 100; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
    evtNb = evt->GetEventID();
    
    if (evtNb%fPrintModulo == 0) 
	G4cout << "\n---> Begin of event: " << evtNb << G4endl;
    
    // initialisation per event
    fEnergySci = fEnergyGap = 0.;
    fTrackLAbs = fTrackLGap = 0.;
    fEnergySciTarget = 0;

    for(G4int i=0;i<8;i++)  EnergyTargetTile[i]=0.;    
    for(G4int i=0;i<25;i++) EnergyGapMod[i]=0.;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  //accumulates statistic
  //
    
//*******************************************
//HERE ARE FILLED THE VARIABLES OF TARGET
    fHistoManager->FillHisto(2, fEnergySciTarget);

    for (G4int j=0; j<8;j++)
    {
	fHistoManager->FillTargetTileNtuple(evtNb, EnergyTargetTile[j], j);
    }
//*******************************************


    if(fEnergySci>0)
    {
	fRunAct->fillPerEvent(fEnergySci, fEnergyGap, fTrackLAbs, fTrackLGap);
	
	//fill histograms
	//
	fHistoManager->FillHisto(1, fEnergySci);

	fHistoManager->FillHisto(3, fTrackLAbs);
	fHistoManager->FillHisto(4, fTrackLGap);
	
	//fill ntuple
  //
	for (G4int i=0; i<25;i++)
	{
	    fHistoManager->FillEventCalMod(evtNb, i, EnergyGapMod[i]);
	}



    }
}

  
void EventAction::EventAtCal(G4ThreeVector Position, G4int cPDGID, G4int cpID, G4double ckineEne, G4double cTotalEne, G4double cTheta, G4double cPhi)
{
    fHistoManager->FillCalEventNtuple(evtNb, Position, cPDGID, cpID,ckineEne, cTotalEne, cTheta, cPhi);

//    G4cout<<"Pos: "<<Position/CLHEP::mm<<" PDGid: "<<cPDGID<<" ckineEne: "<<ckineEne/CLHEP::MeV<<" cTotalEne: "<<cTotalEne/CLHEP::MeV<<" cTheta: "<<cTheta/CLHEP::deg<<" Phi: "<<cPhi/CLHEP::deg<<G4endl;
}

void EventAction::AddTarget(G4double de) 
{
    fEnergySciTarget += de; 
}

void EventAction::AddTargetTile(G4double de, G4int mod) 
{
    EnergyTargetTile[mod] += de;
}

void EventAction::AddSci(G4double de) 
{
    fEnergySci += de; 
}

void EventAction::AddGap(G4double de, G4double dl) 
{
    fEnergyGap += de; fTrackLGap += dl;
}

void EventAction::AddSciMod(G4double de, G4int mod) 
{
    EnergyGapMod[mod] += de;
}

G4int EventAction::GetEventNo()
{
  return evtNb;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
