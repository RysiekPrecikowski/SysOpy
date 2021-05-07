cmake_minimum_required(VERSION 3.17)
project(lab2 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab2 main.c "zad1 (odczytywanie na zmiane linijek)/main.c" "zad2 (wyszukiwanie wierszy ze znakiem)/main.c" "zad3 (podział liczb)/main.c" "zad4 (replace w pliku)/main.c" "zad5 (łamanie linijek)/main.c")