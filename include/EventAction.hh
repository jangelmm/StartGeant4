#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4AnalysisManager.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    // Métodos para que el SteppingAction sume fotones
    void AddProduced() { fProduced++; }
    void AddDetected() { fDetected++; }

private:
    G4int fProduced = 0;
    G4int fDetected = 0;
};

#endif