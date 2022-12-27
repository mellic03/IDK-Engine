local engine = require("LuaScripting/engine");
local vlib = engine.vectorLibrary;


return function(worldData, objectID)

  local player_pos = worldData.positions[1];

  local pos = worldData.positions[objectID];
  local vel = worldData.velocities[objectID];

  local dist = vlib.dist(pos, player_pos);

  if dist > 2.5 then
    local dir = vlib.normalise(player_pos - pos) / 50.0;
    vlib.add(pos, dir);
  end;  

  if dist < 2.0 then
    local dir = vlib.normalise(pos - player_pos) / 25.0;
    vlib.add(pos, dir);
  end;

end;
