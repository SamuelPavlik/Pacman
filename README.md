# Pacman Doc
Simple 2D Pacman
- init all map in InitPathmap with dots and all
- pointers to shared pointers
- constants class
- compiled with C++ 17 for inline
- created component system for sprites, no need to define them in entities' childrens
- added EntityCollection for entity adding, update and removal
- C for components, S for systems
- members which should have just one instance are kept as raw pointers like Input 
otherwise their smart pointers like entities
- MovableGameEntity functionality moved to separate components for better maintainability

## Necessary
- pacman turning and animation

## Good to have
- ghost movement in separate component
- ghost returning to cage
- pathfinding looking for pacman
- incorporate cherry somehow
- sound
- multiple ghosts
- teleport tunnel in the middle

## Clean-ups
- sort #pragma once and #ifndef
- all file names to constants
- check unneccessary includes
