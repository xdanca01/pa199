#pragma once

#include "PetrMath/Vector.hpp"
#include "glad/glad.h"
#include <vector>

struct Vertex2 { float x, y, z, u, v, nx, ny, nz;};
enum renderType
{
    FAN,
    STRIP,
    INDICES
};

class RenderObject {
public:
    //Vec3
    GLuint VBO;
    GLuint VAO;
    //GLuint index_buffer;
    std::vector<Vertex2> vertices;
    //std::vector<int> indices;
    renderType type;
    GLuint Texture;
    RenderObject(std::vector<Vertex2> v, renderType t) : type(t), vertices(v),
        VAO([]() -> GLuint {
            GLuint vertex_arrays;
            glGenVertexArrays(1, &vertex_arrays);
            glBindVertexArray(vertex_arrays);
            return vertex_arrays;
        }())
        ,
        VBO([v]() -> GLuint
            {
                GLuint vertex_buffer;
                glCreateBuffers(1, &vertex_buffer);
                assert(glGetError() == 0U);
                glNamedBufferData(vertex_buffer, v.size() * sizeof(Vertex2), v.data(), GL_STATIC_DRAW);
                assert(glGetError() == 0U);
                return vertex_buffer;
            }())
    {
        //x, y, z
        glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(float) * 8);
        //u, v
        glVertexArrayVertexBuffer(VAO, 1, VBO, sizeof(float) * 3, sizeof(float) * 8);
        //normal x, y, z
        glVertexArrayVertexBuffer(VAO, 2, VBO, sizeof(float) * 5, sizeof(float) * 8);
        assert(glGetError() == 0U);
        glEnableVertexArrayAttrib(VAO, 0);
        glEnableVertexArrayAttrib(VAO, 1);
        glEnableVertexArrayAttrib(VAO, 2);
        assert(glGetError() == 0U);
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(VAO, 2, 3, GL_FLOAT, GL_FALSE, 0);
        assert(glGetError() == 0U);
        glVertexArrayAttribBinding(VAO, 0, 0);
        glVertexArrayAttribBinding(VAO, 1, 1);
        glVertexArrayAttribBinding(VAO, 2, 2);
        assert(glGetError() == 0U);
    };

    void Render(GLuint shader_program, Petr_Math::Vector lightColor, Petr_Math::Vector lightPosition, Petr_Math::Vector cameraPosition, GLuint texture = 0)
    {
        int booleanLocation = glGetUniformLocation(shader_program, "useTexture");
        glUniform1i(booleanLocation, 0);
        if (texture != 0) {
            glUniform1i(booleanLocation, 1);
            glActiveTexture(GL_TEXTURE0);
            assert(glGetError() == 0U);
            glBindTexture(GL_TEXTURE_2D, texture);
            assert(glGetError() == 0U);
        }

        int lightColorLoc = glGetUniformLocation(shader_program, "lightColor");
        glUniform3fv(lightColorLoc, 1, lightColor.getData());

        int lightPosLoc = glGetUniformLocation(shader_program, "lightPosition");
        glUniform3fv(lightPosLoc, 1, lightPosition.getData());

        int cameraPosLoc = glGetUniformLocation(shader_program, "cameraPosition");
        glUniform3fv(cameraPosLoc, 1, cameraPosition.getData());

        glBindVertexArray(VAO);

        assert(glGetError() == 0U);
        if (type == INDICES)
        {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            assert(glGetError() == 0U);
        }
        else if (type == FAN)
        {
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
            assert(glGetError() == 0U);
        }
        else if (type == STRIP)
        {
            glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
            assert(glGetError() == 0U);
        }
    };

    void deleteBuffers()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};