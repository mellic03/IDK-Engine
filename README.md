# IDK Engine

Game engine wee woo text goes here boom bam.

# Contents
- [Contents](#Contents)
- [Editor](#Editor)
- [Lua API](#Lua-API)


# Purpose of Branch
This branch exists to refactor all code written between November 2022
and March 2023.

**Checklist**:
- [ ] Implement new base class GameObject. User can then define
    GameObject-derived classes in their project
    ``` C++
    // GameObject.h ---------------------------------
    namespace gameobject
    {
        class GameObject;
    };
    // ----------------------------------------------
    
    // SomeFile.h -----------------------------------
    Class gameobject::Actor: public GameObject;
    Class gameobject::Terrain: public GameObject;
    // ----------------------------------------------
    ```

- [ ] Make engine more easily used as a base project.
    ``` C++
    engine.beginFrame() // Initialise/clear frame.
    engine.perFrame()   // Defined by user.
    engine.endFrame()   // Draw frame to screen.
    ```




# Editor
- The editor can be broken up into the following windows:
    - [Viewport](#Viewport)
    - [Scene Hierarchy](#Scene-Hierarchy)
    - [Render](#Render)
    - [Physics](#Physics)
    - [Properties](#Properties)
    - [Script Browser](#Script-Browser)
    - [Script Editor](#Script-Editor)



# Editor Windows

## Viewport
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.

## Scene Hierarchy
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.

## Render
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.


## Physics
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.


## Properties
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.


## Script Browser
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.


# Lua API
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.

