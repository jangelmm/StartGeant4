#include "TargetSD.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4OpticalPhoton.hh"

TargetSD::TargetSD(const G4String& name)
    : G4VSensitiveDetector(name) {}

G4bool TargetSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    const G4Track* track = step->GetTrack();

    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        const auto* evtAction = dynamic_cast<const EventAction*>(
            G4RunManager::GetRunManager()->GetUserEventAction());

        if (evtAction) {
            const_cast<EventAction*>(evtAction)->AddPhoton();
            //evtAction->AddPhoton();
        }
        return true;
    }
    return false;
}