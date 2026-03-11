#include "PhysicsList.hh"

// Física electromagnética estándar
#include "G4EmStandardPhysics.hh"

// Física óptica (fotones, centelleo, absorción, etc.)
#include "G4OpticalPhysics.hh"

// Decaimiento de partículas inestables
#include "G4DecayPhysics.hh"

PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
    // Orden recomendado: decaimiento → EM → óptica
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4OpticalPhysics());
}

PhysicsList::~PhysicsList() = default;