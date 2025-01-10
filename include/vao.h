#ifndef VAO
#define VAO

#include <glad/glad.h>
#include "vbo.h"

class vao{
    public:
    GLuint _ID;
    vao(){
        glGenVertexArrays(1, &_ID);
    }

    void linkAttr(vbo& vbo, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset){
        vbo.bind();
        glVertexAttribPointer(layout, num, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        vbo.unbind();
    }

    void bind(){
        glBindVertexArray(_ID);
    }

    void unbind(){
        glBindVertexArray(0);
    }

    void del(){
        glDeleteVertexArrays(1, &_ID);
    }
};

#endif /* VAO */
