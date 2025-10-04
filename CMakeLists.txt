cmake_minimum_required(VERSION 3.20)
project(ProjetReseaux LANGUAGES CXX)

# --- Qt ---
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Boost classique ---
set(BOOST_ROOT "C:/boost_1_89_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")  # ou lib si tu as compilé
set(Boost_USE_STATIC_LIBS ON)    # ou OFF si tu veux shared
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)  # pour Boost_ROOT
find_package(Boost 1.89 REQUIRED COMPONENTS graph filesystem system)

if(Boost_FOUND)
    message(STATUS "Boost trouvé : ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost non trouvé !")
endif()

# --- Inclure les dossiers ---
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

# --- Qt modules ---
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

# --- Sources ---
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# --- Executable ---
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# --- Lier les libs ---
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    ${Boost_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
