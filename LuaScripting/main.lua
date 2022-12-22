local engine = require("LuaScripting/engine");



-- External variables provided by engine
local worldData = {
  deltaTime,
  Positions,
  Velocities
};
engine.worldDataInit(worldData);
----------------------------------------



for i = 1, engine.tableLength(Scripts), 1 do

  local modulepath = Scripts[i];
  local filepath = modulepath .. ".lua";

  local fh = io.open(filepath, "r");
  
  if (fh ~= nil)
    then
      require(modulepath)(worldData, IDs[i]);
      io.close(fh);
    
    else
      print("Error opening file: ", filepath);
  end;
    
end;


