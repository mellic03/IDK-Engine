#pragma once

#include "../state.h"
#include "../../transform.h"

#include "../computemesh/computemesh.h"
#include "../physics.h"

#include "entitycomponent.h"
class EntityComponent;


enum GameObjectType {
  GAMEOBJECT_UNDEFINED,
  GAMEOBJECT_TERRAIN,
  GAMEOBJECT_STATIC,
  GAMEOBJECT_BILLBOARD,
  GAMEOBJECT_ACTOR,
  GAMEOBJECT_PLAYER,
  GAMEOBJECT_LIGHTSOURCE,
  GAMEOBJECT_NUM_TYPES
};


class GameObject {

  private:

    GameObjectType gameobject_type = GAMEOBJECT_UNDEFINED;
    LightSourceType lightsource_type = LIGHTSOURCE_NONE;
  
    bool _has_geometry = false;

    std::vector<GameObject *> m_children;
    Transform _transform;

    // Physics
    //----------------------------------------------------
    PhysicsState m_physics_state = PHYSICS_NONE;
    NavigationState m_navigation_state = NAVIGATION_REST;
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



  public:

    glm::vec3 emission = glm::vec3(0.0f);
    float emission_scale = 1.0f;

    int m_ID = 0;
    int parentID = -1;
    GameObject *m_parent = nullptr;

    std::vector<EntityComponent> transform_components;
    std::vector<EntityComponent> lightsource_components;
    std::vector<EntityComponent> script_components;
    std::vector<EntityComponent> variable_components;
    std::vector<EntityComponent> terrain_components;

    PhysicsEngine::SphereCollider spherecollider;
    PhysicsEngine::CapsuleCollider capsulecollider;


    Model *m_model;
    Model *m_volumetric_light_model = nullptr;
    CollisionMesh m_collision_mesh;

    std::string m_template_name = "DEFAULT";
    std::string m_given_name = "DEFAULT";
    std::string m_interactivity = "DEFAULT";


    glm::vec3 pos_worldspace = glm::vec3(0.0f);

    bool selected = false;

    GameObject(void);


    Transform *getTransform(void)     { return &this->_transform; };
    glm::vec3 *getPos(void)           { return this->_transform.getPos(); };
    glm::vec3 *getVel(void)           { return this->_transform.getVel(); };


    // Object ID
    //---------------------------------------------------------------------------------------------
    int   getID(void)                                         { return this->m_ID; };
    int  *getIDptr(void)                                      { return &this->m_ID; };
    void  setID(int id)                                       { this->m_ID = id; };

    std::string getName(void) /*...........................*/ { return this->m_given_name; };
    void setName(std::string name) /*......................*/ { this->m_given_name = name; };

    std::string getTemplateName(void)                         { return this->m_template_name; };
    void setTemplateName(std::string name)                    { this->m_template_name = name; };
    //---------------------------------------------------------------------------------------------


    // Interactivity
    //---------------------------------------------------------------------------------------------
    void setObjectType(GameObjectType type)           { this->gameobject_type = type; };
    GameObjectType getObjectType(void)                { return this->gameobject_type; };
    std::string    getObjectTypeString(void);
    
    void setLightSourceType(LightSourceType type)     {  };

  
    void setInteractivity(std::string interactivity)  { this->m_interactivity = interactivity; };
    bool isNPC(void)              { return this->m_interactivity == "npc"; };
    bool isEnvironmental(void)    { return this->m_interactivity == "environmental"; };
    bool isMisc(void)             { return this->m_interactivity == "misc"; };
    bool isPlayer(void)           { return this->m_interactivity == "player"; };

    bool usePhysics(void)         { return this->m_interactivity == "npc" || this->m_interactivity == "player"; };

    bool isHidden(void)           { return this->_hidden; };
    bool *isHiddenPtr(void)       { return &this->_hidden; };

    GameObject *getParent(void)   { return this->m_parent; };
    //---------------------------------------------------------------------------------------------


    bool hasGeometry(void)                          { return this->_has_geometry; };
    void hasGeometry(bool has_geometry)             { this->_has_geometry = has_geometry; };

    bool hasCollisionMesh(void)                     { return this->_has_collisionmesh; };
    void hasCollisionMesh(bool has_collisionmesh)   { this->_has_collisionmesh = has_collisionmesh; };

    bool hasLightSource(void)                       { return this->lightsource_components.size() > 0; };


    bool hasParent(void)                                { return this->m_parent != nullptr; };
    bool hasChildren(void)                              { return this->m_children.size() > 0; };
    std::vector<GameObject *> getChildren(void)         { return this->m_children; };
    void setParent(GameObject *parent, bool keepGlobalPos = true);
    void clearParent(void);
    void giveChild(GameObject *child, bool keepGlobalPos = true);
    void removeChild(GameObject *child);
    void clearChildren(void);
    bool isChild(GameObject *object);


    // Per-frame operations
    //---------------------------------------------------------------------------------------------
    void changePhysState(PhysicsState state);
    void changePhysState(std::string state);
    void changeNavState(NavigationState state);

    PhysicsState getPhysState(void);
    NavigationState getNavState(void);

    std::string physStateString(void);
    std::string navStateString(void);
    std::string getStateString(StateType state_type);

    CollisionMesh *getCollisionMesh(void)  { return &this->m_collision_mesh; };
    void collideWithObject(GameObject *object);
    void collideWithMeshes(void);

    void perFrameUpdate(Renderer *ren);
    //---------------------------------------------------------------------------------------------



};


