#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

EventAction::EventAction() = default;

void EventAction::BeginOfEventAction(const G4Event*){
    fPhotonCount = 0;
}

void EventAction::EndOfEventAction(const G4Event* event){
    //Guardar en Root usando G4AnalysisManager
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 0, event->GetEventID());  //Columna: 0 EventID
    analysisManager->FillNtupleDColumn(0, 1, fPhotonCount);         //Columna: 1 Numero de fotones
    analysisManager->AddNtupleRow(0);

    if (event->GetEventID() % 10 == 0){
        G4cout << "# Event " << event->GetEventID() << " -> " << fPhotonCount << " fotones opticos generados" << G4endl;
    }
}