#include "DetectorConstruction.hh"
#include "MyMaterials.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
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

    // Silicio (El SiPM de 6x6 mm) =========================================================
    auto* silicon = materials->GetSilicon();
    G4double sensorX = 0.1 * mm;   // Espesor mínimo solo para que exista un volumen frontera
    G4double sensorY = 6.0 * mm;   // 6 mm de altura según datasheet MicroFC-60035
    G4double sensorZ = 6.0 * mm;   // 6 mm de ancho según datasheet MicroFC-60035

    auto* sensorSolid = new G4Box("SiliconSensor", sensorX/2, sensorY/2, sensorZ/2);
    auto* sensorLV = new G4LogicalVolume(sensorSolid, silicon, "SiliconSensorLV");

    // Posición: pegado a la cara lateral +X del centellador (hacia adentro para evitar overlaps)
    G4ThreeVector sensorPos(detX/2 - sensorX/2, 0, 0);
    auto* sensorPV = new G4PVPlacement(nullptr, sensorPos, sensorLV, "SiliconSensorPV", scintLV, false, 0);


    // ================== SUPERFICIES ÓPTICAS ==================
    
    // Frontera: Centelleador -> Teflón
    auto* opScintTeflon = new G4OpticalSurface("ScintTeflonSurface");
    opScintTeflon->SetType(dielectric_dielectric);
    opScintTeflon->SetModel(unified);
    
    // CLAVE: "ground" (rugoso para reflexión difusa) + "frontpainted" (refleja directo en la frontera)
    opScintTeflon->SetFinish(groundfrontpainted); 
    opScintTeflon->SetSigmaAlpha(0.1); // Nivel de dispersión de la rugosidad

    // Le decimos a LA FRONTERA que refleje el 98% de la luz
    auto* mptScintTeflon = new G4MaterialPropertiesTable();
    std::vector<G4double> energy = {2.0*eV, 3.5*eV};
    std::vector<G4double> reflectivity = {0.98, 0.98}; 
    mptScintTeflon->AddProperty("REFLECTIVITY", energy, reflectivity);
    
    opScintTeflon->SetMaterialPropertiesTable(mptScintTeflon);

    // Asignar la superficie SÓLO a la frontera entre el cristal y el teflón
    new G4LogicalBorderSurface("ScintTeflonBorder", scintPV, teflonPV, opScintTeflon);

    // NOTA: No creamos frontera para la cinta. Como le quitamos las propiedades ópticas 
    // en MyMaterials, absorberá por defecto todo lo que la toque.

    // Vincular las tablas de MyMaterials a las superficies ópticas

    return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
    // INTENCIONALMENTE VACÍO
    // Ya no usamos TargetSD ni G4SDManager aquí. 
    // La detección de fotones (con su eficiencia cuántica) se gestiona de forma física y manual
    // interceptando las partículas al momento en que entran al 'SiliconSensorPV' 
    // dentro de la clase SteppingAction.
}