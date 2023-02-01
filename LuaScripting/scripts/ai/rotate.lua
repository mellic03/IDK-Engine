return function(objectID, engine)

  local pos = CE_GameObject_GetPos(1);

  -- CE_GameObject_SetPath(objectID, pos.x, pos.y, pos.z);
  CE_GameObject_AddRot(objectID, 0.0, 0.25*DeltaTime, 0.0);

end;