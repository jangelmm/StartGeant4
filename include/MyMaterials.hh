#ifndef MY_MATERIALS_HH
#define MY_MATERIALS_HH 1

#include "globals.hh"
#include "G4Material.hh"

class G4NistManager;
class MyMaterials{
public: 
    //Destructor
    ~MyMaterials();

    //Obtener la única instancia (Patrón Singleton)
    static MyMaterials* GetInstance();

    //Obtener un material por nombre (muy útil)
    G4Material* GetMaterial(const G4String materialName);

    //Getters directos para los materiales más usados
    G4Material* GetAir()         const { return fAir;         }
    G4Material* GetWater()       const { return fWater;       }
    G4Material* GetPolystyrene() const { return fPolystyrene; }
    G4Material* GetPMMA()        const { return fPMMA;        }

private:
    //Constructor privado (para que solo exista una instancia)
    MyMaterials();

    //Método donde definimos todos los materiales
    void CreateMaterials();

    // Única instancia (Singletón)
    static MyMaterials* fInstance;

    //Manager de NIST
    G4NistManager* fNistMan = nullptr;

    //Materiales (punteros que guardamos)
    G4Material* fAir          = nullptr;
    G4Material* fWater        = nullptr;
    G4Material* fPolystyrene  = nullptr;
    G4Material* fPMMA         = nullptr;
    //Puedes agregar más aquí (fCoating, fTiO2, etc.)
};

#endif