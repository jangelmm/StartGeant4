cmake_minimum_required(VERSION 3.10)
project(startG4)

# Busca Geant4 con visualización
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# ROOT (opcional)
find_package(ROOT)
if(ROOT_FOUND)
    include(${ROOT_USE_FILE})
    add_definitions(-DWITH_ROOT)
endif()

# Fuentes y cabeceras
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Crea ejecutable
add_executable(startG4 main.cc ${sources} ${headers})
target_include_directories(startG4 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(startG4 PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Copia run.mac al build/
set(SCRIPTS run.mac)
foreach(_script ${SCRIPTS})
    configure_file(${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_BINARY_DIR}/${_script} COPYONLY)
endforeach()