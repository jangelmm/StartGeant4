#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4EventManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <vector>

SteppingAction::SteppingAction() {}

// --- SUBRUTINA: Interpolación Lineal del PDE ---
G4double GetPDE(G4double wavelength_nm) {
    // Valores digitalizados de la Fig 3 (Overvoltage 5.0V)
    std::vector<G4double> wl =  {300, 320, 340, 360, 380, 400, 420, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950};
    std::vector<G4double> pde = {0.04, 0.10, 0.23, 0.29, 0.31, 0.39, 0.41, 0.39, 0.31, 0.22, 0.16, 0.12, 0.08, 0.06, 0.04, 0.025, 0.015, 0.01};

    // Límites
    if (wavelength_nm <= wl.front()) return pde.front();
    if (wavelength_nm >= wl.back()) return pde.back();

    // Interpolación Lineal
    for (size_t i = 0; i < wl.size() - 1; ++i) {
        if (wavelength_nm >= wl[i] && wavelength_nm <= wl[i+1]) {
            G4double slope = (pde[i+1] - pde[i]) / (wl[i+1] - wl[i]);
            return pde[i] + slope * (wavelength_nm - wl[i]);
        }
    }
    return 0.0;
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    
    // Solo nos interesan los fotones ópticos
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return;

    // Obtener el EventAction para sumarle contadores (usando EventManager)
    auto eventAction = static_cast<EventAction*>(G4EventManager::GetEventManager()->GetUserEventAction());
    if (!eventAction) return;

    G4StepPoint* prePoint = step->GetPreStepPoint();
    G4StepPoint* postPoint = step->GetPostStepPoint();

    // 1. Calcular fotones PRODUCIDOS en el centelleador
    // (Si está en el primer paso de su vida, acaba de nacer)
    if (track->GetCurrentStepNumber() == 1 && prePoint->GetPhysicalVolume()->GetName() == "ScintillatorPV") {
        eventAction->AddProduced();
    }

    // 2. Extraer energía cuando golpea el Silicio (SiPM)
    if (postPoint && postPoint->GetPhysicalVolume() && postPoint->GetPhysicalVolume()->GetName() == "SiliconSensorPV") {
        
        // Confirmar que apenas va entrando (prePoint estaba afuera)
        if (prePoint->GetPhysicalVolume()->GetName() != "SiliconSensorPV") {
            
            G4double energy = track->GetKineticEnergy();
            
            // Fórmula: lambda(nm) = hc / E 
            G4double wl_nm = (1239.841939 * eV * nm / energy) / nm;
            
            // Evaluar PDE
            G4double currentPDE = GetPDE(wl_nm);
            
            // Condición Monte Carlo
            if (G4UniformRand() <= currentPDE) {
                eventAction->AddDetected();
            }
            
            // Como ya tocó el sensor y se evaluó, matamos el fotón para ahorrar memoria
            track->SetTrackStatus(fStopAndKill);
        }
    }
}