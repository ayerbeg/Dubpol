//
/// \file analysis/AnaEx02/include/HistoManager.hh
/// \brief Definition of the HistoManager class
//
// $Id: HistoManager.hh 74272 2013-10-02 14:48:50Z gcosmo $
// GEANT4 tag $Name: geant4-09-04 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HistoManager_h
#define HistoManager_h 1
#include "G4ThreeVector.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 class TFile;
 class TTree;
 class TH1D;

  const G4int MaxHisto = 5;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
  public:
  
    HistoManager();
   ~HistoManager();
   
    void book();
    void save();

    void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
    void Normalize(G4int id, G4double fac);    

    void FillTargetNtuple(G4int EventNo, G4double energyGap, G4double energyKine, G4int Tile,
			  G4int PDGid, G4int pID,G4double Theta, G4double Phi);

    void FillTargetTileNtuple(G4int EventNo, G4double energyGap, G4int Tile);

    void FillCalEventNtuple(G4int EventNo, G4ThreeVector Position, G4int PDGID, G4int PARENTID, G4double kineEne, G4double TotalEne, G4double Theta, G4double Phi);
    void FillEventCalMod(G4int EventNo, G4int mod, G4double EnergyGapMod);

    void PrintStatistic();
        
  private:
  
    TFile*   fRootFile;
    TH1D*    fHisto[MaxHisto];            
    TTree*   fNtuple1;    
    TTree*   fNtuple3;    
    TTree*   fNtuple2;    
    TTree*   fNtupleCalMod;



    G4int fNoEve;
    G4double fEdep;
    G4double fkineEne;
    G4int fTileNo;
    G4int fPDGid;
    G4int fPid;
    G4double fTheta;
    G4double fPhi;


    G4int tNoEve;
    G4double tEdep;
    G4int tTileNo;

    G4double fLabs;
    G4double fLgap;

    G4int fMod;
    G4double fEneMod;

    G4int cNoEve;
    G4int cPDGID;
    G4int cPid;
    G4double ckineEne;
    G4double cTotalEne;
    G4double cTheta;
    G4double cPhi;
    G4double cX, cY, cZ;

   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

