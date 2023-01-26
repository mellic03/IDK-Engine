#pragma once

#include <list>
#include <map>

#include <fstream>
#include "../gameobject/gameobject.h"
#include "../gameobject/gameobjectutil.h"
#include "../player/player.h"
#include "../../GraphicsEngine/GraphicsEngine.h"
#include "boundingvolume.h"

#define MAX_POINTLIGHTS 10
#define MAX_SPOTLIGHTS 2


class SceneGraph {

  private:

    BVTree _bvtree;

    std::map<std::string, std::list<GameObject *>>  _object_instances_by_template_name;

    std::list<GameObject *>  _object_templates_by_type[GAMEOBJECT_NUM_TYPES];

    std::list<GameObject *>  _object_instances_by_type[GAMEOBJECT_NUM_TYPES];
    std::list<GameObject *>  _object_instances_by_type_animated[GAMEOBJECT_NUM_TYPES];
    std::list<GameObject *>  _object_instances_by_type_instanced[GAMEOBJECT_NUM_TYPES];

    std::list<GameObject *>  _visible_instances_by_type[GAMEOBJECT_NUM_TYPES];
    std::map<std::string, InstanceData> _instance_data;

    std::list<ModelLOD> _modelLODs;


  public:

    std::list<Model> m_models;

    std::list<GameObject>     m_object_templates;
    std::list<GameObject>     m_object_instances;
  
    std::list<GameObject *>   m_dirlight_instances; 
    std::list<GameObject *>   m_spotlight_instances;
    std::list<GameObject *>   m_pointlight_instances;

    std::list<GameObject *>   m_selectable_instances;
  
    GameObject *player_object;


    DirLight dirlight;
    PointLight pointlights[MAX_POINTLIGHTS];
    SpotLight spotlights[MAX_SPOTLIGHTS];
    

    std::vector<PointLight *> active_pointlights, shadowmapped_pointlights, volumetric_pointlights, shadowmapped_volumetric_pointlights;
    int _num_pointlights = 0,  num_active_pointlights = 0, num_shadow_pointlights = 0;
    int num_volumetric_pointlights = 0, num_shadowmapped_volumetric_pointlights = 0;

    std::vector<SpotLight *>  active_spotlights,  shadowmapped_spotlights,  volumetric_spotlights, shadowmapped_volumetric_spotlights;
    std::vector<SpotLight *> sorted_spotlights;
    


    int _num_spotlights  = 0,  _num_active_spotlights  = 0;


    SceneGraph();
    

    void addInstanceData(std::string template_name, Model *model, Transform *transform);
    std::map<std::string, InstanceData> *getInstanceData(void);
    void perFrameUpdate(void);

    Model *modelPtr(std::string model_name);
    GameObject *objectPtr(int object_id);
    GameObject *objectPtr(std::string object_name);
    GameObject *templatePtr(std::string object_name);
    GameObject *frontObjectPtr(void);
    GameObject *rearObjectPtr(void);


    // Object Instantiation
    //----------------------------------------------------------------------------
    void newObjectInstance_terrain(GameObject *objectptr);
    void newObjectInstance_empty(GameObject *objectptr);
    void newObjectInstance_static(GameObject *objectptr);
    void newObjectInstance_billboard(GameObject *objectptr);
    void newObjectInstance_actor(GameObject *objectptr);
    void newObjectInstance_lightsource(GameObject *objectptr);

    void newObjectInstance(std::string object_name, Transform *transform = new Transform());
    //----------------------------------------------------------------------------

    // File IO
    //----------------------------------------------------------------------------
    void directoryloadrecursething(std::filesystem::path filepath);
    void loadObject(std::string directory);
    void loadObjects(std::string index_filepath);

    void objectFromFile(std::ifstream &stream, std::string &line, Player *player);
    void exportScene(std::string filepath);
    void importScene(std::string filepath, Player *player);
    //----------------------------------------------------------------------------


    // Wee waa
    //----------------------------------------------------------------------------
    void cullObjects(Frustum *frustum);
    BVTree *getBVTree()  { return &this->_bvtree; };
    //----------------------------------------------------------------------------


    void clearScene(void);
    void sortLights(Frustum *frustum);

    std::list<GameObject>   *getTemplates();

    std::list<GameObject *> *getInstancesByTemplateName(std::string template_name);
    std::list<GameObject *> *getTemplatesByType(GameObjectType object_type);
    
    std::list<GameObject *> *getInstancesByType(GameObjectType object_type);
    std::list<GameObject *> *getInstancesByType(GameObjectType object_type, InstancingType instancing);

    std::list<GameObject *> *getVisibleInstancesByType(GameObjectType object_type);
    std::list<GameObject *> *getVisibleInstancesByType(GameObjectType object_type, InstancingType instancing);
};

