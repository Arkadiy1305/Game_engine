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


int main(void)
{
    srand(time(NULL));
    oalpp::SoundContext ctx;
    
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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << glGetString(GL_VERSION) << '\n';
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, mode->width, mode->height);
    ShaderClass rect_shader("./resources/shaders/base.vert", "./resources/shaders/base.frag");
    
    
    const auto projection = glm::ortho(0.f, static_cast<float>(mode->width), 0.f, static_cast<float>(mode->height));
    rect_shader.setMat4("projection", projection);
    const glm::mat4 view { 1.f };
    rect_shader.setMat4("view", view);
    GameScreen g_scr(projection, mode, rect_shader, glm::ivec2{140, 100});
    

    glClearColor(1.f, 1.f, 1.f, 1.f);
    float now;
    float lastTime { 0.f };
    float deltatime { 0.f };
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        now = glfwGetTime();
        deltatime = now - lastTime;
        
        if (glfwGetKey(window, GLFW_KEY_UP)) {
            g_scr.input(GLFW_KEY_UP);
        } else if (glfwGetKey(window, GLFW_KEY_LEFT)) {
            g_scr.input(GLFW_KEY_LEFT);
        } else if (glfwGetKey(window, GLFW_KEY_DOWN)) {
            g_scr.input(GLFW_KEY_DOWN);
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
            g_scr.input(GLFW_KEY_RIGHT);
        }
        /* Render here */
        
        glClear(GL_COLOR_BUFFER_BIT);

        if (deltatime > 0.001f) { 
            lastTime = now;
            g_scr.update();
        }
        
        g_scr.render();
        
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

