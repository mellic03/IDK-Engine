#pragma once

#include <functional>
#include <any>

class Event {

    private:

        std::function<bool (void *)> _condition_callback;
        void *_condition_param = nullptr;

        std::function<void (void *)> _onCondition_callback;
        void *_onCondition_param = nullptr;


    public:

        Event() { };

        void setCondition(std::function<bool (void *)> condition_callback, void *condition_param);
        void onCondition(std::function<void ( void*)> callback, void *callback_param);
        void perFrameUpdate();

};





