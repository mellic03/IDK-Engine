local this = { };


  this.initVector = function(vector)

    setmetatable(vector, {
      
      __index = function (vector, key)

        if key == 'x' then
          return vector[1];

        elseif key == 'y' then
          return vector[2];

        elseif key == 'z' then
          return vector[3];

        end;
        
      end ;
    });

    vector.x = vector[1];
    vector.y = vector[2];
    vector.z = vector[3];

  end;


  this.initVectorArray = function (vector_array, size)
    for i = 1, size, 1 do
      this.initVector(vector_array[i]);
    end;
  end;


  this.getPos = function (worldData)
      return worldData[1];
  end;


  this.vec3_sub = function(v1, v2)
      return { v1[1]-v2[1], v1[2]-v2[2], v1[3]-v1[3] };
  end;


  this.vec3_dot = function(v1, v2)
      return v1[1]*v2[1] + v1[2]*v2[2];
  end;



return this;