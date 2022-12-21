local engine = require("src/LuaScripting/engine");

return {

  main = function (worldData, objectInstance)
    print("pos: ", worldData.positions[objectInstance].x, worldData.positions[objectInstance].y, worldData.positions[objectInstance].z);
  end;

};