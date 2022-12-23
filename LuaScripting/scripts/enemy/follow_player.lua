local engine = require("LuaScripting/engine");
local vlib = engine.vector;


return function(worldData, objectID)

  local player_pos = worldData.positions[1];

  local pos = worldData.positions[objectID];
  local vel = worldData.velocities[objectID];


  local dist = vlib.dist(pos, player_pos);

  if dist > 3.5 then
    local dir = vlib.normalise(player_pos - pos) / 25.0;
    pos.x = pos.x + dir.x;
	pos.z = pos.z + dir.z;
  end;  

  if dist < 3.0 then
    local dir = vlib.normalise(pos - player_pos) / 25.0;
    vlib.add(vel, dir);
  end;

end;
