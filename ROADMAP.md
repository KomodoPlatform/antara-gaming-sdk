# Roadmap antara-gaming-sdk

## Core of the SDK

### Graphics (SFML)

- [ ] 1.0 Graphics (Total 20 hours)
    - [ ] 1.1 Manipulation of a window, opening, closing, resize. Must work on all major platforms (**Linux**, **Windows**, **OSX**) And must also have a nice `CMake` integration to facilitate its use in external projects in a modern C++ 17 wrapper. (8 hours) (Design + Implementation)
    - [ ] 1.2 Ability to draw any type of drawable with a predefined list at compilation in the form of ecs and not object-oriented, must work on all platforms in a modern C++ 17 wrapper. (8 hours) (Design + Implementation)
    - [ ] 1.3 Technical Documentation (4 Hours) 

### Input (SFML)

- [ ] 1.0 Input (Total 20 Hours)
    - [ ] 1.1 Manipulation of input, through a keyboard or a mouse, automatic transmission of events inputs to other parts of the program, must work on all platforms And must also have a nice `CMake` integration to facilitate its use in external projects. in a modern C++ 17 wrapper. (16 hours) (Design + Implementation)
    - [ ] 1.2 Technical Documentation (4 Hours)

### Audio (SFML)

- [ ] 1.0 Audio (Total 20 Hours)
    - [ ] 1.1 Have the ability to play a sound effect, a sound with a position, a music, be able to repeat a music, choose the volume. Must work on all platforms in a modern C++ 17 wrapper. And must also have a nice `CMake` integration to facilitate its use in external projects (16 hours) (Design + Implementation)
    - [ ] 1.2 Technical Documentation (4 Hours)

### Config

- [ ] 1.0 Config (Total 2 Hours)
    - [ ] 1.1 Configuration json of the game to choose the title of the game, the size of the screen, if you want a screen fullscreen etc. Possibility to have access to this configuration from the ecs (2 hours)
    - [ ] 1.2 Technical Documentation (4 Hours)

### Core

- [ ] 1.0 Core (Total 52 Hours)
    - [ ] 1.1 Contains all the core of the projects such as function utilities (real asset path, versionning, safe headers, etc...) (48 Hours) (Design + Implementation)
    - [ ] 1.2 Technical Documentation (4 Hours)

### ECS (Entity Component System)

- [ ] 1.0 ECS (100 hours)
    - [ ] 1.1 Full system implementation for the whole logic of the game, optimization compile time + CMake integration + Cross platform + Unit tests + Design + Implementation (96 Hours)
    - [ ] 1.2 Technical Documentation (4 Hours)
    
### Event

- [ ] 1.0 Event (20 hours)
   - [ ] 1.1 Full event system implementation + transmission of all kind of events (input, audio, config) + Design + Implementation (16 hours)
   - [ ] 1.2 Technical Documentation (4 Hours)
   
### Scripting

#### Lua

- [ ] 1.0 Lua (100 Hours)
   - [ ] 1.1 Full implementation and bridge with lua to create games and game logic, systems, events, input, graphics etc Design + Implementation (96 hours)
   - [ ] 1.2 Technical Documentation (4 Hours)
   
### Scenes

- [ ] 1.0 Lua (52 Hours)
   - [ ] 1.1 Full Scenes systems + Transition Design + Implementation (48Hours)
   - [ ] 1.2 Technical Documentation (4 Hours)

### World

- [ ] 1.0 World (28 Hours)
   - [ ] 1.1 Full World App Implementation +  Design + Implementation (24 Hours)
   - [ ] 1.2 Technical Documentation (4 Hours)

### Timer

- [ ] 1.0 Timer (9 Hours)
   - [ ] 1.1 Timer Implementation that require handle of 60, 120 and 144 fps with fixed delta time + Design + Implementation (8 Hours)
   - [ ] 1.2 Technical Documentation (1Hours)