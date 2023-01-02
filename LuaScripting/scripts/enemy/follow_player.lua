
return function(objectID, engine)
  local vlib = engine.vectorLibrary;

  local player_pos = GetPos(6);

  local pos = GetPos(objectID);
  local vel = GetVel(objectID);

  local dist = vlib.dist(pos, player_pos);
  local dir = vlib.newVector(0, 0, 0);

  if dist > 0.2 then
    dir = vlib.normalise(vlib.sub(player_pos, pos)) / 250.0;
  end;

  if dist < 2.0 then
    -- dir = vlib.normalise(pos - player_pos);
  end;

  SetPos(objectID, pos.x+dir.x, pos.y+dir.y, pos.z+dir.z);

end;
