return function(objectID, engine)

  local iolib = engine.ioLibrary;


  local oPos = CE_GameObject_GetPos(objectID);
  local pPos = CE_GameObject_GetPos(1);
  local dist = engine.vectorLibrary.distance(oPos, pPos);
  local cutoff_dist = 6.0;


  local pDir = CE_Player_GetDir();
  pDir = engine.vectorLibrary.normalize(pDir);

  local oDir = engine.vectorLibrary.sub(oPos, pPos);
  oDir = engine.vectorLibrary.normalize(oDir);
  local lookingAt = engine.vectorLibrary.dot(pDir, oDir) > 0.9;


  pDir.x = pDir.x * 5.0;
  pDir.y = pDir.y * 5.0;
  pDir.z = pDir.z * 5.0;
  pDir = engine.vectorLibrary.add(pDir, pPos);


  local closeEnough = true;
  local ePressed = iolib.keyPressed.E;


  if ( ePressed ) then
    CE_GameObject_SetPath(objectID, pPos.x, pPos.y, pPos.z);
    CE_GameObject_SetAnimation(objectID, "walk");

  elseif ( CE_GameObject_GetNavState(objectID) ~= "SEEK" ) then
    CE_GameObject_SetAnimation(objectID, "stand");

  end;


end;
