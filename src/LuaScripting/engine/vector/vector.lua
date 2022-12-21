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

      __add = function (v1, v2)
        return this.newVector(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
      end;

      __sub = function (v1, v2)
        return this.newVector(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
      end;

      __div = function(v1, n)
        return this.newVector(v1.x/n, v1.y/n, v1.z/n);
      end;

      __mul = function(v1, n)
        return this.newVector(v1.x*n, v1.y*n, v1.z*n);
      end;

    });

    vector.x = vector[1];
    vector.y = vector[2];
    vector.z = vector[3];

  end;


  this.newVector = function(x, y, z)
    local lx, ly, lz;
    if z == nil then
      lx = 0; ly = 0; lz = 0;
    else
      lx = x; ly = y; lz = z;
    end;
    
    local vec = {lx, ly, lz};
    this.initVector(vec);
    return vec;
  end;


  this.initVectorArray = function (vector_array, size)
    for i = 1, size, 1 do
      this.initVector(vector_array[i]);
    end;
  end;


  this.add = function(v1, v2)
    v1.x = v1.x + v2.x;
    v1.y = v1.y + v2.y;
    v1.z = v1.z + v2.z;
  end;



  this.mag = function(v1)
    return math.sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
  end;


  this.dist = function(v1, v2)
    return this.mag(v1 - v2);
  end;


  this.normalise = function(v1)
    local m = this.mag(v1);
    return this.newVector(v1.x/m, v1.y/m, v1.z/m);
  end;


return this;