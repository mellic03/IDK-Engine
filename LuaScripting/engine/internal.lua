
local function v_init()
  return {x = 0, y = 0, z = 0};
end

-- Return the position of the object with ID objectID
function GetPos(objectID) return v_init(); end;

-- Set the position of the object with ID objectID
function SetPos(objectID, x, y, z) end;

-- Return the velocity of the object with ID objectID
function GetVel(objectID) return v_init(); end;

-- Set the velocity of the object with ID objectID
function SetVel(objectID, x, y, z) end;

function SetJumpForce(force) end;
function SetMoveForce(force) end;

function SetGravity(force) end;

-- Load a scene from filepath where filepath is relative to assets/scenes/
function LoadScene(filepath) end;


----------------------------------------
