//
/// \file analysis/shared/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
// $Id: SteppingAction.cc 85905 2014-11-06 08:51:55Z gcosmo $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* evt, HistoManager* histo): G4UserSteppingAction(),
								       fHistoManager(histo),
								       fEventAction(evt)
{ 
    EnergyGap=0;
    for(G4int i=0;i<25;i++) EnergyGapMod[i]=0;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  //  G4cout<<"event: "<<evtNo<<G4endl;

  G4Track *actualTrack = aStep->GetTrack();
  G4StepPoint* point = aStep->GetPreStepPoint();
  G4TouchableHandle touch = point->GetTouchableHandle();
  
  // get volume of the current step
   G4String volume 
  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
 
  //  G4cout<<"I'm in: "<<volume<<G4endl;

  NextVolume = "NULL";//Due to the definition of NextVolume inside an 'if' some value must be give to it

  ActualVolume = actualTrack->GetVolume()->GetName();
  if (actualTrack->GetNextVolume())
    {	   
      NextVolume = actualTrack->GetNextVolume()->GetName();
    }
  
  G4double kineticEnergy = actualTrack->GetKineticEnergy();  
  if(kineticEnergy<300.*CLHEP::keV)
    {
//	G4cout<<"Killed!! Energy less than: "<<kineticEnergy<<G4endl;
//	actualTrack->SetTrackStatus(fStopAndKill); //kill particles with kinEne smaller than certain value
    }

  if(
     (Exit("WorldPhysical") && Entry("TargetPhysical"))|| //entering Target box
     (NextVolume=="SciTile")||
     (Exit("SciTile") && Entry("TargetPhysical"))
     )//keeping the particle inside the Target
    {
//      G4cout<<"Going to TargetAnalysis::TargetAnalysis()"<<G4endl;
      UserTargetAnalysis(aStep);
    }

  if(
      (Exit("WorldPhysical") && Entry("CalPhysical"))
/* || //entering Calorimeter box
     NextVolume=="CaloModul"||
     NextVolume=="Layer"||
     NextVolume=="Iron"||
     NextVolume=="Sci"*/
      ) //we guarantee that particle is in the calorimeter
    {
	UserCalAnalysis(aStep);

//   actualTrack->SetTrackStatus(fStopAndKill);   //go to Cal Analysis
    }

  if (Exit("CalPhysical"))// && Entry("WorldPhysical") )
    {
//      G4cout<<"Particle leaving the calorimeter. KILLED!!"<<G4endl;
     actualTrack->SetTrackStatus(fStopAndKill);
    }


}


void SteppingAction::UserTargetAnalysis(const G4Step* aStep)
{
    
//  G4cout<<"SteppingAction::UserTargetAnalysis"<<G4endl;
  
  G4Track *actualTrack = aStep->GetTrack();
  
  //It is always useful keep control of the next volume
  //in case it is not defined (out of World)
  
  ActualVolume = actualTrack->GetVolume()->GetName();
  
  G4String nameParticle = actualTrack->GetDefinition()->GetParticleName();
  
  if (actualTrack->GetNextVolume())
    {	   
      NextVolume = actualTrack->GetNextVolume()->GetName();
    }
  
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
  G4VPhysicalVolume* theVolume =theTouchable->GetVolume();
  G4String nameVolume = theTouchable->GetVolume()->GetName();
  
  G4int historyDepth = theTouchable->GetHistoryDepth();
  
  G4String MotherName;
//  G4int ParID = actualTrack->GetParentID();  

    
  //**************************
  //Instructions to kill processes out of elastic or transport WITHIN target
  //**************************
  Process = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  
//  G4cout<<"Process: "<<Process<<G4endl;
  
/*
  if( (ParID<2)&&( (nameParticle=="neutron")||(nameParticle=="proton")) )//restrictions for primary neutrons (ParId =0) or secondary protons (ParId=1)
    {
      if ( ((Process=="hadElastic")||
	    (Process=="Transportation"))&&
	   (nameParticle=="neutron")
	   )
	{
//	  G4cout<<"TargetAnalysis::UserTargetAnalysisI'm IN (1st IF)"<<G4endl;
	}
      else
	{
	  if ( 
	      (nameParticle=="proton")
	       )
	    {
//	      G4cout<<"I'm IN (2nd IF)  "<<nameParticle<<G4endl;
	    }
	  else
	    {
//	      G4cout<<"KILLED!!!! by procesess   "<<nameParticle<<G4endl;
	      actualTrack->SetTrackStatus(fKillTrackAndSecondaries);
	      //restrict process
	    }
	}
    }
*/
  
  //**************************
  
  if(historyDepth==2)//World=0, TargetPhysical=1, SciTile=2 

    //**Instrucctions indicates there is a better way, but doesn't work**
    //** Segmentation Fault ** 
    {
      copyNo =  theVolume->GetCopyNo(); 
      G4ThreeVector pos1 = preStepPoint->GetPosition();
    }
  

  if (nameVolume=="SciTile" )
    {
	edep = aStep->GetTotalEnergyDeposit();
//	if (edep>300.*CLHEP::keV)//only store data if energy is deposited
	                         //Note that there are a condition to kill particles
	                         //with KinEne smaller than 300keV, but it happen after the 
                                 //energy deposition. So here we control, no data is stored 
                                 //with such energy

	{
	    SciTileNo = copyNo;
	    EnergySciTile[copyNo] += edep;//per tile

	    fEventAction->AddTarget(edep);
	    fEventAction->AddTargetTile(edep,SciTileNo);

	    tevtNb =  fEventAction->GetEventNo();

	    // G4cout<<"event numeber read: "<<tevtNb<<G4endl;
	    // G4cout<<"MotherName (inside if): "<<nameVolume<<" copyNo: "<< SciTileNo<<G4endl;
//	    G4cout<<"Particle to NTuple: "<<  actualTrack->GetDefinition()->  GetParticleName()<<G4endl;

	    tPDGID = actualTrack->GetDefinition()-> GetPDGEncoding();

	    if(tPDGID==12||tPDGID==14||tPDGID==22)//Removing gammas and neutrinos. Perhaps e's too?
	    {
		//	G4cout<<actualTrack->GetDefinition()->GetParticleName()<<" I'M DROPPING ENERGY"<<G4endl;
	    }
	    else
	    {
	    tpID = actualTrack->GetParentID();
	    fEventNo = tevtNb; 
	    fEnergyPerStep = edep; 
	    fkineEne =actualTrack-> GetKineticEnergy();
	    fTileNo = SciTileNo;
	    fPDGid = tPDGID; 
	    fTheta = actualTrack-> GetMomentum().theta();
	    fPhi = actualTrack-> GetMomentum().phi();	    

	    fHistoManager->FillTargetNtuple(fEventNo, fEnergyPerStep, fkineEne, fTileNo, fPDGid, tpID, fTheta, fPhi);
	    }	

	}

    }
  
  
}



void SteppingAction::UserCalAnalysis(const G4Step* aStep)
{
//  G4cout<<"SteppingAction::UserCalAnalysis"<<G4endl;
 
  G4Track *actualTrack = aStep->GetTrack();
//Data to be stored when a particle enter the calorimeter
//The store is done at the EventAction method

  if (Exit("WorldPhysical") && Entry("CalPhysical"))
  {
      Position = actualTrack->GetPosition();
//	G4cout<<"Position at ECal: "<<Position/cm<<G4endl;
      
      cPDGID = actualTrack->GetDefinition()-> GetPDGEncoding();
      cpID   = actualTrack->GetParentID();
      ckineEne =actualTrack-> GetKineticEnergy();
      cTotalEne =actualTrack->  GetTotalEnergy();
      cTheta = actualTrack-> GetMomentum().theta();
      cPhi = actualTrack-> GetMomentum().phi();
  

      //    for(G4int i=0;i<25;i++) G4cout<<" EnergyGapMod["<<i<<"]: "<<EnergyGapMod[i]<<G4endl;

      fEventAction->EventAtCal(Position, cPDGID,cpID,ckineEne,cTotalEne,cTheta,cPhi);
  }

  if(actualTrack->GetVolume()->GetName()=="Sci")
  {
      if(aStep->GetTotalEnergyDeposit()>0)
      {
      G4StepPoint* point1 = aStep->GetPreStepPoint();
      G4TouchableHandle touch1 = point1->GetTouchableHandle();
      G4int copyNumberModule = touch1->GetCopyNumber(2);

      fEventAction->AddSci(aStep->GetTotalEnergyDeposit());
      fEventAction->AddSciMod(aStep->GetTotalEnergyDeposit(),copyNumberModule);
      }
      //   G4cout<<" EnergyGapMod["<<copyNumberModule<<"]: "<<EnergyGapMod[copyNumberModule]<<G4endl;
  }


}




























G4bool 
 SteppingAction::Entry(G4String Volumen)
{
  if ((ActualVolume != Volumen) 
      && (NextVolume == Volumen))
    return true;
  else 
    return false;
}

G4bool 
 SteppingAction::Exit(G4String Volumen)
{
  if ((ActualVolume == Volumen) 
      && (NextVolume != Volumen))
    return true;
  else 
    return false; 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
