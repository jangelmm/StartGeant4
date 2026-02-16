#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    // En modo multi-hilo, puedes definir acciones específicas para el hilo maestro si es necesario
    // Por ahora, no necesitamos acciones específicas para el hilo maestro
}

void ActionInitialization::Build() const {
    // Define las acciones del usuario para cada hilo
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new SteppingAction());
}