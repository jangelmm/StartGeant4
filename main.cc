#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {
    // Modo un solo Hilo (Debug)
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial); 

    // Modo multi-hilo
    //auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);

    //G4int nThreads = G4Threading::G4GetNumberOfCores();
    //runManager->SetNumberOfThreads(nThreads); 


    // Inicializa clases
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());  // ← NUEVO

    runManager->Initialize();

    // Visualización
    auto* visManager = new G4VisExecutive();
    visManager->Initialize();

    // UI
    auto* UImanager = G4UImanager::GetUIpointer();

    if (argc == 1) {
        auto* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute run.mac");
        ui->SessionStart();
        delete ui;
    } else {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + argv[1]);
    }

    delete visManager;
    delete runManager;
    return 0;
}