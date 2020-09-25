// M Hentz, 2016
// J Yap, 2018 edited to get eDep in layers

#include <iostream>

#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAccumulator.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4Event.hh"
#include "Randomize.hh"

//#include "ParallelWorldConstruction.hh"
using namespace std;

RunAction::RunAction( DetectorConstruction* det, PhysicsList* phys, PrimaryGeneratorAction* kin )
    :G4UserRunAction(),
     fAnalysisManager(0),
     fDetector(det),
     fPhysics(phys),
     fKinematic(kin),
     fLayerEdep(new G4double[MaxLayer]),
     fEdepTot(0.),
     fNbPrimarySteps(0),
     fBufferSize(1000),
     fDumpSingle(0)
{
  fRunActionMessenger = new RunActionMessenger( this );
}

RunAction::~RunAction()
{
  delete fRunActionMessenger;
  delete [] fLayerEdep;
}

void RunAction::BeginOfRunAction( const G4Run* aRun )
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

// JYAP updated 2018  
//create messenger link
MaxLayer = fDetector ->GetLayerNumber();
//MaxLayer = fContainerLength ->GetContainerLength();
//MaxLayer = fDetDistance ->GetDetDistance();

//create edep file
  // Dump
    psfile.open( "data/edep.txt",  ofstream::app );

  // Set precision of variables to be dumped
  psfile
      <<  setiosflags( ios::fixed)
      <<  setprecision(8);

   G4String header = "eDep [MeV]";
    psfile << header << G4endl;
 psfile.close();
  //
// MH default follows

  CLHEP::HepRandom::showEngineStatus();
  
  fNbPrimarySteps = 0;
  fEdepTot = 0.;
  for ( G4int j=0; j < MaxLayer; j++ ){
    fLayerEdep[j] = 0.;
  }

  fKinematic->ResetEbeamCumul();
}

G4Run* RunAction::GenerateRun()
{
  if( fDumpSingle )
    return new RunAccumulator( fBufferSize, fDetPosition );

  return new RunAccumulator( fBufferSize, fDetDistance, fDetSpacing );
}

void RunAction::EndOfRunAction( const G4Run* aRun )
{
  G4cout << "\n### End of run" << G4endl;
  
  G4int nbofEvents = aRun->GetNumberOfEvent();
 
  //Write edep to file

 psfile.open( "data/edep.txt",  ofstream::app );
     for ( G4int j=0; j < MaxLayer; j++ ){
 psfile <<j<<" "<<fLayerEdep[j] <<G4endl;}
  psfile.close();
//

  // Compute mean number of primary steps
  G4double nstep = G4double(fNbPrimarySteps)/G4double(nbofEvents);
  G4cout << "\n---> Mean number of primary steps : " << nstep << G4endl;
  
  CLHEP::HepRandom::showEngineStatus();
}
