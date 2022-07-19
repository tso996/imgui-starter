//
//  main.cpp
//  OpenGLExperiments
//
//  Created by Joe on 18/06/2022.
//
#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <GL/glew.h>//this needs to be included before any other gl header
#include <GLFW/glfw3.h>



static unsigned int compileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();//&source[0]
    
    
    return 1;
}

static int createShader(const std::string& vertexShader, const std::string& fragmentShader){
    
    unsigned int program = glCreateProgram();
    unsigned int v = compileShader(GL_VERTEX_SHADER, vertexShader);
    
    return 1;
}



int main(int argc, const char * argv[]) {
    
    #ifdef DEBUG
        std::cout<<"debug build"<<std::endl;
    #else
        std::cout<<"release build"<<std::endl;
    #endif
    
    if (!glfwInit())
    {
        std::cout<<"glfw did not initialise properly.."<<std::endl;
        return -1;
    }else{
        std::cout<<"glfw initialised correctly.."<<std::endl;
    }
    
    //The following is required to change the opengl version from 2.1 to 4.1 in macs..
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //===============================================================


    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if(glewInit()!=GLEW_OK){//this needs to happen after the context is made
        std::cout<<"glew did not initialize properly.."<<std::endl;
        return -1;
    }else{
        std::cout<<"glew initialised correctly.."<<std::endl;
    }
    
    std::cout<<"gl version: "<<glGetString(GL_VERSION)<<std::endl;
    
    float positions[6] ={-0.5f, -0.5f,
                          0.0f,0.5f,
                          0.5f, -0.5f};
    
    unsigned int buffer;//variable to store the id of the buffer
    glGenBuffers(1/*could be more than one*/, &buffer);//creates a buffer and returns the id of the buffer..
    glBindBuffer(GL_ARRAY_BUFFER,buffer);//selecting or binding the target buffer
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
    //At this point the gpu just knows we have provided some data, it does not know the type of data, number of bytes of data, or that 2 elements from the positions array actually represent one point/vertex of the triangle..
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);//tell the gpu details about the bytes it has in the buffer with id
    glEnableVertexAttribArray(buffer);//enable the vertex

    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //leagcy OpenGL way
//        glBegin(GL_TRIANGLES);
//
//        glVertex2f(-0.5f, -0.5f);
//        glVertex2f(0.0f,0.5f);
//        glVertex2f(0.5f, -0.5f);
//
//        glEnd();
        
        glDrawArrays(GL_TRIANGLES, 0, 3);//drawcall. 0 is the starting index of array, 3 is number of vertices to render..
        
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//#if defined DEBUG
//    std::cout<<"debug build"<<std::endl;
//#else
//    std::cout<<"release build"<<std::endl;
//    std::filesystem::current_path(Game::exeParentPath);//if it is release build, then we change the current working directory to the parent path of where the executable file is located
//    std::cout<<"the current working directory has been changed to the executable file path.."<<std::endl;
//#endif
