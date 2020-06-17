#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static constexpr float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

static constexpr const char* vertexShaderSource = R"(
    #version 330
    layout (location=0) in vec3 position;

    void main() {
        gl_Position = vec4(position, 1.0);
    }
)";

static constexpr const char* fragmentShaderSource = R"(
    #version 330
    out vec4 fragColor;

    void main() {
        fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

unsigned int compileShaderProgram() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Could not compile vertex shader: " << infoLog << std::endl;
        std::exit(1);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Could not compile fragment shader: " << infoLog << std::endl;
        std::exit(1);
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // the shaders objects aren't needed once they've been linked into the program.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Could not link shader program: " << infoLog << std::endl;
        std::exit(1);
    }

    return shaderProgram;
}

int main() {
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    });

    if (!glfwInit()) {
        return 1;
    }

    // specify the desired version (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // create the window
    auto window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    // set callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    });

    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    // enable vsync
    glfwSwapInterval(1);

    glClearColor(255/255.0, 192/255.0, 203/255.0, 0);

    auto shaderProgram = compileShaderProgram();

    // create an object to hold VBOs
    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);

    // Bind vertex array object
    glBindVertexArray(vertexArrayObject);

    // create an object to hold index data
    unsigned int vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    // copy the vertices into the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // set the vertex attribute pointers (data passed to the vertex shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw the screen and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}