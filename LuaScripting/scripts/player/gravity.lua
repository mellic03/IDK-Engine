local engine = require("LuaScripting/engine");
local vlib = engine.vector;


return function(worldData, objectID)

	local pos = worldData.positions[objectID];
	local vel  = worldData.velocities[objectID];
	
	vel.y = vel.y - 1 * worldData.deltaTime;

end;

