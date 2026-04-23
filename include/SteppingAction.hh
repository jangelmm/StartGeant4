#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction();
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step*) override;

private:
    // Ya no necesitamos fOutFile, fTotalEdepPerEvent ni fCurrentEventID
    // Toda la información se envía ahora al EventAction para guardarse en ROOT.
};

#endif