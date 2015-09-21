//
/// \file analysis/shared/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
// $Id: PrimaryGeneratorAction.cc 85905 2014-11-06 08:51:55Z gcosmo $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"
#include "Variables.hh"

#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4cout<<"PrimaryGeneratorAction::PrimaryGeneratorAction() begin..."<<G4endl;

  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  
  //In this method is defined "static" properties of the gun
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  G4String particleName;


  G4ParticleDefinition* particle 
    = particleTable->FindParticle(iVariables->BeamParticle);
  

  ParticleMomentum =  iVariables-> BeamMomentum;//for neutrons

  fParticleGun->SetParticleDefinition(particle);  
  fParticleGun->SetParticleMomentum(ParticleMomentum);

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  
  G4cout<<"Particle: "<<particleName<<" Momentum: "<<ParticleMomentum/GeV<<"GeV Energy: "<<fParticleGun->GetParticleEnergy()<<G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  //because, we want certain distribution every event, the position is 
  //defined here

  rad = G4UniformRand()* 1.5 *cm; //random distance with a 3cm diameter
  theta = G4UniformRand()* 360.*deg; //random theta angle
  
  //  G4cout<<"rad: "<<rad/cm<<" theta: "<<theta/deg<<G4endl;

  offset = -300.*cm;//some separation from the 0 position

  G4ThreeVector position(rad*cos(theta),rad*sin(theta),offset);

  fParticleGun->SetParticlePosition(position);

  //this function is called at the begining of event
  // 
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ParticleGun* PrimaryGeneratorAction::GetParticleGun()
{
  return fParticleGun;
} 
