#ifndef Variables_H
#define Variables_H 1

#include <iostream>
using namespace std;

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

// Since the vectors can not be dimensioned dinamically
// we give a big number (40x80=3200 modules for example)
#define NoMaxModules 3200

class Variables
{
public:
    Variables();
    ~Variables();
    
    G4int LoadFromFile(G4String FileName);
    //   G4int SaveToFile(G4String FileName);
    
//ECalDetectorConstruction
  
  G4bool G4GUI;
  G4String RootFile;

  G4String BeamParticle;
  G4double BeamMomentum;


  G4int    NoOfCol;
  G4int    NoOfRow;
  G4int    NoOfTiles;
  G4double TargetShift;
  G4double TilesSizeX;
  G4double TilesSizeY;
  G4double TilesSizeThick;
  G4String TargetMat;

  
  G4int    NoOfPa;
  G4double CalShift;
  
  G4double ModuleSizeX;
  G4double ModuleSizeY;
  G4double AbsThick;
  G4double SciThick;

  
  
    G4double LeadThickness;
    G4double ScintThickness;
    G4int NbOfLayers;  
    G4double CalModSizeXY;   
    G4int NbOfCaloColumns;
    G4int NbOfCaloRows;

    G4bool ECalDetail;
    G4bool Fibers;
 
    G4int Verbose;  


    G4double EleAng;   
    G4double ECalDis;

//ECalAnalysis
    G4String      FileNameSuffix;	
  
//SBSPrimaryGeneratorAction
    G4ThreeVector Target;

    G4int         ReactionCase;	
    G4double      EnergyBeam;
    G4int         nK;
    G4String      ReactionParticle[4];

/*

  G4String Elektrondetektor;
 
  G4double SpecBImpuls;
  //	G4double SpecBImpulsbreite;
  
  G4String Target;


  G4bool Simple;

  G4bool ScanAngles;
  G4double KAOS_ang;
  G4double C_ang;
 
  G4double maxAng;
  G4double angStep;
  
  G4bool ScanEnergy;
  G4double minEne;
  G4double maxEne;
  G4double eneStep;

 
  G4ThreeVector TARGETPosition;


*/
private:
    G4bool ReadBoolean(G4String Value);
	
};

ostream & operator << (ostream& s, const Variables v);

extern Variables *iVariables;
extern G4String RootFile;

#endif
