local engine = require("LuaScripting/engine");
local vlib = engine.vectorLibrary;


return function(worldData, objectID)

  local player_pos = worldData.positions[1];

  local pos = worldData.positions[objectID];
  local vel = worldData.velocities[objectID];


  local dist = vlib.dist(pos, player_pos);

  if dist > 3.5 then
    local dir = vlib.normalise(player_pos - pos) / 25.0;
    vel.x = vel.x + dir.x * 200 * worldData.deltaTime;
	  vel.z = vel.z + dir.z * 200 * worldData.deltaTime;
  end;  

  if dist < 3.0 then
    local dir = vlib.normalise(pos - player_pos) / 25.0;
    vlib.add(vel, dir);
  end;

end;
