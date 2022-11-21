#include "../GraphicsEngine/GraphicsEngine.h"


enum WeaponType { SHOTGUN };


class Weapon {

  private:
    Model model;


  public:
    bool aiming = false;

    glm::vec3 hip_pos         = {0.0f, 0.0f, 0.0f};
    glm::vec3 aim_pos         = {0.0f, 0.0f, 0.0f};
    glm::vec3 movement_offset = {0.0f, 0.0f, 0.0f};

    float angle = 0.0f;
    float sway = 55.0f;

    Weapon() { };
    void load_model(const char *filepath, const char *name, Camera *cam);
    void draw(Camera *cam);

};