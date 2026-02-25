#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction {

public:
    EventAction();
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    void AddPhoton() { fPhotonCount++; }

private:
    G4int fPhotonCount = 0; //Fotones por evento

};

#endif