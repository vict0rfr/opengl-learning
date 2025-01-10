#ifndef VBO
#define VBO

#include <glad/glad.h>


class vbo{
    public:
        GLuint _ID;
        vbo(GLfloat* vertices, GLsizeiptr size){
            glGenBuffers(1, &_ID);
            glBindBuffer(GL_ARRAY_BUFFER, _ID);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // make dynamic for performance
        }

        void bind(){
            glBindBuffer(GL_ARRAY_BUFFER, _ID);
        }

        void unbind(){
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void del(){
            glDeleteBuffers(1, &_ID);
        }
};

#endif /* VBO */
