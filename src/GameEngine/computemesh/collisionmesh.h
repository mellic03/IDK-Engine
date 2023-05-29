#pragma once

#include <string>
#include <vector>

#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../include/glm/glm.hpp"


class CollisionMesh
{
    public:

        glm::vec3 bounding_sphere_pos = glm::vec3(0.0f);
        float bounding_sphere_radiusSQ = 0.0f;
    
        std::vector<Vertex> m_vertices;
        void computeBoundingSphere(void);
        /** Load navmesh from an .obj file */
        void load(std::string filepath);

};