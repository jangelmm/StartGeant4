#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);
    auto* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    if (!particle) {
        G4cerr << "ERROR: Partícula 'e-' no encontrada!\n";
        return;
    }
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(42.0 * MeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -20*cm));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    fParticleGun->GeneratePrimaryVertex(event);  // ¡DISPARA!
}