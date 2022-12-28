local engine = require("LuaScripting/engine");

DeltaTime = 0;
IDs = {};
Scripts = {};


-- ----------------------------------------

function Main()

  for i = 1, engine.tableLength(Scripts), 1 do

    local modulepath = Scripts[i];
    local filepath = modulepath .. ".lua";

    local fh = io.open(filepath, "r");
    
    if (fh ~= nil)
      then
        require(modulepath)(IDs[i], engine);
        io.close(fh);
      
      else
        print("Error opening file: ", filepath);
    end;
      
  end;

end;




