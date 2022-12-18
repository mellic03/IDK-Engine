#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include "daeloader.h"
#include "mesh.h"
#include "animation.h"

// #include "../GameEngine/player.h"
// class Player;

class Model {

  private:
    std::vector<ColladaEffect> _collada_effects;

  public:

    std::vector<Mesh> m_meshes;

    void loadDae(std::string filepath);


};

