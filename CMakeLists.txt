cmake_minimum_required(VERSION 3.16)
project(ProjetReseaux)

# ------------------------------
# C++17
# ------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# Qt6 MinGW
# ------------------------------
set(CMAKE_PREFIX_PATH "E:/Qt/6.9.3/mingw_64")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL Network)

# ------------------------------
# Activer AUTOMOC, AUTORCC, AUTOUIC
# ------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ------------------------------
# Executable
# ------------------------------
add_executable(${PROJECT_NAME}
    src/frontend/MainWindow.cpp
    src/frontend/MainWindow.h
    src/backend/main.cpp
    src/frontend/mapwidgetosm.cpp
    src/frontend/mapwidgetosm.h
    src/backend/vehicule.h src/backend/vehicule.cpp
    src/backend/simulateur.h src/backend/simulateur.cpp
    src/backend/grapheinterference.h src/backend/grapheinterference.cpp
    src/backend/liencommunication.h src/backend/liencommunication.cpp
    src/backend/Arete.cpp src/backend/Arete.h src/backend/GrapheRoutier.cpp src/backend/GrapheRoutier.h src/backend/Noeud.cpp src/backend/Noeud.h
    src/backend/gestiontemps.h src/backend/gestiontemps.cpp

)

add_executable(TestSimulation
    src/backend/main_test_simulation_console.cpp
    src/backend/vehicule.h src/backend/vehicule.cpp
    src/backend/simulateur.h src/backend/simulateur.cpp
    src/backend/grapheinterference.h src/backend/grapheinterference.cpp
    src/backend/liencommunication.h src/backend/liencommunication.cpp
    src/backend/Arete.cpp src/backend/Arete.h src/backend/GrapheRoutier.cpp src/backend/GrapheRoutier.h src/backend/Noeud.cpp src/backend/Noeud.h
)

# ------------------------------
# Liaisons bibliothèques
# ------------------------------
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::Network
)
