return function(worldData, objectID, engine)

  local vlib = engine.vectorLibrary;

	local pos = worldData.positions[objectID];
	local vel  = worldData.velocities[objectID];
	
	vel.y = vel.y - 2 * worldData.deltaTime;

end;

