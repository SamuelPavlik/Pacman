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
- describe all the components
- decided not to do testing for time purposes

## Necessary
- ghost animation comp
- ghost movement comp (possible parent to keyboard comp too)

## Good to have
- ghost returning to cage
- pathfinding looking for pacman
- incorporate cherry somehow
- sound
- multiple ghosts
- teleport tunnel in the middle
- don't stop when not open road and pressing in dir
- make avatar invulnerable after respawn for a while

## Clean-ups
- sort #pragma once and #ifndef
- all file names to constants
- check unneccessary includes
