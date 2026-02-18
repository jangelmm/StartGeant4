#include "MyMaterials.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh" //Propiedades ópticas

MyMaterials* MyMaterials::fInstance = nullptr;

//Constructor privado
MyMaterials::MyMaterials(){
    fNistMan = G4NistManager::Instance();
    fNistMan->SetVerbose(1); //Muestra info útil al crear los materiales

    CreateMaterials();
}

//Destructor: liberamos lo que creamos maunualmente (aunque Geant4 limpia al final)
MyMaterials::~MyMaterials(){

}

// Obtener instancia única (thread-safe básico)
MyMaterials* MyMaterials::GetInstance()
{
    if (fInstance == nullptr) {
        fInstance = new MyMaterials();
    }
    return fInstance;
}

//Método principal: aquí creas todos los materiales
void MyMaterials::CreateMaterials(){
    // 1. Materiales de NIST (rápidos y estándar)
    fAir =   fNistMan->FindOrBuildMaterial("G4_AIR");
    fWater = fNistMan->FindOrBuildMaterial("G4_WATER");

    // 2. Polystyrene (centellador clásico C8H8)
    G4double density = 1.050 * g / cm3;
    std::vector<G4String> elem_ps = {"C", "H"};
    std::vector<G4int> nat_ps = {8, 8};
    fPolystyrene = fNistMan->ConstructNewMaterial("Polystyrene", elem_ps, nat_ps, density);

    // 3. PMMA (para fibras ópticas)
    std::vector<G4String> elem_pmma = {"C", "H", "O"};
    std::vector<G4int>    nat_pmma  = {5, 8, 2};
    fPMMA = fNistMan->ConstructNewMaterial("PMMA", elem_pmma, nat_pmma, 1.190 * g / cm3);

    // Opcional: Agregar más propiedades ópticas (RINDEX, ABSLENGTH, SCINTILATIONCOMPONENT1, etc.)
    // Ejemplo básico para Polystyrene:
    auto* mpt = new G4MaterialPropertiesTable();

    std::vector<G4double> energy = {2.0*eV, 3.5*eV};
    std::vector<G4double> rindex = {1.59, 1.59};

    mpt->AddProperty("RINDEX", energy, rindex);
    std::vector<G4double> scint = {0.0, 1.0};
    mpt->AddProperty("SCINTILLATIONCOMPONENT1", energy, scint);
    mpt->AddConstProperty("SCINTILLATIONYIELD", 10000./MeV);
    fPolystyrene->SetMaterialPropertiesTable(mpt);

    //Se puede hacer lo mismo para PMMA, etc...
}

//Método para pedir cualquier material por nombre (múy útil)
G4Material* MyMaterials::GetMaterial(const G4String materialName){
    G4Material* mat = fNistMan->FindOrBuildMaterial(materialName);
    
    if(!mat) mat = G4Material::GetMaterial(materialName);

    if(!mat){
        G4ExceptionDescription ed;
        ed << "Material '" << materialName << "' no encontrado!";
        G4Exception("MyMaterials::GetMaterial", "", FatalException, ed);
    }

    return mat;
}