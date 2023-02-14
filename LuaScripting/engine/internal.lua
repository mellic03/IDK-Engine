
local function vectorinit()
  return {x = 0, y = 0, z = 0};
end


PlayerID = 1;

-- luaLibrary_gameobject
-----------------------------------------------
function CE_GameObject_SetPos(objectID, x, y, z) end;
function CE_GameObject_GetPos(objectID) return vectorinit(); end;
function CE_GameObject_GetPos_worldspace(objectID) return vectorinit(); end;
function CE_GameObject_SetVel(objectID, x, y, z) end;
function CE_GameObject_GetVel(objectID) return vectorinit(); end;
function CE_GameObject_AddVel(objectID, x, y, z) end;
function CE_GameObject_AddRot(objectID, x, y, z) end;
function CE_GameObject_SetPath(objectID, x, y, z) end;


function CE_GameObject_SetInt   ( objectID,  name,  value ) end;
function CE_GameObject_GetInt   ( objectID,  name ) end;

function CE_GameObject_SetFloat ( objectID,  name,  value ) end;
function CE_GameObject_GetFloat ( objectID,  name ) end;

function CE_GameObject_SetString( objectID,  name,  value ) end;
function CE_GameObject_GetString( objectID,  name ) end;


function CE_GameObject_SetAnimation( objectID, animationName ) end;
function CE_GameObject_GetAnimation( objectID ) return ""; end;

function CE_GameObject_GetNavState( objectID ) return ""; end;


function CE_GameObject_SetParent( childID, parentID ) end;
function CE_GameObject_ClearParent( childID ) end;
function CE_GameObject_GetParentID( childID ) end;

function CE_GameObject_InBoundingBox( objectID, x, y, z ) end;

function CE_GameObject_GetFront ( objectID ) return vectorinit(); end;
-----------------------------------------------


-- luaLibrary_lighting
-----------------------------------------------
function CE_Lighting_SetBloomMinResolution(resolution) end;
function CE_Lighting_SetBloomThreshold(threshold) end;
function CE_Lighting_SetBloomStrength(strength) end;
-----------------------------------------------


-- luaLibrary_physics
-----------------------------------------------
function CE_Physics_SetGravity(force) end;
-----------------------------------------------


-- luaLibrary_player
-----------------------------------------------
function CE_Player_SetJumpForce(force) end;
function CE_Player_SetMoveForce(force) end;
function CE_Player_SetFly(bool) end;
function CE_Player_GetDir() return vectorinit(); end;
function CE_Player_GetID() end;
-----------------------------------------------


-- luaLibrary_scene
-----------------------------------------------
function CE_Scene_LoadScene(filepath) end;
function CE_Scene_ClearColor(r, g, b) end;
-----------------------------------------------
