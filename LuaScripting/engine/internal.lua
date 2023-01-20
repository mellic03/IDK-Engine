
local function vectorinit()
  return {x = 0, y = 0, z = 0};
end


-- luaLibrary_gameobject
-----------------------------------------------
function CE_GameObject_GetPos(objectID) return vectorinit(); end;
function CE_GameObject_SetPos(objectID, x, y, z) end;
function CE_GameObject_GetVel(objectID) return vectorinit(); end;
function CE_GameObject_SetVel(objectID, x, y, z) end;
function CE_GameObject_AddRot(objectID, x, y, z) end;
-----------------------------------------------


-- luaLibrary_lighting
-----------------------------------------------
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
-----------------------------------------------


-- luaLibrary_scene
-----------------------------------------------
function CE_Scene_LoadScene(filepath) end;
function CE_Scene_ClearColor(r, g, b) end;
-----------------------------------------------
