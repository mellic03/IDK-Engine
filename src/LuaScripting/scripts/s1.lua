local engine = require("src/LuaScripting/engine");
local vlib = engine.vector;


return {

  main = function(worldData, objectID)

    local pos = worldData.positions[objectID];
    local vel = worldData.velocities[objectID];

    local player_pos = worldData.positions[1];

    local dist = vlib.dist(pos, player_pos);
    if dist < 2.0 then
      local dir = vlib.normalise(pos - player_pos) / 20.0;
      vlib.add(vel, dir);
    end;

  end;

};