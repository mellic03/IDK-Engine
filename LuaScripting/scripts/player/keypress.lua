local engine = require("LuaScripting/engine");
local vlib = engine.vectorLibrary;
local iolib = engine.ioLibrary;


return function(worldData, objectID)

	local pos = worldData.positions[objectID];

	if (iolib.keyPressed.KB_LSHIFT) then
		print("Running");
	end

end;

