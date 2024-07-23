#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return 0;
}

GLFWindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
if(window == NULL){
    std::cout << "Error creating window" << '/n';
    glfwTerminate();
    return -1;
}

glfwMakeContextCurrent(window);

if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Error init GLAD" << '/n';
    return -1;
}

glViewport(0, 0, 800, 600);

void framebuffer_size_callback(GLFWindow* window, int width, int height){

void framebuffer_size_callback(GLFWindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

void processInput(GLFWWindow *window){
    if(glfwGetKEy(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

while(!glfwWindowShouldClose(window)){
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

glfwTerminate();
return 0;