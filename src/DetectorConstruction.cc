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
#include "G4SubtractionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

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
    auto* worldSolid = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    auto* worldLV    = new G4LogicalVolume(worldSolid, vacuum, "World");
    auto* worldPV    = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0);

    // ====================== CAJA ======================
    G4double caseX = 25.0 * cm;
    G4double caseY = 25.0 * cm;
    G4double caseZ = 5.0 * cm;

    auto* caseSolid = new G4Box("Case", caseX/2, caseY/2, caseZ/2);
    auto* caseLV    = new G4LogicalVolume(caseSolid, air, "CaseLV");
    auto* casePV    = new G4PVPlacement(nullptr, {}, caseLV, "CasePV", worldLV, false, 0);

    // ====================== DETECTOR MULTICAPA ======================
    G4double detX = 20.0 * cm;
    G4double detY = 20.0 * cm;
    G4double detZ = 1.0 * cm;

    // 1. Polystyrene (centelleador)
    auto* scintSolid = new G4Box("Scintillator", detX/2, detY/2, detZ/2);
    auto* scintLV    = new G4LogicalVolume(scintSolid, polystyrene, "ScintillatorLV");

    // 2. Teflón (reflector)
    G4double teflonThickness = 0.2 * mm;
    auto* teflonSolid = new G4Box("Teflon",
                                  (detX + 2*teflonThickness)/2,
                                  (detY + 2*teflonThickness)/2,
                                  (detZ + 2*teflonThickness)/2);
    auto* teflonLV = new G4LogicalVolume(teflonSolid, teflon, "TeflonLV");

    // 3. Cinta de aislar
    G4double tapeThickness = 0.3 * mm;
    auto* tapeSolid = new G4Box("Tape",
                                (detX + 2*teflonThickness + 2*tapeThickness)/2,
                                (detY + 2*teflonThickness + 2*tapeThickness)/2,
                                (detZ + 2*teflonThickness + 2*tapeThickness)/2);
    auto* tapeLV = new G4LogicalVolume(tapeSolid, tape, "TapeLV");

    // Colocamos las capas de dentro hacia fuera
    auto* scintPV = new G4PVPlacement(nullptr, {}, scintLV, "ScintillatorPV", teflonLV, false, 0);
    auto* teflonPV = new G4PVPlacement(nullptr, {}, teflonLV, "TeflonPV", tapeLV, false, 0);

    // Detector completo dentro de la caja
    G4ThreeVector detPosition(0, 0, -caseZ/2 + detZ/2 + 3.0*mm);
    auto* tapePV = new G4PVPlacement(nullptr, detPosition, tapeLV, "DetectorPV", caseLV, false, 0);

    // Silicio =============================================================================================================
    auto* silicon = materials->GetSilicon();
    G4double sensorX = 0.1 * cm;   // espesor en X
    G4double sensorY = 1.0 * cm;   // altura
    G4double sensorZ = 1.0 * cm;   // ancho

    auto* sensorSolid = new G4Box("SiliconSensor", sensorX/2, sensorY/2, sensorZ/2);
    auto* sensorLV = new G4LogicalVolume(sensorSolid, silicon, "SiliconSensorLV");

    // Posición: pegado a la cara lateral +X del centellador
    G4ThreeVector sensorPos(detX/2 - sensorX/2, 0, 0);
    auto* sensorPV = new G4PVPlacement(nullptr, sensorPos, sensorLV, "SiliconSensorPV", scintLV, false, 0);

    // Hacemos sensible el volumen del sensor
    fLogicTarget = sensorLV;


    // ================== SUPERFICIES ÓPTICAS ==================
    // 1. Superficie del centelleador (pulida)
    auto* opScint = new G4OpticalSurface("ScintSurface");
    opScint->SetType(dielectric_dielectric);
    opScint->SetModel(unified);
    opScint->SetFinish(polished);
    opScint->SetSigmaAlpha(0.0);

    // 2. Superficie del teflón (rugosa)
    auto* opTeflon = new G4OpticalSurface("TeflonSurface");
    opTeflon->SetType(dielectric_dielectric);
    opTeflon->SetModel(unified);
    opTeflon->SetFinish(ground);
    opTeflon->SetSigmaAlpha(0.1);

    // Asignar superficies a las fronteras (usando volúmenes físicos)
    new G4LogicalBorderSurface("ScintTeflonBorder", scintPV, teflonPV, opScint);
    new G4LogicalBorderSurface("TeflonTapeBorder", teflonPV, tapePV, opTeflon);

    // Vincular las tablas de MyMaterials a las superficies ópticas
    opScint->SetMaterialPropertiesTable(polystyrene->GetMaterialPropertiesTable());
    opTeflon->SetMaterialPropertiesTable(teflon->GetMaterialPropertiesTable());
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
