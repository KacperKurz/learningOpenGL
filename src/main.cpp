#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath){
    std::ifstream stream(filepath);

    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream,line)){
        if (line.find("#shader") != std::string::npos){
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else{
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    return {ss[0].str(),ss[1].str()};
}

static unsigned int CompileShader(unsigned int type,const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id,length,&length,message);
        std::cout<<"Error: failed to compile "<< (type==GL_VERTEX_SHADER?"vertex":"fragment") <<" shader: \n"<<message<<'\n';
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "OpenGL app", nullptr, nullptr);
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

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,nullptr);           //tell OpenGl how to interpret our buffer
    glEnableVertexAttribArray(0);                                                   //remember to enable it!

    ShaderProgramSource source = ParseShader("../res/shaders/triangle.shader");
    unsigned int shader = CreateShader(source.VertexSource,source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw a triangle using earlier defined buffer (note: the triangle won't be visible since we need to implement a shader and vertex attributes) */
        glDrawArrays(GL_TRIANGLES,0,3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}