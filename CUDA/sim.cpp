#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "simCuda.h"
int numberParticles = 1023;
//Init Camera
Camera camera(glm::vec3(0.0f,0.0f,25.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool paused = false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// Window dimensions
const GLuint WIDTH = 1900, HEIGHT = 1000;
//Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint genCircleTexture();
GLfloat rFloat(GLuint Max)
{
   return (GLfloat)(((float)rand()/(float)RAND_MAX)*(float)Max);
}
// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);
    //Enable Point Drawing
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Build and compile our shader program
    Shader ourShader("default.vs", "default.frag");
    GLfloat *particles =(GLfloat *)malloc(sizeof(GLfloat)*numberParticles*8);
    printf("size = %lu",sizeof(GLfloat)*numberParticles*8);
    for(int i =0; i< numberParticles; i++)
    {
      particles[8*i]=5.0f-rFloat(10);
      particles[8*i+1]=5.0f-rFloat(10);
      particles[8*i+2]=-5.0f-rFloat(2.0);
      particles[8*i+3]=0.0f;
      particles[8*i+4]=0.0f;
      particles[8*i+5]=0.0f;
      if(i>=numberParticles/2)
      {
      particles[8*i+6]=4.0f;
      particles[8*i+7]=1.0f;
      }
      else
      {
        particles[8*i+6]=2.0f;
        particles[8*i+7]=-1.0f;
      }
    }
    // for(int i = 0; i<numberParticles*8;i++)
    // {
    //   std::cout<<sizeof(particles)<<"   "<<particles[i]<<"   "<<i<<std::endl;
    // }
    // X    Y   Z   VX    VY    VZ    Mass    Charge
    // 0    1   2   3     4     5     6       7
    for (int i =0; i<8*numberParticles; i++)
    {
      std::cout<<particles[i]<<":";
      if((i+1)%8==0)
      std::cout<<std::endl;
    }
        GLuint VBO, VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,8*numberParticles*sizeof(GLfloat),particles,GL_STREAM_DRAW);
    //Position Attributes
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //Mass Atrribute
    glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid *)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //Charge Attribute
    glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid *)(7*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

  //  Set up vertex data (and buffer(s)) and attribute pointers
    // GLfloat vertices[] = {
    //     // Positions         // Colors
    //     0.5f, -0.5f, -2.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
    //    -0.5f, -0.5f, -2.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
    //     0.0f,  0.5f, -2.0f,   0.0f, 0.0f, 1.0f   // Top
    // };
    // GLuint VBO, VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    // glBindVertexArray(VAO);
    //
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //
    // // Position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    // glEnableVertexAttribArray(0);
    // // Color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);
    //
    // glBindVertexArray(0); // Unbind VAO
    // //Game loop
    GLuint texture = genCircleTexture();
    int count = 0;
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        GLfloat currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        Do_Movement();
        // Render
        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Draw the triangle
        ourShader.Use();
        //Setup tranform matricies
        glm::mat4 view, projection,model;
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(projection));
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBindVertexArray(VAO);
        //model = glm::rotate(model,(GLfloat)glfwGetTime()*20.0f,glm::vec3(0.0f,0.0f,1.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        if(!paused)
        particles=calcAndUpdate(particles,numberParticles);
        // particles[0]=0;
        // particles[1]=0;
        // particles[2]=0.0;
        // particles[3]=0.0f;
        // particles[4]=0.0f;
        // particles[5]=0.0f;
        // particles[6]=5000.0f;
        // particles[7]=1.0f;
        #ifdef DEBUG
        for (int i =0; i<8*numberParticles; i++)
        {
          std::cout<<particles[i]<<":";
          if((i+1)%8==0)
          std::cout<<std::endl;
        }
        std::cout<<"Done with try "<<count<<std::endl;
        #endif
        glBufferData(GL_ARRAY_BUFFER,8*numberParticles*sizeof(GLfloat),particles,GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS , 0, numberParticles);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        // Swap the screen buffers
        glfwSwapBuffers(window);
        count++;


    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
GLuint genCircleTexture()
{
  GLuint texture_name;
  const GLint texWidth = 256;
    const GLint texHeight = 256;
    const float texHalfWidth = 128.0f;
    const float texHalfHeight = 128.0f;
    printf("INIT: \n");

    unsigned char* pData = new unsigned char[texWidth*texHeight*4];
    for(int y=0; y<texHeight; ++y){
        for(int x=0; x<texWidth; ++x){
            int offs = (x + y*texWidth) * 4;
            float xoffs = ((float)x - texHalfWidth) / texHalfWidth;
            float yoffs = ((float)y - texHalfWidth) / texHalfHeight;
            float alpha = 1.0f - std::sqrt(xoffs*xoffs + yoffs*yoffs);
            if(alpha < 0.0f)
                alpha = 0.0f;
            pData[offs + 0] = 255; //r
            pData[offs + 1] = 0; //g
            pData[offs + 2] = 0; //b
            pData[offs + 3] = 255.0f * alpha; // *
            //printf("alpha: %f\n", pData[x + y*texWidth + 3]);
        }
    }

    glGenTextures(1, &texture_name);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
    glEnable(GL_POINT_SPRITE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D,0);
    return texture_name;
}
// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        {
          camera.ProcessKeyboard(FORWARD, deltaTime);
        }
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  //  std::cout << key << std::endl;
   if(key=='P')
   paused=true;
   else if (key=='U')
   paused = false;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
