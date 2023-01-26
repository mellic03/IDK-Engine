#include <algorithm>
#include "scenegraph.h"



SceneGraph::SceneGraph()
{
  this->clearScene();
}



Model *SceneGraph::modelPtr(std::string model_name)
{
  for (auto &model: this->m_models)
    if (model.m_name == model_name)
      return &model;

  return nullptr;
}


GameObject *SceneGraph::objectPtr(int object_id)
{
  for (auto &object: this->m_object_instances)
    if (object.getID() == object_id)
      return &object;

  return nullptr;
}


GameObject *SceneGraph::objectPtr(std::string object_name)
{
  for (auto &object: this->m_object_instances)
    if (object.getName() == object_name)
      return &object;

  return nullptr;
}


GameObject *SceneGraph::templatePtr(std::string object_name)
{
  for (auto &object: this->m_object_templates)
    if (object.getTemplateName() == object_name)
      return &object;

  return nullptr;
}


void SceneGraph::directoryloadrecursething(std::filesystem::path filepath)
{
  namespace fs = std::filesystem;

  for (auto const& dir_entry: fs::directory_iterator{filepath}) 
  {
    if (dir_entry.path().has_extension())
    {
      if (dir_entry.path().filename() == "asset.txt")
      {
        std::string relative_path = fs::relative(dir_entry.path(), fs::current_path()).parent_path().string();
        this->loadObject(relative_path + "/");
      }

      continue;
    }

    this->directoryloadrecursething(dir_entry.path());
  }

}


void SceneGraph::loadObjects(std::string index_filepath)
{
  std::ifstream stream;
  stream.open(index_filepath);
  
  std::string line;
  while(getline(stream, line))
  {
    if (line.find("#folder ") != std::string::npos)
    {
      line.erase(0, std::string("#folder ").size());

      namespace fs = std::filesystem;
      fs::path pth = fs::current_path() / "assets" / line;

      for (auto const& dir_entry : fs::directory_iterator{pth}) 
      {
        this->directoryloadrecursething(dir_entry.path());
      }
    }
  }
}


void SceneGraph::loadObject(std::string directory)
{
  std::string filepath = directory + "asset.txt";

  GameObject object;
  GameObjectData *objectData = object.getData();
  Model new_model;
  ModelLOD new_modelLod;

  FILE *fh = fopen(filepath.c_str(), "r");

  char buffer[256];
  char stringdata[256];
  int intdata;



  while (fgets(buffer, 256, fh) != NULL)
  {
    if (sscanf(buffer, "#gameobject %s", stringdata))
      object.setTemplateName(std::string(stringdata));


    else if (sscanf(buffer, "#GameObjectType %s", stringdata))
      object.setObjectType(GameObjectUtil::objectType_fromString(std::string(stringdata)));


    else if (sscanf(buffer, "#LightSourceType %s", stringdata))
      objectData->setLightSourceType(GameObjectUtil::lightsourceType_fromString(std::string(stringdata)));


    else if (sscanf(buffer, "#InstancingType %s", stringdata))
      objectData->setInstancingType(GameObjectUtil::instancingType_fromString(std::string(stringdata)));


    else if (sscanf(buffer, "#BilloardType %s", stringdata))
      objectData->setBillboardType(GameObjectUtil::billboardType_fromString(std::string(stringdata)));


    else if (sscanf(buffer, "#physics %s", stringdata))
      object.changePhysState(std::string(stringdata));


    else if (sscanf(buffer, "#meshlod %d %s", &intdata, stringdata))
    {
      new_modelLod.loadLOD(intdata, directory, std::string(stringdata), object.getObjectType() == GAMEOBJECT_TERRAIN);
      this->_modelLODs.push_back(new_modelLod);
      object.setModelLOD(&*std::prev(this->_modelLODs.end()));
    }


    else if (sscanf(buffer, "#collision %s", stringdata))
    {
      object.m_collision_mesh.load(directory + std::string(stringdata));
      object.hasCollisionMesh(true);
    }


    else if (strstr(buffer, "#exit"))
      break;
  }

  if (object.getModelLOD() != nullptr)
  {
    Model *model = object.getModelLOD()->getDefaultLOD_model();
    
    object.getData()->isAnimated(model->isAnimated());

    object.getCullingData()->setLocalBoundingSpherePos(model->bounding_sphere_pos);
    object.getCullingData()->bounding_sphere_radius = model->bounding_sphere_radius;
    object.getCullingData()->bounding_sphere_radiusSQ = model->bounding_sphere_radiusSQ;
  }

  this->m_object_templates.push_back(object);

  GameObject *objectptr = this->templatePtr(object.getTemplateName());
  this->_object_templates_by_type[objectptr->getObjectType()].push_back(objectptr);

  fclose(fh);
}


GameObject *SceneGraph::rearObjectPtr(void)
{
  std::list<GameObject>::iterator ptr = this->m_object_instances.end();
  ptr = std::prev(ptr);

  GameObject *objptr = &*ptr;

  return objptr;
}


GameObject *SceneGraph::frontObjectPtr(void)
{
  std::list<GameObject>::iterator ptr = this->m_object_instances.begin();

  GameObject *objptr = &*ptr;

  return objptr;
}


void SceneGraph::clearScene(void)
{
  this->m_object_instances.clear();
  this->m_selectable_instances.clear();

  for (int i=0; i<GAMEOBJECT_NUM_TYPES; i++)
    this->_object_instances_by_type[i].clear();
    
  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    this->pointlights[i].active = false;
    this->pointlights[i].shadowmapped = false;
    this->pointlights[i].m_transform = &this->pointlights[i].default_transform;
    this->pointlights[i].diffuse = glm::vec3(0.0f);
  }

  this->_num_pointlights = 0;
  this->_num_spotlights = 0;
  this->num_active_pointlights = 0;
  this->num_shadow_pointlights = 0;
  this->_num_active_spotlights = 0;
}


void SceneGraph::sortLights(Frustum *frustum)
{
  this->num_active_pointlights = 0;
  this->num_volumetric_pointlights = 0;
  this->num_shadowmapped_volumetric_pointlights = 0;
  this->num_shadow_pointlights = 0;

  this->active_pointlights.clear();
  this->shadowmapped_pointlights.clear();
  this->volumetric_pointlights.clear();
  this->shadowmapped_volumetric_pointlights.clear();


  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    glm::vec3 p = this->pointlights[i].m_transform->getPos_worldspace();
    float radius = this->pointlights[i].radius;

    if (frustum->partiallyVisible(p, radius) == false)
      continue;

    if (this->pointlights[i].active)
    {
      if (!this->pointlights[i].shadowmapped)
        this->active_pointlights.push_back(&this->pointlights[i]);
      else
        this->shadowmapped_pointlights.push_back(&this->pointlights[i]);
      
      if (this->pointlights[i].volumetrics_active)
      {
        if (!this->pointlights[i].shadowmapped)
          this->volumetric_pointlights.push_back(&this->pointlights[i]);
        else
          this->shadowmapped_volumetric_pointlights.push_back(&this->pointlights[i]);
      }
    }
  }


  this->num_active_pointlights = this->active_pointlights.size();
  this->num_shadow_pointlights = this->shadowmapped_pointlights.size();
  this->num_volumetric_pointlights = this->volumetric_pointlights.size();
  this->num_shadowmapped_volumetric_pointlights = this->shadowmapped_volumetric_pointlights.size();


  for (size_t i=this->active_pointlights.size(); i<MAX_POINTLIGHTS; i++)
    this->active_pointlights.push_back(&this->pointlights[0]);

  for (size_t i=this->shadowmapped_pointlights.size(); i<MAX_POINTLIGHTS; i++)
    this->shadowmapped_pointlights.push_back(&this->pointlights[0]);

  for (size_t i=this->volumetric_pointlights.size(); i<MAX_POINTLIGHTS; i++)
    this->volumetric_pointlights.push_back(&this->pointlights[0]);

  for (size_t i=this->shadowmapped_volumetric_pointlights.size(); i<MAX_POINTLIGHTS; i++)
    this->shadowmapped_volumetric_pointlights.push_back(&this->pointlights[0]);
}


std::list<GameObject> *SceneGraph::getTemplates()
{
  return &this->m_object_templates;
}


std::list<GameObject *> *SceneGraph::getInstancesByTemplateName(std::string template_name)
{
  return &this->_object_instances_by_template_name[template_name];
}


std::list<GameObject *> *SceneGraph::getTemplatesByType(GameObjectType object_type)
{
  return &this->_object_templates_by_type[object_type];
}


std::list<GameObject *> *SceneGraph::getInstancesByType(GameObjectType object_type)
{
  return &this->_object_instances_by_type[object_type];
}
std::list<GameObject *> *SceneGraph::getInstancesByType(GameObjectType object_type, InstancingType instancing)
{
  if (instancing == INSTANCING_ON)
    return &this->_object_instances_by_type_instanced[object_type];
  else
    return &this->_object_instances_by_type[object_type];
}


std::list<GameObject *> *SceneGraph::getVisibleInstancesByType(GameObjectType object_type)
{
  return &this->_visible_instances_by_type[object_type];
}
std::list<GameObject *> *SceneGraph::getVisibleInstancesByType(GameObjectType object_type, InstancingType instancing)
{
  return &this->_visible_instances_by_type[object_type];
}

void SceneGraph::cullObjects(Frustum *frustum)
{
  this->_bvtree.clearTree();


  for (GameObject *object: *this->getInstancesByType(GAMEOBJECT_TERRAIN))
    this->_bvtree.insert(object);

  for (GameObject *object: *this->getInstancesByType(GAMEOBJECT_STATIC))
    this->_bvtree.insert(object);

  for (GameObject *object: *this->getInstancesByType(GAMEOBJECT_ACTOR))
    this->_bvtree.insert(object);

  for (GameObject *object: *this->getInstancesByType(GAMEOBJECT_BILLBOARD))
    this->_bvtree.insert(object);

  this->_bvtree.insert(this->player_object);

  // system("clear");
  // this->_bvtree.print();

  for (int i=0; i<(int)GAMEOBJECT_NUM_TYPES; i++)
    this->_visible_instances_by_type[i].clear();
  this->_bvtree.cullObjects(frustum, this->_visible_instances_by_type);

}