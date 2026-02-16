#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include <fstream>

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction();
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step*) override;

private:
    std::ofstream fOutFile;
    G4double fTotalEdepPerEvent = 0.0;
    G4int fCurrentEventID = -1;
};

#endif