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
		-0.5f, -0.5f,  0.0f,   0.8f, 0.5f, 1.0f, // Lower left corner
		0.5f, -0.5f,   0.0f,   0.2f, 0.1f, 0.8f, // Lower right corner
		0.5f, 0.5f,    0.0f,   0.6f, 0.4f, 0.0f, // Upper corner
		-0.5f, 0.5f,   0.0f,   1.0f, 1.0f, 0.5f, // Inner left
	};

    GLuint indices[] = {
        0, 2, 3,
        0, 1, 2,
    };

    Shader shader("include/vertex.glsl", "include/frag.glsl");

    vao vao;
    vao.bind();

    vbo vbo(vert, sizeof(vert));
    ebo ebo(indices, sizeof(indices));

    vao.linkAttr(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.linkAttr(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    GLuint unID = glGetUniformLocation(shader._ID, "scale");


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

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // clear buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);

        // Outputs the matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shader._ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shader._ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shader._ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glUniform1f(unID, 0.5f);

        glEnable(GL_DEPTH_TEST);

        vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

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