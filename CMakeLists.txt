cmake_minimum_required(VERSION 3.29)
project(SerraIntelligente2)

set(CMAKE_CXX_STANDARD 20)

include_directories(include) #include la directory degli header

file(GLOB SOURCES "src/*.cpp") #trova tutti i file sorgente nella directory src

add_executable(SerraIntelligente2 ${SOURCES}) #crea l'eseguibile con tutti i file sorgente