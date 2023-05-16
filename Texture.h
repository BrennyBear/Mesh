#pragma once

#include<iostream>

#include "glad/glad.h"
#include<GLFW/glfw3.h>
#include "stb_image.h"

class Texture
{
private:
    GLuint id;
    int width;
    int height;
    GLenum type;
public:
    Texture(const char* fileName, GLenum type)
    {
        this->type = type;


        glGenTextures(1, &this->id);
        glBindTexture(GL_TEXTURE_2D, this->id);
        int nrComponents;


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(fileName, &this->width, &this->height, &nrComponents, 0);


        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;


            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);



            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << fileName << std::endl;
            stbi_image_free(data);
        }
    }
    ~Texture()
    {
        glDeleteTextures(1, &this->id);
    }

    inline GLuint getID() const { return this->id; }

    void bind(const GLint texture_unit)
    {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, this->id);
    }

    void unbind()
    {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void loadFromFile(const char* fileName)
    {
        if (this->id)
        {
            glDeleteTextures(1, &this->id);
        }

        int nrComponents;
        unsigned char* data = stbi_load(fileName, &this->width, &this->height, &nrComponents, 0);

        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;


            glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(type);

            glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << fileName << std::endl;
            stbi_image_free(data);
        }
    }


    bool operator==(const Texture& tex)
    {
        if (this->id == tex.id)
            return true;
        else
            return false;
    }

    bool operator!=(const Texture& tex)
    {

        if (this->id != tex.id)
            return true;
        else
            return false;
    }

};