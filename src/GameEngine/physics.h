
#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"





class ModelContainer {

  private:
    Model *head = NULL;

  public:
    ModelContainer(void) {};

    void add(Model *model);
    void draw();
    void collide(Player *player);
};

