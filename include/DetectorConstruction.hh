#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;   // La dejamos declarada aunque esté vacía en el .cc

private:
    // ¡Todo limpio! 
    // Las variables fLogicTarget y fTargetSD fueron eliminadas 
    // ya que la detección ahora se hace directamente desde SteppingAction.
};

#endif