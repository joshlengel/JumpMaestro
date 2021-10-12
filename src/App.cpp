#include"App.h"
#include"Constants.h"
#include"StartState.h"

#include<iostream>
#include<chrono>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

App::App(int width, int height, const char *title):
    m_machine(),
    m_window()
{
    if (!glfwInit())
    {
        std::cerr << "Error initializing GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int w, int h) { glViewport(0, 0, w, h); });

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Error initializing GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    Constants::Init("assets/constants.json");

    m_machine.Init<StartState>();
}

App::~App()
{
    glfwTerminate();
}

void App::Start()
{
    glfwShowWindow(m_window);

    std::chrono::high_resolution_clock::time_point t1, t2;
    t1 = t2 = std::chrono::high_resolution_clock::now();

    while (!m_machine.ShouldExit() && !glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        t2 = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count();
        t1 = t2;

        m_machine.Update(dt);
        m_machine.Render(m_window);

        glfwSwapBuffers(m_window);
    }
}