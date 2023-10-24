#pragma once

#include "PetrMath/Vector.hpp"
#include "glad/glad.h"
#include <vector>

struct Vertex2 { float x, y, z, u, v; };
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
        glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(float) * 5);
        //u, v
        glVertexArrayVertexBuffer(VAO, 1, VBO, sizeof(float) * 3, sizeof(float) * 5);
        assert(glGetError() == 0U);
        glEnableVertexArrayAttrib(VAO, 0);
        glEnableVertexArrayAttrib(VAO, 1);
        assert(glGetError() == 0U);
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 0);
        assert(glGetError() == 0U);
        glVertexArrayAttribBinding(VAO, 0, 0);
        glVertexArrayAttribBinding(VAO, 1, 1);
        assert(glGetError() == 0U);
    };

    void Render()
    {
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