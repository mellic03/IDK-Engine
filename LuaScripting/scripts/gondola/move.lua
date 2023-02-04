
return function(objectID, engine)

  local pos = CE_GameObject_GetPos(objectID);
  CE_GameObject_SetPos(objectID, pos.x, pos.y, pos.z + 0.01);

end;

