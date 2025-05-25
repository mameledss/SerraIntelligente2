cmake_minimum_required(VERSION 3.29)
project(SerraIntelligente2)

set(CMAKE_CXX_STANDARD 20)

# Includi la directory degli header
include_directories(include)

# Trova tutti i file sorgente nella directory src
file(GLOB SOURCES "src/*.cpp")

# Crea l'eseguibile con tutti i file sorgente
add_executable(SerraIntelligente2 ${SOURCES}
        src/impiantoMediterraneo.cpp
        src/impiantoAlpino.cpp)