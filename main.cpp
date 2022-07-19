//  Created by Joe on 18/06/2022.
//
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GL/glew.h>//this needs to be included before any other gl header
#include <GLFW/glfw3.h>

namespace shaderFunctions{

    static void CheckStatus( GLuint obj )
    {
        GLint status = GL_FALSE;
        if( glIsShader(obj) ) glGetShaderiv( obj, GL_COMPILE_STATUS, &status );
        if( glIsProgram(obj) ) glGetProgramiv( obj, GL_LINK_STATUS, &status );
        if( status == GL_TRUE ) return;
        GLchar log[ 1 << 15 ] = { 0 };
        if( glIsShader(obj) ) glGetShaderInfoLog( obj, sizeof(log), NULL, log );
        if( glIsProgram(obj) ) glGetProgramInfoLog( obj, sizeof(log), NULL, log );
        std::cerr << log << std::endl;
        std::exit( EXIT_FAILURE );
    }

    static void AttachShader( GLuint program, GLenum type, const char* src )
    {
        GLuint shader = glCreateShader( type );
        glShaderSource( shader, 1, &src, NULL );
        glCompileShader( shader );
        CheckStatus( shader );
        glAttachShader( program, shader );
        glDeleteShader( shader );
    }

    static GLuint Load( const char* shader, ... )
    {
        GLuint prog = glCreateProgram();
        va_list args;
        va_start( args, shader );
        while( shader )
        {
            const GLenum type = va_arg( args, GLenum );
            AttachShader( prog, type, shader );
            shader = va_arg( args, const char* );
        }
        va_end( args );
        glLinkProgram( prog );
        CheckStatus( prog );
        return prog;
    }
}

int main(int argc, const char * argv[]) {
    
    std::cout<<"STARTING PROGRAM"<<std::endl;

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //===============================================================


    GLFWwindow* window = glfwCreateWindow(1280, 960, "Shapes", NULL, NULL);
    
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
    
    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );


    float positions[ 6 ] =
    {
        -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f,
    };
    
    unsigned int buffer;//variable to store the id of the buffer
    glGenBuffers(1/*could be more than one*/, &buffer);//creates a buffer and returns the id of the buffer..
    glBindBuffer(GL_ARRAY_BUFFER,buffer);//selecting or binding the target buffer
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
    //At this point the gpu just knows we have provided some data, it does not know the type of data, number of bytes of data, or that 2 elements from the positions array actually represent one point/vertex of the triangle..
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);//tell the gpu details about the bytes it has in the buffer with id
    std::cout<<"buffer id: "<<buffer<<std::endl;
    glEnableVertexAttribArray(0);//enable the vertex

    std::string vertexShader =
        "#version 410 core\n"
        "layout(location = 0) in vec4 aPos;\n"
        "uniform float size;\n"
        "void main(){\n"
        "   gl_Position = vec4(aPos.x*size, aPos.y*size, aPos.z*size, 1.0);\n"
        "}\n";

    std::string fragmentShader =
        "#version 410 core\n"
        "out vec4 shadeOut;\n"
        "uniform vec4 color;\n"
        "void main(){\n"
        "   shadeOut = color;\n"
        "}\n";
    unsigned int shader = shaderFunctions::Load( vertexShader.c_str(), GL_VERTEX_SHADER, fragmentShader.c_str(), GL_FRAGMENT_SHADER, NULL );
    //imgui code
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 410");
    //========

    bool drawTriangle = false;
    float size = 1.0f;
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "size"),size);
    glUniform4f(glGetUniformLocation(shader, "color"), color[0],color[1],color[2], color[3]);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //imgui initialization
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(!io.WantCaptureMouse){
            //mouse not on gui
            
        }else{
            //mouse on gui
        }
  
        if(drawTriangle){
            glDrawArrays(GL_TRIANGLES, 0, 3);//drawcall. 0 is the starting index of array, 3 is number of vertices to render..
        }
        //imgui function
        ImGui::Begin("Console");
        ImGui::Text("Select a shape");
        ImGui::Checkbox("Triangle", &drawTriangle);
        ImGui::SliderFloat("Size",&size, 0.5f, 2.0f);
        ImGui::ColorEdit4("Color", color);
        ImGui::End();
        
        glUseProgram(shader);
        glUniform1f(glGetUniformLocation(shader, "size"),size);
        glUniform4f(glGetUniformLocation(shader, "color"), color[0],color[1],color[2], color[3]);


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    //imgui wind up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout<<"Quitting.."<<std::endl;
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
