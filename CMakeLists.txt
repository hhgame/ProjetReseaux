cmake_minimum_required(VERSION 3.16)
project(ProjetReseaux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# Qt6
# ------------------------------
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL Network)

# ------------------------------
# OpenStreetMap via tuiles (tout intégré)
# ------------------------------
# On utilise QNetworkAccessManager pour télécharger les tuiles directement
# Pas besoin de libosmscout ni de GLFW/GLEW/glm

# ------------------------------
# Executable
# ------------------------------
add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.cpp
    MainWindow.h
    # ajoute ici tous tes fichiers
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
