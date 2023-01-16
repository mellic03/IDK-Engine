return function(objectID, engine)

  local vlib = engine.vectorLibrary;
  local origin = vlib.newVector();

  local pos = getPos(objectID);

  local dist = vlib.dist(origin, pos);

  if dist < 1.0 then
    local dir = vlib.newVector(pos.x - origin.x, pos.y - origin.y, pos.z - origin.z);
    dir = vlib.normalise(dir);
    local overlap = 1.0 - dist;
    dir = dir * overlap;

    setPos(objectID, pos.x+dir.x, pos.y+dir.y, pos.z+dir.z);
  end


end;




