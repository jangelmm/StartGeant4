#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction() = default;
DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume* DetectorConstruction::Construct() {

    auto* nist = G4NistManager::Instance();                //Cátologo de materiales
    auto* air = nist->FindOrBuildMaterial("G4_AIR");       //Seleccionamos Aire 
    auto* water = nist->FindOrBuildMaterial("G4_WATER");   //Seleccionamos Agua

    // ================================= EJEMPLO CREACION MATERIAL - CASO 1
    //Polystyrene (C8H8)
    G4double density = 1.050 * g / cm3;
    std::vector<G4String> elements = {"C", "H"}; //Carbono, Hidrogeno
    std::vector<G4int> natoms = {8,8}; //8 De Carbono, 8 de Hidrogeno
    auto* polystyrene = nist->ConstructNewMaterial("Polystyrene", elements, natoms, density); //Basta con reemplazar en G4LogicalVolume
    
    // ================================= MUNDO ====================================
    G4double worldSize = 2.0 * m;                                                        //worldSize será el - Tamaño total 1m
    auto* worldSolid = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);        //worlSolid será el - Cubo(nombre, mitad anchoX , mitad alturaY, mitad profundidadZ)
    auto* worldLV = new G4LogicalVolume(worldSolid, air, "World");                       //worldLV será el   - VolumenLogico(Cubo, elemento, nombre)
    auto* worldPV = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0); //worldPV será el   - Espacio(rotacion, posicion {} es default, VolumenLogico, nombre, PL_madre, copia_multiple, numero_copia)

    // ================================= DETECTOR

    // Blanco
    G4double targetSize = 10.0 * cm;                                                     //targetSize será el  -  Tamaño total 10 cm
    auto* targetSolid = new G4Box("Target", targetSize/2, targetSize/2, targetSize/2);   //targetSolid será el -  Cubo(nombre, mitad anchoX , mitad alturaY, mitad profundidadZ)
    fLogicTarget = new G4LogicalVolume(targetSolid, polystyrene, "Target");                    //flogicalTarget será el-VolumenLogico(Cubo, elemento, nombre)
    new G4PVPlacement(nullptr, {}, fLogicTarget, "Target", worldLV, false, 0);           //No se crea            - Espacio(rotacion, posicion {} es default, VolumenLogico, nombre, PL_madre, copia_multiple, numero_copia)

    
    // ================================= MODIFCACIONES 

    //Alternativas o cambios (Práctica)
    //G4RotationMatrix* rot = new G4RotationMatrix(); rot->rotateZ(45*deg);                                      //Rotar el cubo
    //new G4PVPlacement(rot, G4ThreeVector(0,10*cm, 0), fLogicTarget, "Target", worldLV, false, 0);          //Mover 10 cm hacia arriba


    return worldPV; //Se devuelve el mundo
}