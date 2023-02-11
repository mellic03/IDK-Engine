#include "event.h"


void Event::setCondition(std::function<bool (void *)> condition_callback, void *condition_param)
{
    this->_condition_callback = condition_callback;
    this->_condition_param = condition_param;
}

void Event::onCondition(std::function<void(void *)> callback, void *callback_param)
{
    this->_onCondition_callback = callback;
    this->_onCondition_param = callback_param;
}

void Event::perFrameUpdate()
{
    this->_onCondition_callback(this->_onCondition_param);
}