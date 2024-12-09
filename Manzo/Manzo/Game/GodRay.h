#include "../Engine/Component.h"
#include "GLTexture.h"

class GodRay : public Component {
public:
    GodRay();
    void Draw();
    void SetUniforms(const GLShader* shader);
private:
    GLTexture* texture;
    mat3 mat;
};

//class Pump : public Component {
//public:
//    Pump();
//    void Draw();
//    void Update(double dt) override;
//    void SetUniforms(const GLShader* shader);
//private:
//    GLTexture* texture;
//    float alpha = 1.0;
//};