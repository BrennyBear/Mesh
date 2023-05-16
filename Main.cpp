/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// Author: Brennen Romo                                                                                                                                                                            ////////////////  
//////////////// Date: May 12th, 2023                                                                                                                                                                            ////////////////
//////////////// Project: Mesh class                                                                                                                                                                             ////////////////
//////////////// Description: This project displays a 3D cube loaded into a Mesh class. You can move the camera with w,a,s,d,q,and e and the cursor                                                              ////////////////
////////////////              The Lightsource can be moved with i,j,k,l,u,o. The Cube can be rotated with the left and right arrow key                                                                           ////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Vertex.h"

void windowSizeCallback(GLFWwindow* window, int width, int height);
void windowCloseCallback(GLFWwindow* window);
void keyboardInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
unsigned int loadTexture(char const* path);

/////////////////////// Global Settings //////////////////////////////////////////
const int screenHeight = 1200;
const int screenWidth = 1600;
float angle = 0.0f;


////////////////////// Lighting   ////////////////////////////////////////

glm::vec3 lightPos(1.2f, 2.0f, 2.0f);

////////////////////// Camera ////////////////////////////////////////////

Camera camera(glm::vec3(0.0f, 0.5f, 5.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
float xNorm = 0;
float yNorm = 0;

////////////////////// Time ////////////////////////////////////////////

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/////////////////////// Global Data ////////////////////////////////////////////
// Vertex Data for our Cube
std::vector<float> planeVertices = {
    0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 50.0f, 50.0f,
    0.5f, 0.0f, -0.5f,0.0f, 1.0f, 0.0f, 50.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 50.0f
};
unsigned int planeIndices[] = {
    0, 1, 3,
    1, 2, 3
};
//Vertex Vectors that will store our Mesh data
std::vector<Vertex> boxVerts, planeVerts;
std::vector<float> boxVertices = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


// Position data for our cube
glm::vec3 cubePosition(0, 0.5, 0);
/////////////////////////// Main Program ///////////////////

int main()
{
    //initialize GLFW
    if (!glfwInit())
    {
        //close program if we've failed to run GLFW
        std::cout << "failed to initialize GLFW";
        return -1;
    }
    //Definitions for when we are using Apple device
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //Set GLFW Window Parameters here (Using version 4.6)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //using Core profile of OpenGL, (Compatibility profile is for legacy)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //////////////////////////create Fullscreen Window here////////////////////////
    // GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Render Window", glfwGetPrimaryMonitor(), NULL);

    //////////////////////////create Windowed fullscreen here////////////////////////
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Render Window", NULL, NULL);
    ////////////////////////create window here////////////////////////////////
   // GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Render Window", NULL, NULL);
    if (!window)
    {
        std::cout << "Error Creating our GLFW Window!" << std::endl;
        //close down GLFW
        glfwTerminate();
        return -1;
    }
    // Make our Window the current Context
    glfwMakeContextCurrent(window);

    camera.setDimensions(mode->width, mode->height);
    /////////////////////////////////////////////////////////////////////////////////
    // /////////////////////////// Set Callback Functions Here ///////////////////////////
    // /////////////////////////////////////////////////////////////////////////////////
    //Sets the Callback function for when the frameBuffer is resized to our custom Callback function.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //Set callback Function for when Window is resized
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    //set callback function for when we close glfw Window 
    glfwSetWindowCloseCallback(window, windowCloseCallback);


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    // Start glad and load all openGL function pointers (for whichever specific system and archritecture our program is running on)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // Configure global penGl state
    glEnable(GL_DEPTH_TEST);

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////// Shaders //////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////


    Shader ourShader("shader.vs", "shader.fs");
    Shader lightShader("Light.vs", "Light.fs");
    ourShader.use();
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Textures & Materials   ////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    loadVertexArray(boxVertices, boxVerts);
    loadVertexArray(planeVertices, planeVerts);
    Mesh Mesh1(boxVerts, 36, NULL, 0);
    Mesh Mesh2(planeVerts, 4, planeIndices, 6);
    Mesh Mesh3(boxVerts,36, NULL, 0);

        // load and create a teture 
    // -------------------------
    Texture texture1("Resources/Textures/crate.jpg", GL_IMAGE_2D);
    Texture texture2("Resources/Textures/checkered.png", GL_IMAGE_2D);
    Texture texture3("Resources/Textures/Floor.jpg", GL_IMAGE_2D);


    Material mat1(texture1.getID(), texture1.getID(), texture1.getID(), 100);
    Material mat2(texture3.getID(), texture3.getID(), texture3.getID(), 100);

    mat1.sendToShader(ourShader);


    texture1.bind(texture1.getID());
    texture2.bind(texture2.getID());
    texture3.bind(texture3.getID());


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  


    // light properties
    ourShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Set initial Mesh transforms
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Mesh1.setPosition(cubePosition);
    Mesh2.setScale(glm::vec3(100.f, 1.f, 100.f));
    Mesh3.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    // Start Render Loop here
    do
    {

        //set constantly changing uniforms
        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setVec3("light.position", lightPos);

        //set Mesh transforms

        Mesh1.setRotation(glm::vec3(0.f, angle, 0.f));
        Mesh3.setPosition(lightPos);

        //calculate delta time

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        //input
        keyboardInput(window);
        camera.ProcessMouseMovement(xNorm, -yNorm);

        //clear the backbuffer to set colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ///////////////////////////////////////////////////////////////////////////////////////////
        //create variables for our cube's transformation
        ///////////////////////////////////////////////////////////////////////////////////////////
        glm::mat4 projection = camera.Projection;
        glm::mat4 view = camera.GetViewMatrix();

        //Set our Shader's variables

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Render our Meshes
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        mat1.sendToShader(ourShader);
        Mesh1.render(&ourShader);
        mat2.sendToShader(ourShader);
        Mesh2.render(&ourShader);

        //Render our lightsource
        mat1.sendToShader(lightShader);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setVec3("viewPos", camera.Position);
        lightShader.setVec3("light.position", lightPos);
        Mesh3.render(&lightShader);






        //Swap buffers
        glfwSwapBuffers(window);
        // poll windows events, call callback functions for events
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));




    // close GLFW
    glfwTerminate();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// FUNCTION DEFINITIONS           ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Function for Processing Inputs
void keyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //if Escape key is pressed, close Window
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //if Escape key is pressed, close Window
    {
        int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        if (!maximized)
            glfwMaximizeWindow(window);
        else
            glfwRestoreWindow(window);
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //if Escape key is pressed, close Window
    {
        angle += 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //if Escape key is pressed, close Window
    {
        angle -= 0.1f;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(ROLLL, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(ROLLR, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos -= glm::vec3(0, 0, 0.01);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos += glm::vec3(0, 0, 0.01);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos -= glm::vec3(0.01, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos += glm::vec3(0.01, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        lightPos += glm::vec3(0, 0.01, 0);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lightPos -= glm::vec3(0, 0.01, 0);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    xNorm = ((xpos - (camera.getWidth() / 2)) * 2) / camera.getWidth();
    yNorm = ((ypos - (camera.getHeight() / 2)) * 2) / camera.getHeight();

    camera.ProcessMouseMovement(xNorm, yNorm);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//Function callback for changing our Window's size
void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.setDimensions(width, height);
    std::cout << "Width: " << width << "\n" << "Height: " << height << "\n";
}

//Function callback for closing our Window
void windowCloseCallback(GLFWwindow* window)
{
    std::cout << "Goodbye!";
}



//Function for when screen is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Set viewport size to new viewport size. 
    glViewport(0, 0, width, height);
}