#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"

#include "Window.h"


namespace Shaders {
    extern const char *vertex;
    extern const char *fragment;
}

// defines a cube
constexpr float vertices[] = {
    //positions            //texture coords
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f
};

unsigned int loadTexture(const std::string &fileName, bool hasAlpha, int desired_channels = 0) {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // set the texture wrapping/filtering options (on the current texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, channels;
    auto data = stbi_load(fileName.c_str(), &width, &height, &channels, desired_channels);
    if (!data) {
        std::stringstream errmsg;
        errmsg << "Could not load texture at '" << fileName << "'.";
        throw std::runtime_error(errmsg.str());
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return textureId;
}

int main() {
    // destroy all resources with RAII before terminating GLFW
    {
        std::array<glm::vec3, 10> cubePositions = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        Window window(640, 480, "Hello World!");
        window.setClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        ShaderProgram shaderProgram{&Shaders::vertex, &Shaders::fragment};

        stbi_set_flip_vertically_on_load(true);
        auto texture1Id = loadTexture("resources/textures/container.jpg", false);
        auto texture2Id = loadTexture("resources/textures/awesomeface.png", true);

        // create the VAO and bind it
        unsigned int vaoId;
        glGenVertexArrays(1, &vaoId);
        glBindVertexArray(vaoId);

        // create the vbo
        unsigned int vboId;
        glGenBuffers(1, &vboId);
        // copy the vertices into the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // define the structure of the VBO data in the VAO
        constexpr auto stride = 5 * sizeof(float);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        // texture coords attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // activate the shaders
        shaderProgram.use();
        shaderProgram.setInt("texture1", 0);
        shaderProgram.setInt("texture2", 1);

        float aspectRatio = static_cast<float>(window.width()) / static_cast<float>(window.height());
        while (!window.shouldClose()) {
            // update the screen and clear for the next frame
            if (window.isResized()) {
                glViewport(0, 0, window.width(), window.height());
                window.setResized(false);
                aspectRatio = static_cast<float>(window.width()) / static_cast<float>(window.height());
            }
            window.clear();
            shaderProgram.use();

            glm::mat4 view{1.0f};
            // note that we're translating the scene in the reverse direction of where we want to move
            view = glm::translate(view, glm::vec3(0.0, 0.0f, -3.0f));
            shaderProgram.setMat4("view", view);

            glm::mat4 projection;
            projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
            shaderProgram.setMat4("projection", projection);

            // draw the triangle
            glActiveTexture(GL_TEXTURE0); // activate the texture unit before binding texture
            glBindTexture(GL_TEXTURE_2D, texture1Id);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2Id);

            glBindVertexArray(vaoId);

            for (unsigned int i = 0; i < cubePositions.size(); i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * static_cast<float>(i);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shaderProgram.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // draw the screen and process events
            window.update();
        }

        // clean up before ending the program
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vboId);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vaoId);
    }

    glfwTerminate();
    return 0;
}