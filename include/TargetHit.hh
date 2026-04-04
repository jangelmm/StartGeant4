#ifndef TARGET_HIT_HH
#define TARGET_HIT_HH

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class TargetHit : public G4VHit
{
public:
    TargetHit() = default;
    ~TargetHit() override = default;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    void Draw() override {}
    void Print() override {}

    // --- NUEVO: energía del fotón ---
    void SetEnergy(G4double e) { fEnergy = e; }
    G4double GetEnergy() const { return fEnergy; }

private:
    G4double fEnergy = 0.0;  // energía en eV
};

extern G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator;

inline void* TargetHit::operator new(size_t)
{
    if (!TargetHitAllocator) TargetHitAllocator = new G4Allocator<TargetHit>;
    return (void*)TargetHitAllocator->MallocSingle();
}

inline void TargetHit::operator delete(void* hit)
{
    TargetHitAllocator->FreeSingle((TargetHit*)hit);
}

#endif
