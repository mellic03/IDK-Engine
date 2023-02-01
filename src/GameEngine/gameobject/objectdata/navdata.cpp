#include "navdata.h"


using namespace Navigation;


void NavData::setPath(glm::vec3 current_pos, glm::vec3 target, std::list<NavMesh> *navmeshes)
{
  this->_target = target;
  this->path = (*navmeshes->begin()).path(current_pos, target);
  this->state = NavState::SEEK;
}



void NavData::followPath(glm::vec3 *objectPos)
{

}
