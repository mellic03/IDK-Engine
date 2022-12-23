#pragma once

#include <string>
#include <vector>

#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../include/glm/glm.hpp"


class CollisionMesh {

  public:

    std::vector<Vertex> m_vertices;


    /** Load a navmesh from an .obj file */
    void load(std::string filepath);

};