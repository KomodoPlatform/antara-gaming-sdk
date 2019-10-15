# Roadmap antara-gaming-sdk

## Core of the SDK

### Graphics (SFML)

- [ ] 1.0 Graphics (Total 20 hours)
    - [x] 1.1 Manipulation of a window, opening, closing, resize. Must work on all major platforms (**Linux**, **Windows**, **OSX**) And must also have a nice `CMake` integration to facilitate its use in external projects in a modern C++ 17 wrapper. (8 hours) (Design + Implementation)
    - [ ] 1.2 Ability to draw any type of drawable with a predefined list at compilation in the form of ecs and not object-oriented, must work on all platforms in a modern C++ 17 wrapper. (8 hours) (Design + Implementation)
    - [ ] 1.3 Technical Documentation (4 Hours) 

### Input (SFML)

- [ ] 1.0 Input (Total 20 Hours)
    - [x] 1.1 Manipulation of input, through a keyboard or a mouse, automatic transmission of events inputs to other parts of the program, must work on all platforms And must also have a nice `CMake` integration to facilitate its use in external projects. in a modern C++ 17 wrapper. (16 hours) (Design + Implementation)
    - [ ] 1.2 Technical Documentation (4 Hours)

### Audio (SFML)

- [ ] 1.0 Audio (Total 20 Hours)
    - [ ] 1.1 Have the ability to play a sound effect, a sound with a position, a music, be able to repeat a music, choose the volume. Must work on all platforms in a modern C++ 17 wrapper. And must also have a nice `CMake` integration to facilitate its use in external projects (16 hours) (Design + Implementation)
    - [ ] 1.2 Technical Documentation (4 Hours)

### Config

- [ ] 1.0 Config (Total 6 Hours)
    - [x] 1.1 Configuration json of the game to choose the title of the game, the size of the screen, if you want a screen fullscreen etc. Possibility to have access to this configuration from the ecs (2 hours)
    - [ ] 1.2 User Documentation (How to customize our configuration) (4 Hours)

### Core

- [ ] 1.0 Core (Total 52 Hours)
    - [ ] 1.1 Contains all the core of the projects such as function utilities (48 Hours) (Design + Implementation)
        - [x] 1.2 Filesystem
        - [x] 1.3 Versionning
        - [ ] 1.4 Security Functionnality
        - [x] 1.5 Reflection
        - [ ] 1.6 Collection of algorithm
        - [x] 1.7 Modern CMake compliant.
        - [x] 1.8 Unit Tests
    - [ ] 1.9 Technical Documentation (Documentation of the functions and how to use them + Choice of design) (4 Hours)

### ECS (Entity Component System)

- [x] 1.0 ECS (100 hours)
    - [x] 1.1 Full system implementation for the whole logic of the game, optimization compile time + CMake integration + Cross platform + Unit tests + Design + Implementation (96 Hours)
        - [x] 1.2 Modern CMake Compliant
        - [x] 1.3 System Manager With possibility of Update, Delete, Add, Remove a system, Update all systems, and update a category of systems.
        - [x] 1.4 Systems class with possibility of Update, Disable, Enable, Get a Name of a system
        - [x] 1.5 Systems category (pre_update, post_update, logic_update)
        - [x] 1.6 Unit Tests
        - [x] 1.7 Common Game Components (Position, Scaling, Transform, Layer, etc...)
    - [x] 1.8 Technical Documentation (4 Hours)
    
### Event

- [ ] 1.0 Event (20 hours)
   - [ ] 1.1 Full event system implementation + transmission of all kind of events (input, audio, config) + Design + Implementation (16 hours)
        - [ ] 1.2 Common Game Event (Key Pressed, Key Released, Button Pressed, Button Released, Mouse Moved, Quit Game, Start Game, etc...)
        - [ ] 1.3 Modern CMake Compliant
        - [ ] 1.4 Unit Tests
        - [ ] 1.5 Type Traits
   - [ ] 1.6 Technical Documentation (4 Hours)
   
### Scripting

#### Lua

- [ ] 1.0 Lua (100 Hours)
   - [ ] 1.1 Full implementation and bridge with lua to create games and game logic, systems, events, input, graphics etc Design + Implementation (In the details, it is about having the possibility to create a system in lua, an entity, or even a game scene all from a script with the same functionalities C++ explained previously) (96 hours)
        - [ ] 1.2 Lua Game Entities
        - [ ] 1.3 Lua Components
        - [ ] 1.4 Lua Logic System
        - [ ] 1.5 Lua Scripted System
        - [ ] 1.6 Lua Scene Manager
        - [ ] 1.7 Modern CMake Compliant
        - [ ] 1.8 Lua Input Replication
        - [ ] 1.9 Unit Tests
   - [ ] 1.10 Technical Documentation (4 Hours)
   
### Scenes

- [ ] 1.0 Scenes (52 Hours)
   - [ ] 1.1 Full Scenes systems + Transition Design + Implementation (For the scene module it is a scene manager + a class base_scene with the possibility to change scene, to forward events automatically to the current scene etc) (48Hours)
        - [ ] 1.2 Modern CMake Compliant
        - [ ] 1.3 Unit Tests
        - [ ] 1.4 Scenes Manager (Add Scenes, Remove Scenes, Change Scenes, Forward Event to the current scene)
        - [ ] 1.5 Scenes Abstract Class (Updates, possibility to have a system manager)
        - [ ] 1.6 Possibility of communicating with the Scene manager through events.
   - [ ] 1.7 Technical Documentation (4 Hours)

### World

- [ ] 1.0 World (28 Hours)
   - [ ] 1.1 Full World App Implementation +  Design + Implementation (The module world is a module allowing the creation easily of a game regrouping the preceding modules through a world class one will be able to implement a constructor, to charge the systems which need for a game) (24 Hours)
        - [ ] 1.2 Modern CMake Compliant
        - [ ] 1.3 Unit Tests
        - [ ] 1.4 World abstraction class with a run member function and game loop, possibility to run with emscripten
   - [ ] 1.4 Technical Documentation (4 Hours)

### Timer

- [ ] 1.0 Timer (9 Hours)
   - [ ] 1.1 Timer Implementation that require handle of 60, 120 and 144 fps with fixed delta time + Design + Implementation (8 Hours)
   - [ ] 1.2 Technical Documentation (1Hours)
