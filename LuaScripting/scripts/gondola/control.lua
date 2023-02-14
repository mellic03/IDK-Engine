
return function(objectID, engine)

  local parentID = CE_GameObject_GetParentID(objectID);
  local playerID = 1;
  local playerPos = CE_GameObject_GetPos_worldspace(playerID);

  if (CE_GameObject_InBoundingBox(objectID, playerPos.x, playerPos.y, playerPos.z) == false) then
    return;
  end;

  if (engine.ioLibrary.keyPressed.I) then
    local front = CE_GameObject_GetFront(parentID);
    CE_GameObject_AddVel(parentID, -front.x, -front.y, -front.z);
  end;
  if (engine.ioLibrary.keyPressed.K) then
    local front = CE_GameObject_GetFront(parentID);
    CE_GameObject_AddVel(parentID, front.x, front.y, front.z);
  end;

  if (engine.ioLibrary.keyPressed.J) then
    CE_GameObject_AddRot(parentID, 0.0, 0.01, 0.0);
  end;
  if (engine.ioLibrary.keyPressed.L) then
    CE_GameObject_AddRot(parentID, 0.0, -0.01, 0.0);
  end;

end;

