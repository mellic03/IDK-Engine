local engine = require("LuaScripting/engine");
local vlib = engine.vector;


return function(worldData, objectID)
	
	local pos = worldData.positions[objectID];
	pos.x = pos.x - 1 * worldData.deltaTime;

end;

