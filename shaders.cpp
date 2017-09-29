#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

GLuint LoadShaders(const char *computeShaderPath) {
    
        // Create the shaders
        GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

        std::string ComputeShaderCode;
        std::ifstream ComputeShaderStream(computeShaderPath, std::ios::in);
        if(ComputeShaderStream.is_open()){
            std::string Line = "";
            while(getline(ComputeShaderStream, Line))
                ComputeShaderCode += "\n" + Line;
            ComputeShaderStream.close();
        }else{
            printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", computeShaderPath);
            getchar();
            return 0;
        }
    
        GLint Result = GL_FALSE;
        int InfoLogLength;
    
        printf("Compiling shader : %s\n", computeShaderPath);
        char const * ComputeSourcePointer = ComputeShaderCode.c_str();
        glShaderSource(computeShader, 1, &ComputeSourcePointer , NULL);
        glCompileShader(computeShader);
    
        glGetShaderiv(computeShader, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ComputeShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(computeShader, InfoLogLength, NULL, &ComputeShaderErrorMessage[0]);
            printf("%s\n", &ComputeShaderErrorMessage[0]);
        }

        // Link the program
        printf("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, computeShader);
        glLinkProgram(ProgramID);
    
        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }
    
        
        glDetachShader(ProgramID, computeShader);
        
        glDeleteShader(computeShader);
    
        return ProgramID;
    }