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
        - cannot be passed by reference due to template Args...
- MovableGameEntity functionality moved to separate components for better maintainability
- describe all the components
- decided not to do testing for time purposes
- xs around the txt map
- A* search for ghost pathfinding
- simplified collision component and collision system
    - bind functions on overlap 
- world holds the tile collision logic and loads the tiles
- factory class to create game entities
- menu
- music and sound, SoundManager
- collision system and component

## Necessary

## Good to have
- incorporate cherry somehow
- different ghost AI
- teleport tunnel in the middle

## Clean-ups
- string instead of char*
- get rid of my<> pattern
- filename constants
