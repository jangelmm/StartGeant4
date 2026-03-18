#include "TargetSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4OpticalPhoton.hh"

TargetSD::TargetSD(const G4String& name)
    : G4VSensitiveDetector(name)
{
    collectionName.insert("PhotonHits");
}

void TargetSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new G4THitsCollection<TargetHit>(SensitiveDetectorName, collectionName[0]);
    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool TargetSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    const G4Track* track = step->GetTrack();
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

    // Creamos un hit (solo para contar)
    auto newHit = new TargetHit();
    fHitsCollection->insert(newHit);

    return true;
}

void TargetSD::EndOfEvent(G4HCofThisEvent*) { }