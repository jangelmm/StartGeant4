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

    // Para que Geant4 pueda usar la colección
    inline void* operator new(size_t);
    inline void operator delete(void*);

    void Draw() override {}   // vacío por ahora (puedes dibujar después)
    void Print() override {}  // vacío por ahora

private:
    // Aquí puedes agregar más datos en el futuro (energía, tiempo, posición...)
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