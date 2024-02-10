// CMakeProject1.cpp: определяет точку входа для приложения.
//

#include "CMakeProject1.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
        
}

float verticies[] = {
    -1.f, -1.f,
    -1.f, 1.f,
    1.f, 1.f,

    1.f, 1.f,
    1.f, -1.f,
    -1.f, -1.f,
};

float color[] = {
    1.f, 0.f,
    0.f, 1.f,
    1.f, 0.f,

    1.f, 0.f,
    0.f, 1.f,
    1.f, 0.f,
};

int main(void)
{
    auto in = glm::vec2{ 1.f, 2.f };
    std::cout << in.x << ' ' << in.y << '\n';
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* monitor = glfwGetPrimaryMonitor();
    auto* mode = glfwGetVideoMode(monitor);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(mode->width, mode->height, "Hello World", monitor, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGL())
    {
        glfwTerminate();
        return -2;
    }
    std::cout << glGetString(GL_VERSION) << '\n';
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, mode->width, mode->height);
    ShaderClass shaderProgramm("./resources/shaders/base.vert", "./resources/shaders/base.frag");
    VAO vao{};
    vao.loadVBO(sizeof(verticies), verticies);
    vao.loadVBO(sizeof(color), color);
    

    glClearColor(0.f, 0.f, 0.f, 1.f);
    /*
    unsigned int VBO, CBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &CBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);    

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    */

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgramm.activate();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

