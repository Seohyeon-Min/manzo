#include "../Engine/Component.h"
#include "GLTexture.h"

class GodRay : public CS230::Component {
public:
    GodRay();
    void Draw();
private:
    GLTexture* texture;
};