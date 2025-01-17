#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "ebo.h"
#include "vao.h"
#include "vbo.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 640*2;
const unsigned int SCR_HEIGHT = 640*2;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLfloat vert[] = {
        //     COORDINATES     /        COLORS          /        NORMALS       //
	    -0.5f, 0.0f,  0.5f,     0.8f, 0.7f, 0.4f, 	 0.0f, -1.0f, 0.0f, // Bottom side
        -0.5f, 0.0f, -0.5f,     0.1f, 0.7f, 0.8f,	 0.0f, -1.0f, 0.0f, // Bottom side
        0.5f, 0.0f, -0.5f,     0.8f, 0.2f, 0.4f,	     0.0f, -1.0f, 0.0f, // Bottom side
        0.5f, 0.0f,  0.5f,     0.4f, 0.70f, 0.44f,	     0.0f, -1.0f, 0.0f, // Bottom side

        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 -0.8f, 0.5f,  0.0f, // Left Side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 -0.8f, 0.5f,  0.0f, // Left Side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	     -0.8f, 0.5f,  0.0f, // Left Side

        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.5f, -0.8f, // Non-facing side
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	     0.0f, 0.5f, -0.8f, // Non-facing side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,      0.0f, 0.5f, -0.8f, // Non-facing side

        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	     0.8f, 0.5f,  0.0f, // Right side
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	     0.8f, 0.5f,  0.0f, // Right side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	     0.8f, 0.5f,  0.0f, // Right side

        0.5f, 0.0f,  0.5f,     0.1f, 0.70f, 0.44f,	     0.0f, 0.5f,  0.8f, // Facing side
        -0.5f, 0.0f,  0.5f,     0.4f, 0.40f, 0.44f, 	 0.0f, 0.5f,  0.8f, // Facing side
        0.0f, 0.8f,  0.0f,     0.0f, 0.86f, 0.76f,	     0.0f, 0.5f,  0.8f  // Facing side
	};

    GLuint indices[] = {
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14 // Facing side
    };

    GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

    Shader shader("include/vertex.glsl", "include/frag.glsl");

    vao vao;
    vao.bind();

    vbo vbo(vert, sizeof(vert)); //Remove the sizeof
    ebo ebo(indices, sizeof(indices));

    vao.linkAttr(vbo, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0); // coords
    vao.linkAttr(vbo, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float))); // colors
    vao.linkAttr(vbo, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float))); // normals
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    Shader lightShader("include/lightVert.glsl", "include/lightFrag.glsl");

    class::vao lightVao;
    lightVao.bind();

    class::vbo lightVbo(lightVertices, sizeof(lightVertices));
    class::ebo lightEbo(lightIndices, sizeof(lightIndices));

    lightVao.linkAttr(lightVbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    lightVao.unbind();
    lightVbo.unbind();
    lightEbo.unbind();

    glm::vec4 lightColor = glm::vec4(1.0f, 0.5f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

    lightShader.use();
    glUniformMatrix4fv(glGetUniformLocation(lightShader._ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader._ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader._ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shader._ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shader._ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    

    // std::vector<glm::vec3> gridVerts;
    // int n = 5;
    // for(int i=0; i<=n; ++i) {
    //     // vertical line
    //     gridVerts.push_back(glm::vec3(i, 0, 0));
    //     gridVerts.push_back(glm::vec3(i, 0, n));
    //     // horizontal line
    //     gridVerts.push_back(glm::vec3(0, 0, i));
    //     gridVerts.push_back(glm::vec3(n, 0, i));
    // }

    glEnable(GL_DEPTH_TEST);

    camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // clear buffers
        glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.inputs(window);
        camera.updateMatrix(45.0f, 0.5f, 100.0f);
        shader.use();
        glUniform3f(glGetUniformLocation(shader._ID, "camPos"), camera._pos.x, camera._pos.y, camera._pos.z);
        camera.matrix(shader, "camMatrix");

        vao.bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        lightShader.use();
        camera.matrix(lightShader, "camMatrix");
        lightVao.bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    vao.del();
    vbo.del();
    ebo.del();
    shader.del();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}