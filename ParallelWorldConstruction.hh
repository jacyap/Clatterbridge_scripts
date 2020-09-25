// M Hentz, 2018
// Updated JY, 2019
//
// A parallel world construction class
//
// - void Construct()
//     creates a parallel world in the mass world and parameterized volumes
//

#ifndef ParallelWorldConstruction_h
#define ParallelWorldConstruction_h 1

#include "G4VUserParallelWorld.hh"
#include "globals.hh"

#include "ParallelWorldMessenger.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;


class ParallelWorldConstruction : public G4VUserParallelWorld
{
public:
  ParallelWorldConstruction( G4String& );
  virtual ~ParallelWorldConstruction();

  virtual void Construct();
  
  void SetDetector( G4String detChoice ) { fDetChoice = detChoice; }
  void SetDetSpacing( G4double detSpacing ) { fDetSpacing = detSpacing; }
  void SetDetDistance( G4double containerLength ) { fContainerLength = containerLength; }
//
  void SetDetVolPosition( G4double detectorvolPosition ) { fdetectorvolPosition = detectorvolPosition; }

  
private:
  ParallelWorldMessenger* fParallelWorldMessenger;

  G4double  fDetSpacing;
  G4double  fContainerLength;
  G4int     fNoOfCopies;
  G4String  fDetChoice;
//
  G4double  fdetectorvolPosition;

};

#endif

