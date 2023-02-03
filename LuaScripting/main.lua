local engine = require("LuaScripting/engine");

DeltaTime = 0;
IDs = {};
Scripts = {};

-- ----------------------------------------

function Main()

  engine.ioLibInit(KeyPressed);


  for i = 1, engine.tableLength(Scripts), 1 do

    local modulepath = Scripts[i];
    local filepath = modulepath .. ".lua";

    local fh = io.open(filepath, "r");

    if (fh ~= nil) then

      local returnValue = require(modulepath)(IDs[i], engine);

      if (returnValue == 1) then
        return;
      end;
      io.close(fh);

      else
        print("Error opening file: ", filepath);

    end;

  end;

end;




