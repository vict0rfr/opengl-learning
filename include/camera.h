#ifndef CAMERA
#define CAMERA
#define GLM_ENABLE_EXPERIMENTAL

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shader.h"


class camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 _pos;
	glm::vec3 _orientation;
	glm::vec3 _up;
    glm::mat4 _cameraMatrix;

	// Prevents the camera from jumping around when first clicking left click
	bool _firstClick;

	int _width;
	int _height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float _speed;
	float _sens;

	// Camera constructor to set up initial values
	camera(int width, int height, glm::vec3 pos) : 
        _width(width), 
        _height(height), 
        _pos(pos), 
        _orientation(glm::vec3(0.0f, 0.0f, -1.0f)), 
        _up(glm::vec3(0.0f, 1.0f, 0.0f)),
        _firstClick(true), 
        _speed(0.1f), 
        _sens(100.0f),
        _cameraMatrix(glm::mat4(1.0f))
    {}

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane){
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        view = glm::lookAt(_pos, _pos + _orientation, _up);
        proj = glm::perspective(glm::radians(FOVdeg), (float)(_width/_height), nearPlane, farPlane);

        _cameraMatrix = proj * view;
    }

	// Updates and exports the camera matrix to the Vertex Shader
	void matrix(Shader& shader, const char* uniform){
        glUniformMatrix4fv(glGetUniformLocation(shader._ID, uniform), 1, GL_FALSE, glm::value_ptr(_cameraMatrix));
    }
	// Handles camera inputs
	void inputs(GLFWwindow* window){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            _pos += _speed * _orientation;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            _pos += _speed * -glm::normalize(glm::cross(_orientation, _up));
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            _pos += _speed * -_orientation;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            _pos += _speed * glm::normalize(glm::cross(_orientation, _up));
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            _pos += _speed * _up;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            _pos += _speed * -_up;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            _speed = 0.4f;
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            _speed = 0.1f;


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            // Hides mouse cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            // Prevents camera from jumping on the first click
            if (_firstClick)
            {
                glfwSetCursorPos(window, (_width / 2), (_height / 2));
                _firstClick = false;
            }

            // Stores the coordinates of the cursor
            double mouseX;
            double mouseY;
            // Fetches the coordinates of the cursor
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
            // and then "transforms" them into degrees 
            float rotX = _sens * (float)(mouseY - (_height / 2)) / _height;
            float rotY = _sens * (float)(mouseX - (_width / 2)) / _width;

            // Calculates upcoming vertical change in the Orientation
            glm::vec3 newOrientation = glm::rotate(_orientation, glm::radians(-rotX), glm::normalize(glm::cross(_orientation, _up)));

            // Decides whether or not the next vertical Orientation is legal or not
            if (abs(glm::angle(newOrientation, _up) - glm::radians(90.0f)) <= glm::radians(85.0f))
            {
                _orientation = newOrientation;
            }

            // Rotates the Orientation left and right
            _orientation = glm::rotate(_orientation, glm::radians(-rotY), _up);

            // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
            glfwSetCursorPos(window, (_width / 2), (_height / 2));
        }
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
            // Unhides cursor since camera is not looking around anymore
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // Makes sure the next time the camera looks around it doesn't jump
            _firstClick = true;
        }
    }
};

#endif /* CAMERA */
