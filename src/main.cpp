#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Window.h"

static constexpr float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

static constexpr const char *vertexShaderSource = R"(
    #version 330
    layout (location=0) in vec3 position;

    void main() {
        gl_Position = vec4(position, 1.0);
    }
)";

static constexpr const char *fragmentShaderSource = R"(
    #version 330
    out vec4 fragColor;

    void main() {
        fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";


int main() {
    // destroy all resources with RAII before terminating GLFW
    {
        Window window(640, 480, "Hello World!");
        window.setClearColor(255 / 255.0, 192 / 255.0, 203 / 255.0, 0);

        ShaderProgram shaderProgram{};
        shaderProgram.createVertexShader(&vertexShaderSource);
        shaderProgram.createFragmentShader(&fragmentShaderSource);
        shaderProgram.linkProgram();

        // create the VAO and bind it
        unsigned int vaoId;
        glGenVertexArrays(1, &vaoId);
        glBindVertexArray(vaoId);

        // create the index vbo
        unsigned int vboId;
        glGenBuffers(1, &vboId);
        // copy the vertices into the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // define the structure of the VBO data in the VAO and enable it, setting it to index 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);


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
            glDrawArrays(GL_TRIANGLES, 0, 3);

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