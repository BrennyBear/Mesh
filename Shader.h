#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm.hpp>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Class used to store and manage our Shader Programs ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Shader
{
public:
    unsigned int ID;
    ///////////////////////// Constructor Function ////////////////////////////////////////////////
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // Variables used to store infor about our two programs
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;


        // code to allow istreams to throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open our Shader Files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close our shader files
            vShaderFile.close();
            fShaderFile.close();
            // store our streams as strings
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        //if an error is thrown, catch it here
        catch (std::ifstream::failure& e)
        {
            //print the thrown exception
            std::cout << "Error occurred while attempting to read a Shader File:" << e.what() << std::endl;
        }

        //Convert our ShaderCode Strings into cStrings.
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        //Declare our Shaders
        unsigned int vertex, fragment;
        // create Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        //Specify where Shader Code is located
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        //Compile Shader Code
        glCompileShader(vertex);
        //Check for Errors
        checkCompileErrors(vertex, "VERTEX");
        // Create fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        //Specify where Shader Code is located
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        //Compile Shader Code
        glCompileShader(fragment);
        //Check for Errors
        checkCompileErrors(fragment, "FRAGMENT");
        
        //Create actual Program
        ID = glCreateProgram();
        //attach Vertex Shader
        glAttachShader(ID, vertex);
        //Attach Fragment Shader
        glAttachShader(ID, fragment);
        //Link Program
        glLinkProgram(ID);
        //Check for errors
        checkCompileErrors(ID, "PROGRAM");
        // Delete our Shader Proograms, now that they are already linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }


    ////////////////////////////////////////// Set this as Active Shader ////////////////////////////////////////////////////////
    void use()
    {
        glUseProgram(ID);
    }

    void unuse()
    {
        glUseProgram(0);
    }
    ///////////////////////////////////////// Set a Specific Bool       /////////////////////////////////////////////////////////
    void setBool(const std::string& name, bool value) 
    {
        //this->use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Specific Int        /////////////////////////////////////////////////////////
    void setInt(const std::string& name, int value) 
    {
        //this->use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Specific Float      /////////////////////////////////////////////////////////
    void setFloat(const std::string& name, float value)
    {
        //this->use();
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Vec 2 /////////////////////////////////////////
    void setVec2(const std::string& name, const glm::vec2& value)
    {
        //this->use();
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        //this->unuse();
    }
    void setVec2(const std::string& name, float x, float y)
    {
        //this->use();
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Vec3 /////////////////////////////////////////
    void setVec3(const std::string& name, const glm::vec3& value)
    {
        //this->use();
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        //this->unuse();
    }
    void setVec3(const std::string& name, float x, float y, float z)
    {
        //this->use();
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Vec4  /////////////////////////////////////////
    void setVec4(const std::string& name, const glm::vec4& value)
    {
        //this->use();
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        //this->unuse();
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        //this->use();
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Mat2 /////////////////////////////////////////
    void setMat2(const std::string& name, const glm::mat2& mat)
    {
        //this->use();
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Mat3 /////////////////////////////////////////
    void setMat3(const std::string& name, const glm::mat3& mat)
    {
        //this->use();
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        //this->unuse();
    }
    ///////////////////////////////////////// Set a Mat4 /////////////////////////////////////////
    void setMat4(const std::string& name, const glm::mat4& mat) 
    {
        //this->use();
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        //this->unuse();
    }
   
private:
    ///////////////////////////////////////// Check for specific Errors ///////////////////////////////////////
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Error while compiling Shader type: " << type << "\n" << infoLog << "\n" << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Error While Linking Program. type:" << type << "\n" << infoLog << "\n" <<  std::endl;
            }
        }
    }
};