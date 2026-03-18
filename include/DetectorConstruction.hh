#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4Cache.hh"
#include "TargetSD.hh"   

class TargetSD;   // forward declaration

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;   // ← ¡Obligatorio en Geant4 11!

private:
    G4LogicalVolume* fLogicTarget = nullptr;
    G4Cache<TargetSD*> fTargetSD;   // Cache thread-safe
};

#endif