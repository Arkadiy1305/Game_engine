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
    -100.f, -100.f,
    -100.f, 100.f,
    100.f, 100.f,
    100.f, -100.f,
    
};

float color[] = {
    1.f, 0.f,
    0.f, 1.f,
    1.f, 0.f,
    0.f, 1.f,
    
};

unsigned int indicies[] = {
    0, 1, 2,
    2, 3, 0,
};

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << glGetString(GL_VERSION) << '\n';
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, mode->width, mode->height);
    ShaderClass shaderProgramm("./resources/shaders/base.vert", "./resources/shaders/base.frag");
    VAO vao{};
    vao.loadVBO(sizeof(verticies), verticies, 2, GL_STATIC_DRAW);
    vao.loadVBO(sizeof(color), color, 2, GL_STATIC_DRAW);
    vao.loadEBO(sizeof(indicies), indicies);
    
    auto projection = glm::ortho(0.f, static_cast<float>(mode->width), 0.f, static_cast<float>(mode->height));
    shaderProgramm.setMat4("projection", projection);
    TextRender text(projection, "./resources/fonts/astron_boy_italic.ttf");
    auto view{ glm::mat4{1.f} };
    shaderProgramm.setMat4("view", view);

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
        if (snd.isPlaying()) {
            snd.update();
        }
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgramm.activate();
        vao.bind();

        auto model{ glm::mat4{1.f} };
        model = glm::translate(model, glm::vec3{ 200.f, 200.f, 0.f });
        model = glm::rotate(model, 1.f, glm::vec3(0.f, 0.f, 1.f));
        shaderProgramm.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        auto model2{ glm::mat4{1.f} };
        model2 = glm::translate(model2, glm::vec3{ 1700.f, 200.f, 0.f });
        model2 = glm::rotate(model2, 1.f, glm::vec3(0.f, 0.f, 1.f));
        shaderProgramm.setMat4("model", model2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        auto model3{ glm::mat4{1.f} };
        model3 = glm::translate(model3, glm::vec3{ 200.f, 900.f, 0.f });
        model3 = glm::rotate(model3, 1.f, glm::vec3(0.f, 0.f, 1.f));
        shaderProgramm.setMat4("model", model3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        auto model4{ glm::mat4{1.f} };
        model4 = glm::translate(model4, glm::vec3 { 1700.f, 900.f, 0.f });
        model4 = glm::rotate(model4, 1.f, glm::vec3(0.f, 0.f, 1.f));
        shaderProgramm.setMat4("model", model4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        text.render("HELLO", 500, 500, 5, glm::vec3 { 1.f, 1.f, 1.f });
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

