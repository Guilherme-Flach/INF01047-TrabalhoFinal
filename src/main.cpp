#include "engine/EngineObject/camera/camera.hpp"
#include "engine/Physics/physicsObject.hpp"
#include "engine/Rendering/defaultModels.hpp"
#include "engine/Rendering/model3D.hpp"
#include "engine/loader.hpp"
#include "engine/Input/keyMap.hpp"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float4.hpp"
#include "matrices.hpp"
#include <functional>
#include <iostream>

int main(int argc, char *argv[]) {
    int width = 800, height = 800;
    char title[] = "romano";

    auto loader = Loader(width, height, title);

    std::vector<GLfloat> vertices = std::vector<GLfloat>({
        -0.5f, 0.5f,  0.5f,  1.0f, // posição do vértice 0
        -0.5f, -0.5f, 0.5f,  1.0f, // posição do vértice 1
        0.5f,  -0.5f, 0.5f,  1.0f, // posição do vértice 2
        0.5f,  0.5f,  0.5f,  1.0f, // posição do vértice 3
        -0.5f, 0.5f,  -0.5f, 1.0f, // posição do vértice 4
        -0.5f, -0.5f, -0.5f, 1.0f, // posição do vértice 5
        0.5f,  -0.5f, -0.5f, 1.0f, // posição do vértice 6
        0.5f,  0.5f,  -0.5f, 1.0f, // posição do vértice 7
    });

    std::vector<GLfloat> colors = std::vector<GLfloat>({
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 1
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 2
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 3
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 4
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 5
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 6
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 7
    });

    std::vector<GLuint> indices = std::vector<GLuint>({
        0, 1, 2, // triângulo 1
        7, 6, 5, // triângulo 2
        3, 2, 6, // triângulo 3
        4, 0, 3, // triângulo 4
        4, 5, 1, // triângulo 5
        1, 5, 6, // triângulo 6
        0, 2, 3, // triângulo 7
        7, 5, 4, // triângulo 8
        3, 6, 7, // triângulo 9
        4, 3, 7, // triângulo 10
        4, 1, 0, // triângulo 11
        1, 6, 2, // triângulo 12
    });

    glm::vec4 position = {2.0, 2.0, 2.0, 1.0};
    glm::vec4 target = {0.0, 0.0, 0.0, 1.0};
    LookAtCamera cameraLookAt = LookAtCamera(position, target);
    FreeCamera cameraFree = FreeCamera({0.0f, 0.0f, 0.0f, 1.0f}, 0, 0);

    GameObject romano = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    romano.set_modelScaling({10,10,10});

    PhysicsObject physObj = PhysicsObject({1.0f, 1.0f, 1.0f, 1.0f}, 1.0f);

    Model3D cuboRender = Model3D(vertices, indices, colors, GL_TRIANGLES);
    Model3D wireCube = WireCube();

    PhysicsObject gamer = PhysicsObject({0.0f, 0.0f, 0.0f, 1.0f}, 1);
    gamer.set_model(wireCube);
    gamer.addChild(cameraFree);

    GameObject cubo1 = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    cubo1.set_model(cuboRender);
    cubo1.set_modelScaling({0.5f,1.0f,0.5f});

    GameObject cubo2 = GameObject({1.0f, 0.0f, 0.0f, 1.0f});
    cubo2.set_model(cuboRender);
    cubo2.set_modelScaling({0.2f,0.2f,0.2f});
    cubo1.addChild(cubo2);

    GameObject cubo3 = GameObject({0.0f, 0.5f, 0.0f, 1.0f});
    cubo3.set_model(cuboRender);
    cubo3.set_modelScaling({0.2f,0.2f,0.2f});
    cubo2.addChild(cubo3);

    physObj.addChild(cubo1);
    physObj.addChild(cameraLookAt);

    GameObject cubo4 = GameObject({0.0f, 0.0f, 0.0f, 1.0f});
    cubo4.set_model(cuboRender);
    cubo4.set_modelScaling({0.3f,0.3f,0.3f});
    
    loader.add_game_object(romano);
    loader.add_game_object(physObj);
    loader.add_game_object(cubo4);

    loader.add_camera(cameraLookAt);
    loader.add_camera(cameraFree);
    loader.set_active_camera(&cameraFree);

    // OH LAWD HE COMING
    // FIXME: Tem que extrair essas putaria pra dentro do player
    // tem um bug se fizer rotation e movement ao msm tempo
    // quando ele larga, ele n desfaz o bgl pq os vector mudaram
    // gaming
    static const GLfloat speed = 0.01f;
    KeyMap::addKeyMapping(GLFW_KEY_W, [&gamer, &cameraFree](Action action) {
        glm::vec4 direction = -cameraFree.get_w_vector();
        if (action == GLFW_PRESS) {
            gamer.increase_acceleration( direction * speed);
        } else if (action == GLFW_RELEASE) {
            gamer.increase_acceleration( -direction * speed);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_S, [&gamer, &cameraFree](Action action) {
        glm::vec4 direction = cameraFree.get_w_vector();
        if (action == GLFW_PRESS) {
            gamer.increase_acceleration( direction * speed);
        } else if (action == GLFW_RELEASE) {
            gamer.increase_acceleration( -direction * speed);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_A, [&gamer, &cameraFree](Action action) {
        glm::vec4 direction = -cameraFree.get_u_vector();
        if (action == GLFW_PRESS) {
            gamer.increase_acceleration( direction * speed);
        } else if (action == GLFW_RELEASE) {
            gamer.increase_acceleration( -direction * speed);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_D, [&gamer, &cameraFree](Action action) {
        glm::vec4 direction = cameraFree.get_u_vector();
        if (action == GLFW_PRESS) {
            gamer.increase_acceleration( direction * speed);
        } else if (action == GLFW_RELEASE) {
            gamer.increase_acceleration( -direction * speed);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_SPACE, [&gamer, &cameraFree](Action action) {
        glm::vec4 direction = cameraFree.get_v_vector();
        if (action == GLFW_PRESS) {
            gamer.increase_acceleration( direction * speed);
        } else if (action == GLFW_RELEASE) {
            gamer.increase_acceleration( -direction * speed);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_LEFT_CONTROL, [&gamer, &cameraFree](Action action) {
        glm::vec4 direction = -cameraFree.get_v_vector();
        if (action == GLFW_PRESS) {
            gamer.increase_acceleration( direction * speed);
        } else if (action == GLFW_RELEASE) {
            gamer.increase_acceleration( -direction * speed);
        }
    });


    // Rotation
    KeyMap::addKeyMapping(GLFW_KEY_UP, [&gamer, &cameraFree](Action action) {
        if (action == GLFW_PRESS) {
            cameraFree.set_phi(cameraFree.get_phi() - 0.05f);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_DOWN, [&gamer, &cameraFree](Action action) {
        if (action == GLFW_PRESS) {
            cameraFree.set_phi(cameraFree.get_phi() + 0.05f);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_LEFT, [&gamer, &cameraFree](Action action) {
        if (action == GLFW_PRESS) {
            cameraFree.set_theta(cameraFree.get_theta() + 0.05f);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_RIGHT, [&gamer, &cameraFree](Action action) {
        if (action == GLFW_PRESS) {
            cameraFree.set_theta(cameraFree.get_theta() - 0.05f);
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_Q, [&gamer, &cameraFree](Action action) {
        glm::vec4 axis = cameraFree.get_w_vector();
        if (action == GLFW_PRESS) {
            cameraFree.set_up_vector(Matrix_Rotate(0.05f,axis) * cameraFree.get_up_vector());
        }
    });

    KeyMap::addKeyMapping(GLFW_KEY_E, [&gamer, &cameraFree](Action action) {
        glm::vec4 axis = -cameraFree.get_w_vector();
        if (action == GLFW_PRESS) {
            cameraFree.set_up_vector(Matrix_Rotate(0.05f,axis) * cameraFree.get_up_vector());
        }
    });

    physObj.applyForce({0.5f, 0.00f, 0.5f, 0.0f});
    gamer.set_drag(0.6);

    loader.start([&physObj, &romano, &cameraLookAt, &cubo1, &cubo3, &gamer, &cameraFree]() {
        physObj.update(Loader::get_delta_t());
        gamer.update(Loader::get_delta_t());
        cubo1.rotate({0.0f, 0.002f, 0.00f});
        cubo3.rotate({0.01f, 0.000f, 0.00f});
        cameraLookAt.set_target(cubo1.get_global_position());
    });
    return 0;
}
