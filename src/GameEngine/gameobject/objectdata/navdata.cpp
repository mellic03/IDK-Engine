#include "navdata.h"


using namespace Navigation;


inline NavState *NavData::state()
{
  return &this->_state;
}

void NavData::setPath(glm::vec3 target)
{
  this->_target = target;
  

  /*

    Determine if target can be reached


    if (reachable)
    {
      this->_state = NavState::SEEK;
    }

    else
    {
      this->_state = NavState::REST;
    }

  */


}



void NavData::followPath(glm::vec3 *objectPos)
{
  switch (this->_state)
  {
    case (Navigation::NavState::REST):

      break;


    case (Navigation::NavState::SEEK):

      if (this->_path.size() == 0)
      {
        this->_state = NavState::REST;
        break;
      }

      if (glm::distance(*objectPos, this->_path[this->_path.size()-1] + glm::vec3(0.0f, 0.5f, 0.0f)) < 0.5f)
      {
        this->_path.pop_back();
      }

      else
      {
        glm::vec3 move_towards_dir = 0.01f * glm::normalize(this->_path[this->_path.size()-1] - *objectPos);
        *objectPos += move_towards_dir;
      }
  
      break;
  }
}
