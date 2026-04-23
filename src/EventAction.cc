#include "EventAction.hh"
#include "G4Event.hh"

EventAction::EventAction() = default;

void EventAction::BeginOfEventAction(const G4Event*)
{
    // Reseteamos los contadores al iniciar un nuevo evento (disparo del electrón)
    fProduced = 0;
    fDetected = 0;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    // Guardamos los datos en el archivo ROOT
    auto analysis = G4AnalysisManager::Instance();
    analysis->FillNtupleIColumn(0, 0, event->GetEventID());
    analysis->FillNtupleIColumn(0, 1, fProduced);
    analysis->FillNtupleIColumn(0, 2, fDetected);
    analysis->AddNtupleRow(0);

    // Imprimir en consola cada 10 eventos para monitorear
    if (event->GetEventID() % 10 == 0) {
        G4cout << "Evento " << event->GetEventID() 
               << " | Fotones Producidos: " << fProduced 
               << " | Detectados (por PDE): " << fDetected << G4endl;
    }
}