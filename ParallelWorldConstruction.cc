#include "ParallelWorldConstruction.hh"
#include "ParallelWorldMessenger.hh"
#include "ParallelWorldParam.hh"

#include "DetectorConstruction.hh"
#include "PhaseSpaceSD.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4PVDivision.hh"

ParallelWorldConstruction::ParallelWorldConstruction( G4String& parallelWorldName )
    :G4VUserParallelWorld(parallelWorldName),
     fDetSpacing(25*CLHEP::mm),
     fContainerLength(1900*CLHEP::mm),
     fdetectorvolPosition(1860*CLHEP::mm)
{
  fParallelWorldMessenger = new ParallelWorldMessenger(this);
}

ParallelWorldConstruction::~ParallelWorldConstruction()
{ delete fParallelWorldMessenger; }

void ParallelWorldConstruction::Construct()
{
  if( !fDetSpacing || fDetSpacing < 1*CLHEP::nm ){
    G4String eMessage = "ParallelWorldConstruction::Construct() \n";
    eMessage = eMessage + "fDetSpacing not set. Set it with /parallel/detector/spacing in macro.";
    throw std::out_of_range( eMessage );
  }


  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  // % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
  // --->>> ROOM <<<---
  // % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %

  // Ghost world which overlaps with the beamline geometry
  G4VPhysicalVolume* pGhostWorld = GetWorld();
  G4LogicalVolume*   lGhostWorld = pGhostWorld->GetLogicalVolume();


  // % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
  // --->>> SENSITIVE DETECTORS <<<---
  // % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %

  // Number of volumes
  fNoOfCopies = (G4int) ( fContainerLength/fDetSpacing );

  // Thickness of detectors
  G4double thickness = 1.*CLHEP::nm;

  //------------------------------------------------------------------------------------
  // Container for parametrised volumes
  // Position relative to room: z = -4200 + fContainerLength/2
  //------------------------------------------------------------------------------------

  // Shift container to overlap with the beamline
  G4double fRoomSizeZ = 8400.*CLHEP::mm;
  G4double containerPositionZ  = -fRoomSizeZ/2 + fContainerLength/2;

  // Container and detector width
  G4double width = 1000.*CLHEP::mm;

  //------------------------------------------------------------------------------------
  // Sensitive detecotr on parametrised volumes
  // - A sensitive detector is defined on the parametrised volumes so that every hit
  //   in an event is recorded in the same hits collection. The ID of the parametrised
  //   volume a hit occurred in is then a unique property of that hit and can be used
  //   to determine the position at which the hit occured.
  //------------------------------------------------------------------------------------

  // If detector chosen in macro with /parallel/detector command is 'all'
  if( fDetChoice == "all" ){

  // Container
 
//L1 - define container
// fixed parameters here to keep the parallel container long & positioned such that it covers enough, incl the absorber cube
  G4double fdetectorvolPosition = 1860*CLHEP::mm; //chosen here fixed at BP position when cube face is at isocentre
  G4double containerPositionZnew  = -fRoomSizeZ/2 + fdetectorvolPosition; //convert to treatment beamline position, start at 0
  
G4Box* sParallelContainer = new G4Box( "parallel_container", width/2, width/2, fContainerLength );
  G4LogicalVolume* lParallelContainer = new G4LogicalVolume( sParallelContainer, 0, "parallel_container" ); 
 
// new G4PVPlacement( 0, G4ThreeVector(0., 0., containerPositionZ), lParallelContainer,
       //             "parallel_container", lGhostWorld, false, 0 );
//L2 - position of container inside ghost world
new G4PVPlacement( 0, G4ThreeVector(0., 0., containerPositionZnew), lParallelContainer,
                    "parallel_container", lGhostWorld, false, 0 );

  //------------------------------------------------------------------------------------
  // Detector for parametrised volumes
  // Position relative to room: z = -4200 + fContainerLength/2
  //------------------------------------------------------------------------------------

//L3 - define parallel detector and parallel slices
 G4VSolid* sParallelDet = new G4Box( "parallel_detector", fContainerLength/2, fContainerLength/2, thickness/2 );
    // Define solid and logical volume to be replicated
   // G4VSolid* sParallelDet = new G4Box( "parallel_detector", width/2, width/2, thickness/2 );
    G4LogicalVolume* lParallelDet = new G4LogicalVolume( sParallelDet, 0, "parallel_detector" );

    // Construct fNoOfCopies parametrised volumes along the z axis
    // ParallelWorldParam* parametrisation = new ParallelWorldParam( fNoOfCopies, 0*CLHEP::mm, fContainerLength, fDetSpacing );
    ParallelWorldParam* parametrisation = new ParallelWorldParam( fContainerLength, fDetSpacing );
    new G4PVParameterised( "parallel_detector", lParallelDet, lParallelContainer,
                          kZAxis, fNoOfCopies, parametrisation );

    // Define sensitive detector on parametrised volumes
    PhaseSpaceSD* sd = new PhaseSpaceSD( "/sd/phaseSpace" );
    SDman->AddNewDetector( sd );
    lParallelDet->SetSensitiveDetector( sd );

  //------------------------------------------------------------------------------------
  // Sensitive detector over parameterised volumes
  // - Same as /all command except over the distances specified below
  // 40x40x40mm cube after isocentre = 1829. Centred at 1849  
  //   J Yap 23/8/18
  //------------------------------------------------------------------------------------
}

  if( fDetChoice == "detectorvolume" ){

  // Container - L1 
  // fdetectorvolPosition wtr room ie centred at fdetectorvolPosition=1860
 
   G4double containerPositionZnew  = -fRoomSizeZ/2 + fdetectorvolPosition;
   //G4double containerLengthNew = 2100*CLHEP::mm - containerPositionZnew; //take width as fcontainerlength is from proton.mac
   G4double containerLengthNew  = fRoomSizeZ/4;
   G4Box* sParallelContainer = new G4Box( "parallel_container", width/2, width/2, containerLengthNew/2 );
   G4LogicalVolume* lParallelContainer = new G4LogicalVolume( sParallelContainer, 0, "parallel_container" ); 
 
  // New position of parallel container - L2
   new G4PVPlacement( 0, G4ThreeVector(0., 0., containerPositionZnew), lParallelContainer,
                    "parallel_container", lGhostWorld, false, 0 );

  //------------------------------------------------------------------------------------
  // Detector for parametrised volumes - L3
  // Position relative to room: z = -4200 + fdetectorvolPosition/2
  //------------------------------------------------------------------------------------
  
   // Container solid and logical volume to be replicated
   G4VSolid* sParallelDet = new G4Box( "parallel_detector", fContainerLength/2, fContainerLength/2, thickness/2 );
   G4LogicalVolume* lParallelDet = new G4LogicalVolume( sParallelDet, 0, "parallel_detector" );

    // Construct fNoOfCopies parametrised volumes along the z axis
   ParallelWorldParam* parametrisation2 = new ParallelWorldParam( fContainerLength, fDetSpacing );
   new G4PVParameterised( "parallel_detector", lParallelDet, lParallelContainer,
                          kZAxis, fNoOfCopies, parametrisation2 );

    // Define sensitive detector on parametrised volumes
    PhaseSpaceSD* sd = new PhaseSpaceSD( "/sd/phaseSpace" );
    SDman->AddNewDetector( sd );
    lParallelDet->SetSensitiveDetector( sd );
 
G4cout << "Value of detector" << fDetChoice << "pos" << fdetectorvolPosition << G4endl;
///

 /*// starting position of first parameterised slice
   G4double detectorvolPositionFirstPos = -950.*CLHEP::mm + (fdetectorvolPosition - fContainerLength/2 + fDetSpacing/2);

  // Define solid and logical volume to be replicated dimensions/2
    G4VSolid* sParallelDetVol = new G4Box( "parallel_detectorvol", fContainerLength/2, fContainerLength/2, thickness/2 );
    G4LogicalVolume* lParallelDetVol = new G4LogicalVolume( sParallelDetVol, 0, "parallel_detectorvol" );
  
  // Construct fNoOfCopies parametrised volumes along the z axis
      ParallelWorldParam* parametrisation2 = new ParallelWorldParam( fNoOfCopies, detectorvolPositionFirstPos, fContainerLength, fDetSpacing );
    new G4PVParameterised("parallel_detectorvol", lParallelDetVol, lParallelContainer, kZAxis, fNoOfCopies, parametrisation2);

  // Define sensitive detector on parametrised volumes
    PhaseSpaceSD* sd = new PhaseSpaceSD( "/sd/phaseSpace" );
    SDman->AddNewDetector( sd );
    lParallelDetVol->SetSensitiveDetector( sd );*/
  }

  //------------------------------------------------------------------------------------
  // Sensitive detector on single volume
  // - Defines sensitive detector on a volume chosen in the macro using the
  //   /parallel/detector command.
  // JY 30/01/19
  // MH defined containervol as 1900mm for this option, for the others JY defined it 
  // as 2400mm (fRoomSizeZ/4) such that it is consistent with the treatment beamline z
  //------------------------------------------------------------------------------------

  if( fDetChoice != "all" && fDetChoice != "detectorvolume" ){
    G4String sdVolName = "sd_" + fDetChoice;

  G4Box* sParallelContainer = new G4Box( "parallel_container", width/2, width/2, fContainerLength/2 );
  G4LogicalVolume* lParallelContainer = new G4LogicalVolume( sParallelContainer, 0, "parallel_container" ); 

  new G4PVPlacement( 0, G4ThreeVector(0., 0., containerPositionZ), lParallelContainer,
                    "parallel_container", lGhostWorld, false, 0 );

    // Define map of component positions of face closest to the source, ie the face
    // hit first by the proton beam. This allows retrieving the position by the component's name.
// JY - Component position = -1900/2 + (position in z) 
    std::map< G4String, G4ThreeVector > componentPosition;
    componentPosition["scatterfoil1"] = G4ThreeVector(0., 0., -869.*CLHEP::mm); //81
    componentPosition["tube1"]        = G4ThreeVector(0., 0., -593.*CLHEP::mm);
    componentPosition["nozzle"]       = G4ThreeVector(0., 0.,  750.*CLHEP::mm);
    componentPosition["outside"]      = G4ThreeVector(0., 0.,  818.*CLHEP::mm); // 1768 mm ie 1cm after nozzle
// JY - Add sensitive detector to water phantom at BP position  
	componentPosition["phantom"]      = G4ThreeVector(0., 0.,  910.*CLHEP::mm); //1860 mm
//add variable component dependent on location z, change z coord here before running
	componentPosition["film1"]      = G4ThreeVector(0., 0., -593.*CLHEP::mm); 
	componentPosition["film1new"]      = G4ThreeVector(0., 0., -570.*CLHEP::mm); 
	componentPosition["film2"]      = G4ThreeVector(0., 0., -501.*CLHEP::mm); 
	componentPosition["film_integzone"]      = G4ThreeVector(0., 0., -274.*CLHEP::mm); 
	componentPosition["film6"]      = G4ThreeVector(0., 0., -104.*CLHEP::mm); 
	componentPosition["film4"]      = G4ThreeVector(0., 0., 59.*CLHEP::mm); 
	componentPosition["film8"]      = G4ThreeVector(0., 0., 496.*CLHEP::mm); 
	componentPosition["film5"]      = G4ThreeVector(0., 0., 721.*CLHEP::mm);	
	componentPosition["film7"]      = G4ThreeVector(0., 0., 921.*CLHEP::mm); 
	
    // Define volume for sensitive detector
    G4VSolid* sDetector = new G4Box( sdVolName, width/2, width/2, thickness/2 );//14.*CLHEP::um);//JY//;
    G4LogicalVolume* lDetector = new G4LogicalVolume( sDetector, 0, sdVolName );

    // Shift position of detector so faces of detector and component volume coincide
    G4ThreeVector detPosition = componentPosition.at(fDetChoice) + G4ThreeVector(0., 0., 0.5*CLHEP::nm);
    new G4PVPlacement(0, detPosition, lDetector, sdVolName, lParallelContainer, 0, 0, 0);

    // Define sensitive detector on volume
    PhaseSpaceSD* sd = new PhaseSpaceSD( "/sd/phaseSpace/" + fDetChoice );
    SDman->AddNewDetector( sd );
    lDetector->SetSensitiveDetector( sd );
  }


}
