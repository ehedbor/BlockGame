#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ShaderProgram.h"
#include "Window.h"

namespace Shaders {
    extern const char *vertex;
    extern const char *fragment;
}

constexpr float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

constexpr unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
};

int main() {
    // destroy all resources with RAII before terminating GLFW
    {
        Window window(640, 480, "Hello World!");
        window.setClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        ShaderProgram shaderProgram{&Shaders::vertex, &Shaders::fragment};

        // create a texture
        stbi_set_flip_vertically_on_load(true);

        unsigned int texture1Id;
        glGenTextures(1, &texture1Id);
        glBindTexture(GL_TEXTURE_2D, texture1Id);
        // set the texture wrapping.filerting options (on the current texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, channels;
        auto data = stbi_load("resources/textures/container.jpg", &width, &height, &channels, 0);
        if (!data) {
            throw std::runtime_error("Could not load texture 'resources/textures/container.jpg'.");
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        // create another texture
        unsigned int texture2Id;
        glGenTextures(1, &texture2Id);
        glBindTexture(GL_TEXTURE_2D, texture2Id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load("resources/textures/awesomeface.png", &width, &height, &channels, 0);
        if (!data) {
            throw std::runtime_error("Could not load texture 'resources/textures/awesomeface.png'.");
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);


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
        constexpr auto stride = 8 * sizeof(float);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coords attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // create the ebo
        unsigned int eboId;
        glGenBuffers(1, &eboId);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // activate the shaders
        shaderProgram.use();
        shaderProgram.setInt("texture1", 0);
        shaderProgram.setInt("texture2", 1);

        while (!window.shouldClose()) {
            // update the screen and clear for the next frame
            if (window.isResized()) {
                glViewport(0, 0, window.width(), window.height());
                window.setResized(false);
            }
            window.clear();

            // draw the triangle
            glActiveTexture(GL_TEXTURE0); // activate the texture unit before binding texture
            glBindTexture(GL_TEXTURE_2D, texture1Id);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2Id);

            glBindVertexArray(vaoId);
            glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

            // draw the screen and process events
            window.update();
        }

        // clean up before ending the program
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vboId);
        glDeleteBuffers(1, &eboId);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vaoId);
    }

    glfwTerminate();
    return 0;
}