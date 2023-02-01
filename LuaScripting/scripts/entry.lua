
return function(objectID, engine)

  print("entry.lua executed");

  CE_Player_SetJumpForce(0.2);
  CE_Player_SetMoveForce(16.0);
  CE_Player_SetFly(true);

  CE_Physics_SetGravity(0.0);

  CE_Lighting_SetBloomMinResolution(64);
  CE_Lighting_SetBloomThreshold(2.0);
  CE_Lighting_SetBloomStrength(0.05);

  CE_Scene_ClearColor(55.0, 120.0, 120.0);
  CE_Scene_LoadScene("area1/a1m1.scene");

end;
