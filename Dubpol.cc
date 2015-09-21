// Alpom calorimeter Main file based on
/// \file analysis/AnaEx02/AnaEx02.cc
/// \brief Main program of the analysis/AnaEx02 example
//
//
// $Id: Dubpol.cc 
//
// Carlos Ayerbe Gayoso 
// 20.Aug.2015
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BIC.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "HistoManager.hh"

#include "Variables.hh"

#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

//The first object to load should be the variables, since they are read externally
  iVariables = new Variables(); // This object stores all parameters which are read from var.ini

  G4String dataname;
  dataname = "var.ini";

  iVariables->LoadFromFile(dataname);

// Choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    
    //with the next code, we assure that every time the program runs, the engine has new seeds
    G4int index=time(NULL);
    G4long seeds[2];
    seeds[0] = (long) time(NULL) ;
    seeds[1] = (long) (time(NULL)*G4UniformRand());
    CLHEP::HepRandom::setTheSeeds(seeds,index);


  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //

  // PhysicsList* physics = new PhysicsList;
  // runManager->SetUserInitialization(physics);

//  runManager->SetUserInitialization(new FTFP_BERT(0));//the 0 sets the verbosity level to null
  runManager->SetUserInitialization(new QGSP_BIC(0));//the 0 sets the verbosity level to null

  // set an HistoManager
  //
  HistoManager*  histo = new HistoManager();

  // Set user action classes
  //
  PrimaryGeneratorAction* gen_action = 
                          new PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  //
  RunAction* run_action = new RunAction(histo);  
  runManager->SetUserAction(run_action);
  //
  EventAction* event_action = new EventAction(run_action,histo);
  runManager->SetUserAction(event_action);
  //
  SteppingAction* stepping_action =
      new SteppingAction(event_action,histo);
  runManager->SetUserAction(stepping_action);
  
  // Initialize G4 kernel
  //
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize(); 
#endif

 
  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);    
    }
  else
    {
      if(iVariables->G4GUI)
	{
	  // G4UIterminal is a (dumb) terminal.
	  G4UIsession * session = 0;
#ifdef G4UI_USE
	  session = new G4UIterminal(new G4UItcsh);
#else
	  session = new G4UIterminal();
#endif
	  session->SessionStart();
	  delete session;
	}
      else
	{ 
#ifdef G4UI_USE
	  G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#endif
	  
	  
#ifdef G4UI_USE
	  if (ui->IsGUI())
	    UImanager->ApplyCommand("/control/execute gui.mac");
	  UImanager->ApplyCommand("/control/execute vis.mac");     
	  ui->SessionStart();
	  
	  delete ui;
#endif
	  
#ifdef G4VIS_USE
	  
	  delete visManager;
#endif
	  
	}
    }
  
  
  // Job termination
  delete histo;                
  delete runManager;
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
