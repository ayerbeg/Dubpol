//
/// \file analysis/AnaEx02/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// $Id: HistoManager.cc 74272 2013-10-02 14:48:50Z gcosmo $
// GEANT4 tag $Name: geant4-09-04 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <sstream> 
#include "Variables.hh"

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
:fRootFile(0), 
 fNtuple1(0), fNtuple2(0), 
 fNoEve(0), fEdep(0), fTileNo(0), fPDGid(0),
 tNoEve(0), tEdep(0), tTileNo(0), 
 fLabs(0), fLgap(0), fMod(0), fEneMod(0)
{
      
  // histograms
  for (G4int k=0; k<MaxHisto; k++) fHisto[k] = 0;
    
  // ntuple
  fNtuple1 = 0;
  fNtuple3 = 0;
  fNtuple2 = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  if ( fRootFile ) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{ 
 // Creating a tree container to handle histograms and ntuples.
 // This tree is associated to an output file.
 //

   
  G4String fileName = iVariables->RootFile;

  // G4String fileName = "./files/5M_1SciTile_targetdata_onCH2.root";
 fRootFile = new TFile(fileName,"RECREATE");
 if(!fRootFile) {
   G4cout << " HistoManager::book :" 
          << " problem creating the ROOT TFile "
          << G4endl;
   return;
 }
   
 fHisto[1] = new TH1D("1", "Edep in Scintillator (Total)", 801, -0.5, 800.5*CLHEP::MeV);
 if (!fHisto[1]) G4cout << "\n can't create histo 1" << G4endl;

 fHisto[2] = new TH1D("2", "Edep in gap", 801, 0.5, 800.5*CLHEP::MeV);
 if (!fHisto[2]) G4cout << "\n can't create histo 2" << G4endl;

 fHisto[3] = new TH1D("3", "trackL in absorber", 100, 0., 1*CLHEP::m);
 if (!fHisto[3]) G4cout << "\n can't create histo 3" << G4endl;

 fHisto[4] = new TH1D("4", "trackL in gap", 100, 0., 50*CLHEP::cm);
 if (!fHisto[4]) G4cout << "\n can't create histo 4" << G4endl;  

 // create 1st ntuple in subdirectory "tuples"
 //
 fNtuple1 = new TTree("Target", "Data from Target");
 fNtuple1->Branch("NoEve", &fNoEve, "NoEVe/I");
 fNtuple1->Branch("Edep", &fEdep, "Edep/D");
 fNtuple1->Branch("KineEne", &fkineEne, "KineEne/D");
 fNtuple1->Branch("TileNo", &fTileNo, "TileNo/I");
 fNtuple1->Branch("PDGID", &fPDGid, "PDGID/I");
 fNtuple1->Branch("ParentID", &fPid, "PID/I");
 fNtuple1->Branch("Theta", &fTheta, "Theta/D");
 fNtuple1->Branch("Phi", &fPhi, "Phi/D");



 fNtuple3 = new TTree("TargetTile", "Data from Target");
 fNtuple3->Branch("NoEve", &fNoEve, "NoEVe/I");
 fNtuple3->Branch("Edep", &tEdep, "Edep/D");
 fNtuple3->Branch("TileNo", &tTileNo, "TileNo/I");

 // create 2nd ntuple in subdirectory "tuples"
 //
 fNtuple2 = new TTree("Cal", "Data from ideal calorimeter");
 fNtuple2->Branch("NoEve", &cNoEve, "NoEve/I");
 fNtuple2->Branch("X", &cX, "X/D");
 fNtuple2->Branch("Y", &cY, "Y/D");
 fNtuple2->Branch("Z", &cZ, "Z/D");
 fNtuple2->Branch("PDGID", &cPDGID, "PDGID/I");
 fNtuple2->Branch("ParentID", &cPid, "PID/I");
 fNtuple2->Branch("kineEne", &ckineEne, "kineEne/D");
 fNtuple2->Branch("TotalEne", &cTotalEne, "TotalEne/D");
 fNtuple2->Branch("Theta", &cTheta, "Theta/D");
 fNtuple2->Branch("Phi", &cPhi, "Phi/D");

 fNtupleCalMod = new TTree("CalMod", "Data from Calorimeter modules");
 fNtupleCalMod->Branch("NoEve", &fNoEve, "NoEVe/I");
 fNtupleCalMod->Branch("Mod", &fMod, "Mod/I");
 fNtupleCalMod->Branch("EneMod", &fEneMod, "EneMod/D");

 
 G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{ 
  if (fRootFile) {
    fRootFile->Write();       // Writing the histograms to the file
    fRootFile->Close();        // and closing the tree (and the file)
    G4cout
      << "\n----> Histogram Tree is saved \n"
      <<"\n\n....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......\n\n"
      <<"THIS IS THE NAME OF THE ROOT FILE TO PROVIDE TO THE ROOT MACRO: \n\n"
      <<"----->   "
      <<iVariables->RootFile
      <<"   <-----"
      << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (fHisto[ih]) { fHisto[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (fHisto[ih]) fHisto[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillTargetNtuple(G4int EventNo, G4double energyGap, G4double energyKine, G4int Tile,
				    G4int PDGid, G4int pID,G4double Theta, G4double Phi )
{
 fNoEve = EventNo;
 fEdep = energyGap;
 fkineEne = energyKine;
 fTileNo = Tile;
 fPDGid = PDGid;
 fPid = pID;
 fTheta = Theta;
 fPhi = Phi;

  if (fNtuple1) 
  {
      fNtuple1->Fill();
   }

}

void HistoManager::FillTargetTileNtuple(G4int EventNo, G4double energyGap, G4int Tile)
{
 tNoEve = EventNo;
 tEdep = energyGap;
 tTileNo = Tile;
  if (fNtuple3&&tEdep>0) 
  {
      fNtuple3->Fill();
    }

}

void HistoManager::FillEventCalMod(G4int EventNo, G4int mod, G4double EnergyGapMod)
{
 fNoEve = EventNo;
 fMod = mod;
 fEneMod = EnergyGapMod;
  
 if (fNtupleCalMod&&fEneMod!=0) fNtupleCalMod->Fill();//we fill if energy is deposited (bigger than 0)
}

void HistoManager::FillCalEventNtuple(G4int EventNo, G4ThreeVector Position, G4int PDGID, G4int PARENTID, G4double kineEne, G4double TotalEne, G4double Theta, G4double Phi)
{
    cNoEve = EventNo;
    cX= Position.x();
    cY= Position.y();
    cZ= Position.z();

    cPDGID = PDGID;
    cPid = PARENTID;
    ckineEne = kineEne;
    cTotalEne = TotalEne;
    cTheta = Theta;
    cPhi = Phi;

  if (fNtuple2) fNtuple2->Fill();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  if(fHisto[1]) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;
    
    G4cout 
    << " EAbs : mean = " << G4BestUnit(fHisto[1]->GetMean(), "Energy") 
            << " rms = " << G4BestUnit(fHisto[1]->GetRMS(),  "Energy") << G4endl;
    G4cout                
    << " EGap : mean = " << G4BestUnit(fHisto[2]->GetMean(), "Energy") 
            << " rms = " << G4BestUnit(fHisto[2]->GetRMS(),  "Energy") << G4endl;
    G4cout 
    << " LAbs : mean = " << G4BestUnit(fHisto[3]->GetMean(), "Length") 
            << " rms = " << G4BestUnit(fHisto[3]->GetRMS(),  "Length") << G4endl;
    G4cout 
    << " LGap : mean = " << G4BestUnit(fHisto[4]->GetMean(), "Length") 
            << " rms = " << G4BestUnit(fHisto[4]->GetRMS(),  "Length") << G4endl;

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


