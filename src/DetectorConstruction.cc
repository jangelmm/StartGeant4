#include "DetectorConstruction.hh"
#include "MyMaterials.hh"
#include "TargetSD.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh" 
#include "G4SDManager.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"

DetectorConstruction::DetectorConstruction() = default;
DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto* nist = G4NistManager::Instance();
    auto* vacuum = nist->FindOrBuildMaterial("G4_Galactic");

    MyMaterials* materials = MyMaterials::GetInstance();
    auto* air         = materials->GetAir();
    auto* polystyrene = materials->GetPolystyrene();
    auto* teflon      = materials->GetTeflon();
    auto* tape        = materials->GetTape();

    // ====================== MUNDO ======================
    G4double worldSize = 2.0 * m;
    auto* worldSolid = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);            //Forma del mundo
    auto* worldLV    = new G4LogicalVolume(worldSolid, vacuum, "World");                     //Volumen logico
    auto* worldPV    = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0);  //Colocar en el espacio

    // ====================== CUERPO B (IMPORTANTE: Pedir datos al Doc de dimensiones, ya que esto es temporal) ======================
    G4double caseX = 25.0 * cm;
    G4double caseY = 25.0 * cm;
    G4double caseZ =  5.0 * cm;  

    auto* caseSolid = new G4Box("Case", caseX/2, caseY/2, caseZ/2);                          //Forma
    auto* caseLV    = new G4LogicalVolume(caseSolid, air, "CaseLV");                         //Volumen Logico

    new G4PVPlacement(nullptr, {}, caseLV, "CasePV", worldLV, false, 0);                     //Colocamos en el centro

    // ====================== CUERPO A (detector multicapa) ======================
    G4double detX = 20.0 * cm;
    G4double detY = 20.0 * cm;
    G4double detZ =  1.0 * cm;

    //IMPORTANTE: Pedir datos de que medida tendrán estas capas.

    // 1. Polystyrene (centelleador) - núcleo
    auto* scintSolid = new G4Box("Scintillator", detX/2, detY/2, detZ/2);                    //Forma
    auto* scintLV    = new G4LogicalVolume(scintSolid, polystyrene, "ScintillatorLV");       //Volumen Logico

    // 2. Teflón (reflector)
    G4double teflonThickness = 0.2 * mm;
    auto* teflonSolid = new G4Box("Teflon",                                                 //Una caja más grande
                                  (detX + 2*teflonThickness)/2,                             //Lado X del centellador + 0.2 mm
                                  (detY + 2*teflonThickness)/2,                             //Lado Y del centellador + 0.2 mm 
                                  (detZ + 2*teflonThickness)/2);                            //Lado Z del centellador + 0.2 mm
    auto* teflonLV = new G4LogicalVolume(teflonSolid, teflon, "TeflonLV");                  //Volumen Logico

    // 3. Cinta de aislar (protección exterior) - capa más externa (Lo mismo de arriba jeje)
    G4double tapeThickness = 0.3 * mm;
    auto* tapeSolid = new G4Box("Tape", 
                                (detX + 2*teflonThickness + 2*tapeThickness)/2,
                                (detY + 2*teflonThickness + 2*tapeThickness)/2,
                                (detZ + 2*teflonThickness + 2*tapeThickness)/2);
    auto* tapeLV = new G4LogicalVolume(tapeSolid, tape, "TapeLV");

    // Colocamos las capas de dentro hacia fuera                                             //"El hijo reemplaza a la madre", por lo que el espacio de la madre, la sustituye el hijo
    new G4PVPlacement(nullptr, {}, scintLV,  "ScintillatorPV", teflonLV, false, 0);          //El centelleador vive dentro del teflon
    new G4PVPlacement(nullptr, {}, teflonLV, "TeflonPV",       tapeLV,   false, 0);          //El teflon vive dentro de la cinta

    // Colocamos el detector completo dentro de la caja 
    G4ThreeVector detPosition(0, 0, -caseZ/2 + detZ/2 + 3.0*mm);  // toca la base de 3 mm
    new G4PVPlacement(nullptr, detPosition, tapeLV, "DetectorPV", caseLV, false, 0);

    // ====================== VOLUMEN SENSIBLE ======================
    fLogicTarget = scintLV;   

    return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
    if (fTargetSD.Get() == nullptr) {
        fTargetSD.Put(new TargetSD("TargetSD"));
    }

    G4SDManager::GetSDMpointer()->AddNewDetector(fTargetSD.Get());

    if (fLogicTarget) {
        SetSensitiveDetector(fLogicTarget, fTargetSD.Get());
        G4cout << "»»» TargetSD adjuntado correctamente al volumen 'Target'" << G4endl;
    }
}