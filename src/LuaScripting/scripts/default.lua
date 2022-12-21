local engine = require("src/LuaScripting/engine");

return {

  main = function (worldData, objectInstance)

    for i, vec in pairs(worldData.positions) do
      vec.x = math.random();
      vec.y = math.random();
      vec.z = math.random();
    end;

  end;

};