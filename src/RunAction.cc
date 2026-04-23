#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction()
{
    auto analysis = G4AnalysisManager::Instance();
    analysis->SetDefaultFileType("root");
    analysis->SetFileName("simulation_output");

    analysis->CreateNtuple("Events", "Datos por evento");
    analysis->CreateNtupleIColumn("EventID");
    analysis->CreateNtupleIColumn("NProduced"); // Columna 1
    analysis->CreateNtupleIColumn("NDetected"); // Columna 2
    analysis->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager::Instance()->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}

RunAction::~RunAction() = default;