#!/bin/bash

#echo "What program would you like to compile"
#read file
#g++ -c file
#g++ file.o o -o executables/main -lsfml-graphics -lsfml-window -lsfml-system
#-lsfml-network -lsfml-audio

g++ -c main.cpp
g++ main.o -o executables/main -lsfml-graphics -lsfml-window -lsfml-system
#-lsfml-network -lsfml-audio
