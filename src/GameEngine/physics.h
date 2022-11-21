
#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"





class ModelContainer {

  public:
    Model *head = NULL;
    ModelContainer(void) {};

    void add(Model *model);
    void draw();
    void collide(Player *player);
};

