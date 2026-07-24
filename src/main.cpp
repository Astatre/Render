#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "filesystem.h"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "model.hpp"

#include <iostream>

// ============================================================================
// CONSTANTS
// ============================================================================
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
const float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
};

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// ============================================================================
// CALLBACK FUNCTIONS (Forward Declarations)
// ============================================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// ============================================================================
// UTILITY FUNCTIONS (Forward Declarations)
// ============================================================================
unsigned int loadTexture(const char* path);

// ============================================================================
// MAIN FUNCTION
// ============================================================================
/*
int main()
{
    // Initialize GLFW and create window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure OpenGL
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Load shaders and models
    Shader ourShader("src/vertexShader.glsl", "src/fragmentShader.glsl");
    Shader lightShader("src/lightVertexShader.glsl", "src/lightFragmentShader.glsl");
    Model ourModel("res/backpack/backpack.obj");

    // Setup light data
  

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f, 0.2f, 2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3( 0.0f, 0.0f, -3.0f)
    };

    // Setup light VAO
    unsigned int lightVAO, VBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //Stencil testing
    glEnable(GL_STENCIL_TEST);


    //GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL and GL_ALWAYS.
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);// pass test if stencil value is not 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glStencilFunc(GL_EQUAL, 1, 0xFF); // pass test if stencil value is 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // each bit is written to as is
    


    // ========================================================================
    // RENDER LOOP
    // ========================================================================
    while (!glfwWindowShouldClose(window))
    {
        // Update timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Setup transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // Render model
        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);

        // Set lighting
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        for(unsigned int i = 0; i < 4; i++)
        {
            std::string number = std::to_string(i);
            ourShader.setVec3("pointLights[" + number + "].position", pointLightPositions[i]);
            ourShader.setVec3("pointLights[" + number + "].ambient", 0.05f, 0.05f, 0.05f);
            ourShader.setVec3("pointLights[" + number + "].diffuse", 0.8f, 0.8f, 0.8f);
            ourShader.setVec3("pointLights[" + number + "].specular", 1.0f, 1.0f, 1.0f);
            ourShader.setFloat("pointLights[" + number + "].constant", 1.0f);
            ourShader.setFloat("pointLights[" + number + "].linear", 0.09f);
            ourShader.setFloat("pointLights[" + number + "].quadratic", 0.032f);
        }

        //ourShader.setVec3("spotLight.position", camera.Position);
        //ourShader.setVec3("spotLight.direction", camera.Front);
        ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.09f);
        ourShader.setFloat("spotLight.quadratic", 0.032f);
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        ourShader.setFloat("material.shininess", 64.0f);
        ourModel.Draw(ourShader);

        // Render light cubes
        glBindVertexArray(lightVAO);
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        for(unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4("model", model);
            lightShader.setVec3("lightColor", lightColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
*/
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // build and compile shaders
    // -------------------------
    Shader shader("src/stencil.vs", "src/stencil.fs");
    Shader shaderSingleColor("src/stencil.vs", "src/borderFragmentShader.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
float cubeVertices[] = {
    // back face (normal -z)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // LBB
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // RTB
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // RBB
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // RTB
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // LBB
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // LTB

    // front face (normal +z)
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // LBF
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // RBF
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // RTF
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // RTF
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // LTF
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // LBF

    // left face (normal -x)
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // LTF
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // LTB
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // LBB
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // LBB
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // LBF
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // LTF

    // right face (normal +x)
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // RTF
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // RBB
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // RTB
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // RBB
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // RTF
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // RBF

    // bottom face (normal -y, légèrement agrandie comme dans l'original)
    -0.51f, -0.51f,  0.51f, 0.0f, 0.0f, // LBF'
     0.51f, -0.51f, -0.51f, 1.0f, 1.0f, // RBB'
     0.51f, -0.51f,  0.51f, 1.0f, 0.0f, // RBF'
     0.51f, -0.51f, -0.51f, 1.0f, 1.0f, // RBB'
    -0.51f, -0.51f,  0.51f, 0.0f, 0.0f, // LBF'
    -0.51f, -0.51f, -0.51f, 0.0f, 1.0f, // LBB'

    // top face (normal +y)
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // LTB
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // RTF
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // RTB
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // RTF
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // LTB
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f  // LTF
};
    float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f
};
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);


    vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3( 1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3( 0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3( 0.5f, 0.0f, -0.6f));


    // grass VAO 
    unsigned int vegetationVAO, vegetationVBO;
    glGenVertexArrays(1,&vegetationVAO);
    glGenBuffers(1, &vegetationVBO);
    glBindVertexArray(vegetationVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vegetationVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);


    
    // load textures
    // -------------
    stbi_set_flip_vertically_on_load(true);
    unsigned int cubeTexture = loadTexture("res/textures/marble.jpg");
    unsigned int floorTexture = loadTexture("res/textures/metal.png");
    unsigned int grassTexture = loadTexture("res/textures/grass.png");
    unsigned int glassTexture = loadTexture("res/textures/window.png");
    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    std::map<float, glm::vec3> sorted;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!

        // set uniforms
        shaderSingleColor.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderSingleColor.setMat4("view", view);
        shaderSingleColor.setMat4("projection", projection);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);




        // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
        glStencilMask(0x00);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // 1st. render pass, draw objects as normal, writing to the stencil buffer
        // --------------------------------------------------------------------
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glBindVertexArray(vegetationVAO);
        glBindTexture(GL_TEXTURE_2D, glassTexture);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        /*
        for(unsigned int i = 0; i < vegetation.size(); i++)
        {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vegetation[i]);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        }*/
       glDisable(GL_CULL_FACE);
        for (unsigned int i = 0; i < vegetation.size(); i++){
            float distance = glm::length(camera.Position - vegetation[i]);
            sorted[distance] = vegetation[i];
        }
        for(std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it !=
        sorted.rend(); ++it){
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glEnable(GL_CULL_FACE);


        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
        // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
        // the objects' size differences, making it look like borders.
        // -----------------------------------------------------------------------------------------------------------------------------
        /*
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        shaderSingleColor.use();
        float scale = 1.1f;
        // cubes
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        */
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

// ============================================================================
// CALLBACK IMPLEMENTATIONS
// ============================================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.Position.y += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.Position.y -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// ============================================================================
// UTILITY IMPLEMENTATIONS
// ============================================================================
unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
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
