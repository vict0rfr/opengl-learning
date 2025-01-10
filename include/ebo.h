#ifndef EBO
#define EBO

#include <glad/glad.h>

class ebo{
    public:
        GLuint _ID;
        ebo(GLuint* indices, GLsizeiptr size){
            glGenBuffers(1, &_ID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); // make dynamic for performance
        }

        void bind(){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID);
        }

        void unbind(){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void del(){
            glDeleteBuffers(1, &_ID);
        }
};

#endif /* EBO */
