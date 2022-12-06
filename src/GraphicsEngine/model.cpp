#include "model.h"


bool Model::load(std::string filepath)
{
  std::string assetfilepath = filepath + "asset.txt";

  FILE *fh = fopen(assetfilepath.c_str(), "r");
  if (fh == NULL)
  {
    printf("Error opening %s\n", assetfilepath);
    return false;
  }

  char buffer[64];
  char stringdata[64];
  int intdata;

  while (fgets(buffer, 64, fh) != NULL)
  {
    if (sscanf(buffer, "#ANIMATION"))
    {
      this->staticmesh = false;
      break;
    }

    else if (sscanf(buffer, "#STATIC"))
    {
      this->staticmesh = true;
      break;
    }
  }

  rewind(fh);

  while (fgets(buffer, 64, fh) != NULL)
  {
    if (sscanf(buffer, "#ANIMATION REST %s %d", stringdata, &intdata))
    {
      printf("animation: %s, keyframes: %d\n", stringdata, intdata);
      this->animations[ANIM_REST].loadKeyframes(filepath, stringdata, intdata);
    }

    else if (sscanf(buffer, "#STATIC %s", stringdata))
    {
      printf("static: %s\n", stringdata);

      this->meshes.push_back(Mesh());
      this->meshes[0].load(filepath.c_str(), stringdata);
    }
    
    else if (sscanf(buffer, "#NAVMESH %s", stringdata))
    {
      printf("navmesh: %s\n", stringdata);

      // this->meshes.push_back(Mesh());
      // this->meshes[0].load(filepath.c_str(), stringdata);
    }
  }

  fclose(fh);

  return true;
}


void Model::bindRenderer(Renderer *ren)
{
  for (auto mesh: this->meshes)
  {
    mesh.bindRenderer(ren);
  }
}

void Model::activeAnimation(AnimationType id)
{
  this->m_active_animation = id;
}


void Model::draw(Renderer *ren)
{
 
  switch (this->staticmesh)
  {

    case (false):
      for (Mesh mesh: this->meshes)
        mesh.draw(ren);
      break;

    case (true):
      switch (this->m_state)
      {
        case (MSTATE_NOANIM_PLAYING):
          this->animations[ANIM_REST].play(ren);
          break;

        case (MSTATE_ANIM_PLAYING):
          this->animations[this->m_active_animation].play(ren);
          break;
      }
      break;
  }
}
