#include "SteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction()
{
    fOutFile.open("energy_deposit.csv");
    fOutFile << "EventID,Edep_MeV\n";
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // Solo en el volumen que nos interesa (puedes filtrar por nombre)
    if (step->GetPreStepPoint()->GetPhysicalVolume()->GetName() != "Target") return;
    
    if (step->GetTrack()->GetDefinition()->GetParticleName() == "opticalphoton") {
    G4cout << "Fotondetectado - Energia = " 
           << step->GetTrack()->GetKineticEnergy()/eV << " eV" << G4endl;
}
    G4double edep = step->GetTotalEnergyDeposit();

    if (edep > 0)
    {
        auto* event = G4RunManager::GetRunManager()->GetCurrentEvent();
        G4int eventID = event->GetEventID();

        // Si cambió de evento -> escribe el anterior y resetea
        if (eventID != fCurrentEventID)
        {
            if (fCurrentEventID >= 0)
            {
                fOutFile << fCurrentEventID << "," << fTotalEdepPerEvent / MeV << "\n";
            }
            fTotalEdepPerEvent = 0.0;
            fCurrentEventID = eventID;
        }

        fTotalEdepPerEvent += edep;
    }
}