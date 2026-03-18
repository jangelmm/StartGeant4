#ifndef TARGET_SD_HH
#define TARGET_SD_HH

#include "G4VSensitiveDetector.hh"
#include "TargetHit.hh"
#include "G4THitsCollection.hh"

class TargetSD : public G4VSensitiveDetector
{
public:
    TargetSD(const G4String& name);
    ~TargetSD() override = default;

    void Initialize(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent*) override;

private:
    G4THitsCollection<TargetHit>* fHitsCollection = nullptr;
};

#endif