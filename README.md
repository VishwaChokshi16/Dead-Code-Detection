# Dead Code Detection and Elimination System

## Subject
Compiler Design

## Project Description
This project implements a Dead Code Detection and Elimination
System using C++.

It identifies unused and overwritten assignments using
backward liveness analysis and generates optimized output.

## Technologies Used
- C++
- MSYS2 UCRT64
- GCC Compiler
- C++17

## Project Modules
1. Parser
2. Dead Code Detector
3. Code Optimizer
4. Main Integration Module

## Project Files
- parser.h
- parser.cpp
- detector.h
- detector.cpp
- optimizer.h
- optimizer.cpp
- main.cpp
- input.txt

## Compilation

g++ -std=c++17 -Wall -Wextra main.cpp parser.cpp detector.cpp optimizer.cpp -o deadcode.exe

## Execution

./deadcode.exe

## Output
The program displays detected dead assignments and generates
an optimized.txt file.

## Team Members
1. Vishwa Chokshi
2. Harshil Dhamat
3. Mitanshu Dholakiya

## Subject
Compiler Design
