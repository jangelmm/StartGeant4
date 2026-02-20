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
    std::vector<G4double> energy = {
    2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV, 2.15 * eV, 2.18 * eV, 2.21 * eV,
    2.24 * eV, 2.27 * eV, 2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV, 2.42 * eV, 2.45 * eV,
    2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV, 2.60 * eV, 2.63 * eV, 2.66 * eV, 2.69 * eV,
    2.72 * eV, 2.75 * eV, 2.78 * eV, 2.81 * eV, 2.84 * eV, 2.87 * eV, 2.90 * eV, 2.93 * eV,
    2.96 * eV, 2.99 * eV, 3.02 * eV, 3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV,
    3.20 * eV, 3.23 * eV, 3.26 * eV, 3.29 * eV, 3.32 * eV, 3.35 * eV, 3.38 * eV, 3.41 * eV,
    3.44 * eV, 3.47 * eV};
    std::vector<G4double> energySmall = {2.0 * eV, 3.47 * eV};
    std::vector<G4double> refractiveIndexPS = {1.50, 1.50};
    std::vector<G4double> absPS = {2. * cm, 2. * cm};
    std::vector<G4double> scintilFast = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};



    auto mptPolystyrene = new G4MaterialPropertiesTable();
    mptPolystyrene->AddProperty("RINDEX", energySmall, refractiveIndexPS);
    mptPolystyrene->AddProperty("ABSLENGTH", energySmall, absPS);
    mptPolystyrene->AddProperty("SCINTILLATIONCOMPONENT1", energy, scintilFast);
    mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
    mptPolystyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptPolystyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10. * ns);

    fPolystyrene->SetMaterialPropertiesTable(mptPolystyrene);

    // Set the Birks Constant for the Polystyrene scintillator
    fPolystyrene->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

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