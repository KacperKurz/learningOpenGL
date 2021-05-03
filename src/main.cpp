#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "OpenGL app", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*initialize glew (we are using glew for openGL function declarations) */
    if (glewInit() != GLEW_OK)
        std::cout<< "ERROR: glewInit didn't initialize properly!\n";

    std::cout<< "INFO: OpenGl version is " <<glGetString(GL_VERSION)<<'n';

    float positions[6] = {
            -0.5f,-0.5f,
             0.0f, 0.5f,
             0.5f,-0.5f
    };

    unsigned int myTriangleBuffer;
    glGenBuffers(1, &myTriangleBuffer);                                             //generate one buffer and assign it's index number to myTriangleBuffer
    glBindBuffer(GL_ARRAY_BUFFER,myTriangleBuffer);                                 //set current buffer to one mentioned above
    glBufferData(GL_ARRAY_BUFFER,6 * sizeof(float), positions, GL_STATIC_DRAW);     //set the size of our buffer (in bytes), provide data, and give OpenGL hint to how our buffer will be used

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw a triangle using earlier defined buffer (note: the triangle won't be visible since we need to implement a shader) */
        glDrawArrays(GL_TRIANGLES,0,3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}