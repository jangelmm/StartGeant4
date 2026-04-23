#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    // Es buena práctica inicializar RunAction también en el hilo maestro para los archivos ROOT
    SetUserAction(new RunAction()); 
}

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new RunAction());
    SetUserAction(new EventAction());
    SetUserAction(new SteppingAction());
}