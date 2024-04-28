#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/plane.h"
#include "glm/glm.hpp"
#include "helper/teapot.h"
#include "helper/objmesh.h"
#include "helper/cube.h"


class SceneBasic_Uniform : public Scene
{
private:
    //Torus torus;
    Plane plane;
    //Teapot teapot;
    std::unique_ptr<ObjMesh> mesh;
    //Cube cube;
    GLuint fsQuad, renderTex,intermediateTex, renderFBO, intermediateFBO;

    float tPrev;
    float angle;
    float rotSpeed;
    GLSLProgram prog;
    void setMatrices();
    void compile();

    void setupFBO();
    void pass1();
    void pass2();
    void pass3();
    float gauss(float, float);

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
