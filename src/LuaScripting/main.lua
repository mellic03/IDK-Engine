local lpath = "src/LuaScripting/";
local engine = require(lpath .. "/engine");


-- External variables provided by engine
local worldData = {
  Positions,
  Velocities
};
engine.worldDataInit(worldData);
----------------------------------------

for i, vec in pairs(worldData.positions) do
  vec.x = math.random();
  vec.y = math.random();
  vec.z = math.random();
end

Scripts = { "default" };

for i = 1, engine.tableLength(Scripts), 1 do

  local modulepath = lpath .. "scripts/" .. Scripts[i];
  local filepath = modulepath .. ".lua";

  local fh = io.open(filepath, "r");
  
  if (fh ~= nil)
    then
      require(modulepath).main(worldData);
      io.close(fh);
    
    else
      print("Error opening file: ", filepath);
  end;
    
end;


