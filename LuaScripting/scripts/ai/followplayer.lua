return function(objectID, engine)

  local iolib = engine.ioLibrary;

  local pPos = CE_GameObject_GetPos(1);

  if ( iolib.keyPressed.E ) then
    CE_GameObject_SetPath(objectID, pPos.x, pPos.y, pPos.z);
    CE_GameObject_SetAnimation(objectID, "walk");

  elseif ( CE_GameObject_GetNavState(objectID) ~= "SEEK" ) then
    CE_GameObject_SetAnimation(objectID, "idle");

  end;

end;
