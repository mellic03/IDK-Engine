# CoomEngine

Game engine wee woo text goes here boom bam.

# Contents
- [Contents](#Contents)
- [Editor](#Editor)
- [Lua API](#Lua-API)



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


## Script Editor
Some placeholder text.
Some placeholder text.
Some placeholder text.
Some placeholder text.



# Lua API

## Structure
- Scene data can be accessed in Lua through the <u>worldData</u> object.

    > /LuaScripting/main.lua
    ```Lua
    worldData = {

      deltaTime = 0.01f,
      positions  = { {x, y, z}, {x, y, z}, ..., {x, y, z} },
      velocities = { {x, y, z}, {x, y, z}, ..., {x, y, z} }

    };
    ```




## Default Script
- A script containing the neccessary boilerplate to access scene data can be found in the scripts folder.
    > /LuaScripting/scripts/default.lua

    ```Lua
    local engine = require("LuaScripting/engine");
    local vlib  = engine.vectorLibrary;
    local iolib = engine.ioLibrary;

    return function(worldData, objectID)

      local pos = worldData.positions[objectID];
      local vel = worldData.velocities[objectID];

      -- An example of applying gravity an object.
      -- vel.y = vel.y - (2.0 * worldData.deltaTime);

    end;
    ```
- A given attribute of a specific game object is obtained by accessing worldData.attribute[objectID]. The object ID is automatically supplied by main.lua, but the object ID of other objects can be seen in-engine under "info" in the properties panel of an object. Some Object ID's are also constant, the player for example always has ID 1 and can therefore always be accessed through worldData.attribute[1];


