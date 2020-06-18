#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Window.h"

namespace Shaders {
    extern const char *vertex;
    extern const char *fragment;
}

int main() {
    // destroy all resources with RAII before terminating GLFW
    {
        Window window(640, 480, "Hello World!");
        window.setClearColor(255 / 255.0, 192 / 255.0, 203 / 255.0, 0);

        ShaderProgram shaderProgram{&Shaders::vertex, &Shaders::fragment};

        constexpr float vertices[] = {
             0.5f,  0.5f, 0.0f, // top right
             0.5f, -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f, // top left
        };

        constexpr unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3, // second triangle
        };

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
        // define the structure of the VBO data in the VAO and enable it, setting it to index 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        // create the ebo
        unsigned int eboId;
        glGenBuffers(1, &eboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        while (!window.shouldClose()) {
            // update the screen and clear for the next frame
            if (window.isResized()) {
                glViewport(0, 0, window.width(), window.height());
                window.setResized(false);
            }
            window.clear();

            // draw the triangle
            shaderProgram.bind();
            glBindVertexArray(vaoId);
            glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

            // restore state
            glBindVertexArray(0);
            shaderProgram.unbind();

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