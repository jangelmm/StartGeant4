#include "EventAction.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4THitsCollection.hh"
#include "TargetHit.hh"

EventAction::EventAction() = default;

void EventAction::BeginOfEventAction(const G4Event*)
{
    fPhotonCount = 0;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce) return;

    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID("TargetSD/PhotonHits");
    auto hitsCollection = static_cast<G4THitsCollection<TargetHit>*>(hce->GetHC(hcID));

    G4int nPhotons = 0;
    if (hitsCollection) {
        nPhotons = hitsCollection->entries();
    }

    // Guardar energía de cada fotón detectado
    for (int i = 0; i < hitsCollection->entries(); i++) {
        auto hit = (*hitsCollection)[i];
        // Aquí podrías extender TargetHit para guardar energía
        // Ejemplo: hit->SetEnergy(photonEnergy);
    }

    auto analysis = G4AnalysisManager::Instance();
    analysis->FillNtupleIColumn(0, 0, event->GetEventID());
    analysis->FillNtupleIColumn(0, 1, nPhotons);
    analysis->AddNtupleRow(0);

    if (event->GetEventID() % 10 == 0) {
        G4cout << "Evento " << event->GetEventID() << " → " << nPhotons << " fotones detectados" << G4endl;
    }
}
