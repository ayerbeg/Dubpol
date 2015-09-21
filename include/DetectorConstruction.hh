//
// $Id: Par01DetectorConstruction.hh 77659 2013-11-27 08:57:46Z gcosmo $
//
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4LogicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "TargetDetectorConstruction.hh"
#include "CalDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  
public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

private:
    TargetDetectorConstruction *Target;
    CalDetectorConstruction    *Calorimeter;


};


#endif

