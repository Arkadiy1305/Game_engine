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
    oalpp::SoundContext ctx;
    oalpp::SoundDataBuilder builder;
    builder.fromFile("./resources/sound/Swords_battle.mp3");
    oalpp::SoundData buffer = builder.create();
    oalpp::Sound snd { buffer };
    snd.play();
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
    GameScreen g_scr(projection, mode, rect_shader, glm::ivec2{300, 300});
    

    glClearColor(1.f, 1.f, 1.f, 1.f);
    float last_time { 0.f };
    float delta_time { 0.f };
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        auto now = glfwGetTime();
        delta_time = now - last_time;
        if (snd.isPlaying()) {
            snd.update();
        }
        /* Render here */
        if (delta_time > 0.02f) {
            last_time = now;
            
            g_scr.update();
            
        }
        glClear(GL_COLOR_BUFFER_BIT);
        g_scr.render();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

