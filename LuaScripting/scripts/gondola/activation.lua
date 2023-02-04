
return function(objectID, engine)

  local parentID = CE_GameObject_GetParentID(objectID);
  local playerID = 1;
  local playerPos = CE_GameObject_GetPos_worldspace(playerID);

  if (CE_GameObject_InBoundingBox(objectID, playerPos.x, playerPos.y, playerPos.z)) then
    CE_GameObject_SetParent(playerID, parentID);
  else
    CE_GameObject_ClearParent(playerID);
  end;


  if (engine.ioLibrary.keyPressed.R) then
    local pos = CE_GameObject_GetPos(parentID);
    CE_GameObject_SetPos(parentID, pos.x, pos.y, pos.z - 0.04);
  end
  if (engine.ioLibrary.keyPressed.F) then
    local pos = CE_GameObject_GetPos(parentID);
    CE_GameObject_SetPos(parentID, pos.x, pos.y, pos.z + 0.04);
  end


end;

