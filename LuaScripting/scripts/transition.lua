
return function(objectID, engine)

	local vlib = engine.vectorLibrary;

	local pos = GetPos(objectID);
	local player_pos = GetPos(6);
	
	local activation_dist = 0.2;
	local dist = vlib.dist(player_pos, pos);

	if (dist < activation_dist) then
		LoadScene("scene2.scene");
    return 1;
	end;
	
end;
