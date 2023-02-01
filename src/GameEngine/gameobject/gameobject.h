#pragma once

#include "../state.h"
#include "../../GraphicsEngine/GraphicsEngine.h"

#include "../computemesh/computemesh.h"
#include "../physics.h"

#include "objectdata/objectdata.h"

#include "entitycomponent.h"
class EntityComponent;




struct GameObjectHeader {
  std::string template_name = "default";
  std::string assigned_name = "default";
  int objectID = 0;
  int parentID = -1;
  GLuint flags = 0;
};


struct CullingData {

  private:
    glm::vec3 _local_bounding_sphere_pos = glm::vec3(0.0f);

  public:

    void setLocalBoundingSpherePos(glm::vec3 pos)   { this->_local_bounding_sphere_pos = pos; };
    glm::vec3 getLocalBoundingSpherePos()           { return this->_local_bounding_sphere_pos; };

    glm::vec3 bounding_sphere_pos = glm::vec3(0.0f);
    float bounding_sphere_radius = 0.0f;
    float bounding_sphere_radiusSQ = 0.0f;
};


struct AnimationData {

  bool blend = false;
  float alpha = 0.0f;
  Animation::Animation *blend1 = nullptr;
  Animation::Animation *blend2 = nullptr;

};




struct LODData {

  int  level_of_detail = 0;
  bool override_global_lod = false;

};



class GameObject {

  private:
    GameObjectData _data;

    std::vector<GameObject *> m_children;
    Transform _transform;

    CullingData _culling_data;

    LODData _lod_data;
    ModelLOD *_modelLOD = nullptr;

    EntityComponents _entity_components;

    AnimationData _animation_data;
    Animation::Animation _animation;
    Animation::AnimationController _animation_controller;

    // Physics
    //----------------------------------------------------
    std::vector<CollisionMesh *> _collision_meshes;
    std::vector<Transform *> _collision_transforms;
    bool _has_collisionmesh = false;
    //----------------------------------------------------

    // Interactivity
    //----------------------------------------------------
    std::vector<glm::vec3> m_path;

    bool _is_static = false;
    bool _is_environmental = false;
    bool _is_animated = false;
    bool _is_npc = false;
    bool _hidden = false;
    //----------------------------------------------------

    bool _groundTest(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal);
    

    void _followPath();
    void _perFrameUpdate_navigation();

    void _perFrameUpdate_physics(Renderer *ren);



  public:

    bool in_frustum = true;

    GameObjectHeader header;


    glm::vec3 emission = glm::vec3(0.0f);
    float emission_scale = 1.0f;

    int parentID = -1;
    GameObject *m_parent = nullptr;


    PhysicsEngine::SphereCollider spherecollider;
    PhysicsEngine::CapsuleCollider capsulecollider;

    CollisionMesh m_collision_mesh;

    std::string m_template_name = "DEFAULT";
    std::string m_given_name = "DEFAULT";

    glm::vec3 pos_worldspace = glm::vec3(0.0f);


    GameObject(void);


    Transform *getTransform(void)     { return &this->_transform; };
    glm::vec3 *getPos(void)           { return this->_transform.getPos(); };
    glm::vec3 *getVel(void)           { return this->_transform.getVel(); };


    // Getters
    //---------------------------------------------------------------------------------------------
    GameObjectData *getData()                                 { return &this->_data; };

    int   getID(void)                                         { return this->header.objectID;  };
    int  *getIDptr(void)                                      { return &this->header.objectID; };

    std::string getName(void)                                 { return this->m_given_name; };
    std::string getTemplateName(void)                         { return this->m_template_name; };

    GameObjectType getObjectType(void)                        { return this->_data.gameobject_type; };
    std::string    getObjectTypeString(void);

    CullingData *getCullingData();

    LODData *getLODData();
    int *getLOD_value();
    int getLevelsLOD();

    ModelLOD *getModelLOD();

    Model *getModel(void);
    Model *getModel(int lod);

    EntityComponents *getComponents();
    
    
    AnimationData *getAnimationData();
    Animation::AnimationController *getAnimationController();
    Animation::Animation *getAnimation();
    Animation::Animation *getAnimation(std::string animation_name);
    //---------------------------------------------------------------------------------------------


    // Setters
    //---------------------------------------------------------------------------------------------
    void setID(int id);
  
    void setName(std::string name);
    void setTemplateName(std::string name);
  
    void setObjectType(GameObjectType type);

    void setModelLOD(ModelLOD *modelLOD);
    //---------------------------------------------------------------------------------------------




    // Interactivity
    //---------------------------------------------------------------------------------------------
    GameObject *getParent(void)   { return this->m_parent; };
    //---------------------------------------------------------------------------------------------

    bool hasCollisionMesh(void)                     { return this->_has_collisionmesh; };
    void hasCollisionMesh(bool has_collisionmesh)   { this->_has_collisionmesh = has_collisionmesh; };


    bool hasParent(void)                                { return this->m_parent != nullptr; };
    bool hasChildren(void)                              { return this->m_children.size() > 0; };
    std::vector<GameObject *> getChildren(void)         { return this->m_children; };
    void setParent(GameObject *parent);
    void clearParent(void);
    void giveChild(GameObject *child);
    void removeChild(GameObject *child);
    void clearChildren(void);
    bool isChild(GameObject *object);


    // Per-frame operations
    //---------------------------------------------------------------------------------------------
    CollisionMesh *getCollisionMesh(void)  { return &this->m_collision_mesh; };
    void collideWithObject(GameObject *object);
    void collideWithMeshes(void);

    void perFrameUpdate(Renderer *ren);
    //---------------------------------------------------------------------------------------------



};


