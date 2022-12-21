local lpath = "src/LuaScripting/"

local this = { };

  this.vector = require(lpath .. "engine/vector");


  this.tableLength = function (table)
    local count = 0;
    for _ in pairs(table) do count = count + 1 end;
    return count;
  end;


  this.worldDataInit = function (worldData)

    this.vector.initVectorArray(worldData[1], this.tableLength(worldData[1]));
    this.vector.initVectorArray(worldData[2], this.tableLength(worldData[2]));
    
    setmetatable(worldData, {

      __index = function (worldData, key)
        if      key == "positions"   then  return worldData[1];
        elseif  key == "velocities"  then  return worldData[2];
        end;
      end;

      __tostring = function (worldData)
        print("POSITIONS");
        for i, vec in pairs(worldData.positions) do
          print(i..": ", vec.x, vec.y, vec.z);
        end;
        print("");
      
        print("VELOCITIES");
        for i, vec in pairs(worldData.velocities) do
          print(i..": ", vec.x, vec.y, vec.z);
        end;
        print("");
      end;

    });

  end;


return this;