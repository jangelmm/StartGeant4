#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("simulation_output");

    // Crear ntuple para guardar datos por evento
    analysisManager->CreateNtuple("Events", "Datos por evento");
    analysisManager->CreateNtupleDColumn("EventID");
    analysisManager->CreateNtupleDColumn("NPhotons");   // o lo que quieras contar
    analysisManager->FinishNtuple();
}

RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager::Instance()->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}