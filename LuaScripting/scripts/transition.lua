
return function(objectID, engine)

	local vlib = engine.vectorLibrary;

	local pos = getPos(objectID);
	local player_pos = getPos(6);
	
	local activation_dist = 1.0;
	local dist = vlib.dist(player_pos, pos);
	if (dist < activation_dist) then
		print("TRANSITION");
	else
		print("NO TRANSITION");
	end;
	
end;
