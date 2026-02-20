#include "DetectorConstruction.hh"
#include "MyMaterials.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh" //Caso 2

DetectorConstruction::DetectorConstruction() = default;
DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume* DetectorConstruction::Construct() {


    auto* nist = G4NistManager::Instance();
    auto* vaccum = nist->FindOrBuildMaterial("G4_Galactic");

    //Obtenemos la instancia única de materiales
    MyMaterials* materials = MyMaterials::GetInstance();

    //Pedimos los materiales que necesitamos
    auto* air         = materials->GetAir();
    auto* polystyrene = materials->GetPolystyrene();
    //auto* puma        = materials->GetPMMA();
    
    // ================================= MUNDO ====================================
    G4double worldSize = 2.0 * m;                                                        //worldSize será el - Tamaño total 1m
    auto* worldSolid = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);        //worlSolid será el - Cubo(nombre, mitad anchoX , mitad alturaY, mitad profundidadZ)
    auto* worldLV = new G4LogicalVolume(worldSolid, vaccum, "World");                       //worldLV será el   - VolumenLogico(Cubo, elemento, nombre)
    auto* worldPV = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0); //worldPV será el   - Espacio(rotacion, posicion {} es default, VolumenLogico, nombre, PL_madre, copia_multiple, numero_copia)

    
    
    // ================================= DETECTOR

    // Detector (construido con Polysterene con centello)
    G4double targetSize = 10.0 * cm;                                                     //targetSize será el  -  Tamaño total 10 cm
    auto* targetSolid = new G4Box("Target", targetSize/2, targetSize/2, targetSize/2);   //targetSolid será el -  Cubo(nombre, mitad anchoX , mitad alturaY, mitad profundidadZ)
    fLogicTarget = new G4LogicalVolume(targetSolid, polystyrene, "Target");                    //flogicalTarget será el-VolumenLogico(Cubo, elemento, nombre)
    new G4PVPlacement(nullptr, {}, fLogicTarget, "Target", worldLV, false, 0);           //No se crea            - Espacio(rotacion, posicion {} es default, VolumenLogico, nombre, PL_madre, copia_multiple, numero_copia)


    return worldPV; //Se devuelve el mundo
}