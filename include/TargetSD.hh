#ifndef TARGET_SD_HH
#define TARGET_SD_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"

class TargetSD : public G4VSensitiveDetector
{
public:
    TargetSD(const G4String& name);
    ~TargetSD() override = default;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
};

#endif