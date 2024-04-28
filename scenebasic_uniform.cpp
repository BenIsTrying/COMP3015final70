#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>

#include <iostream>
using std::cerr;
using std::endl;


#include "helper/glutils.h"
#include "helper/texture.h"
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;


SceneBasic_Uniform::SceneBasic_Uniform() : 
    tPrev(0), angle(0.0f),rotSpeed(glm::pi<float>()/8.0f), plane(50.0f, 50.0f, 1, 1){
    //, teapot(14, mat4(1.0f)), torus(0.7f * 1.5f, 0.3f * 1.5f, 50, 50)
    mesh = ObjMesh::load("media/newPenquin.obj");


}

void SceneBasic_Uniform::initScene()
{
    compile();
    glClearColor(0.0f, 0.2f,0.8f, 1.0f);//set colour of background
    glEnable(GL_DEPTH_TEST);
    //model = mat4(1.0f);

    //model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));//rotate on y axis

    projection = mat4(1.0f);
    angle = glm::pi<float>()/4.0f;
    
    prog.setUniform("Light.L", vec3(1.0f));
    prog.setUniform("Light.La", vec3(0.2f));

    
    
    //GLuint spotTexture = Texture::loadTexture("media/spot/spot_texture.png");
    //mesh = ObjMesh::load("media/spot/spot_triangulated.obj");
    setupFBO();
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, spotTexture);

    GLfloat verts[] = {
        -1.0f,-1.0f,0.0f,1.0f,-1.0,0.0,1.0f,1.0f,0.0f,
        -1.0f,-1.0f,0.0f,1.0f,1.0,0.0,-1.0f,1.0f,0.0f
    };
    GLfloat tc[] = {
        0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,
        0.0f,0.0f,1.0f,1.0f,0.0f,1.0f
    };

    unsigned int handle[2];
    glGenBuffers(2, handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    glGenVertexArrays(1, &fsQuad);
    glBindVertexArray(fsQuad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0,0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0,0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    prog.setUniform("EdgeThreshold", 0.05f);
    prog.setUniform("Light.L", vec3(1.0f));
    prog.setUniform("Light.La", vec3(0.2f));

    /*model = glm::translate(model, vec3(0.1f, -2.0f, 1.0f));

    float x, z;
    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 2.2f, z + 1.0f, 1.0f));
    }

    prog.setUniform("Light.Position", view * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));
    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 1.2f));
    prog.setUniform("lights[1].L", vec3(0.0f, 1.2f, 0.0f));
    prog.setUniform("lights[2].L", vec3(1.2f, 0.0f, 0.0f));

    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.4f));
    prog.setUniform("lights[1].La", vec3(0.0f, 0.4f, 0.0f));
    prog.setUniform("lights[2].La", vec3(0.4f, 0.0f, 0.0f));
    */


    // prog.setUniform("Light.La", vec3(0.4f, 0.4f, 0.4f));
     //prog.setUniform("Light.Ls", vec3(1.0f, 1.0f, 1.0f));


}

void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");

        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;
    angle += rotSpeed* deltaT;
    if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    pass1();
    glFlush();
    pass2();
}

void SceneBasic_Uniform::pass1() {


    prog.setUniform("Pass", 1);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(7.0f * cos(angle), 4.0f,7.0f * sin(angle)), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);


    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    prog.setUniform("Material.Kd", vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Ka", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));

    model = glm::translate(model, vec3(0.0f, -5.75f, 0.0f));
    
    setMatrices();
    mesh->render();
    

    
    prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.4f));
    prog.setUniform("Material.Ks", vec3(0.0f, 0.0f, 0.0f));
    prog.setUniform("Material.Ka", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    model = glm::translate(model,vec3(0.0f, -5.75f, 0.0f));
    setMatrices();
    plane.render();
    //std::cout << "plane should render";// i now know it is indeed rendering

    /*
    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    
    prog.setUniform("Material.Kd", vec3(0.9f, 0.5f, 0.2f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Ka", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(1.0f,1.0f, 3.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    torus.render();*/

    //mesh->render();
}

void SceneBasic_Uniform::pass2() {


    prog.setUniform("Pass", 2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    model = mat4(1.0f);
    view = mat4(1.0f);
    projection = mat4(1.0f);

    setMatrices();

    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;

    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices() {
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setupFBO() {

    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    //GLuint renderTex;
    glGenTextures(1, &renderTex);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    if (result == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is complete" << endl;
    }
    else {
        std::cout << "Framebuffer error:" << result << endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}