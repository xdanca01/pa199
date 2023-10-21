// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once
#include "iapplication.h"
#include "glad/glad.h"
#include "Camera.hpp"
#include "RenderObject.hpp"

struct Vertex { float x, y, z, u, v; };

#define D2R M_PI/180.0f

class Application : public IApplication {
    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  private:
    Petr_Math::Matrix model;
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint shader_program;
    GLuint vertex_shader_line;
    GLuint fragment_shader_line;
    GLuint shader_program_line;
    GLuint vertex_arrays;
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint texture;
    GLuint Camerabuffer;
    GLuint VBO;
    GLuint VAO;
    std::vector<RenderObject> objects;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    Camera camera;
    Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    /** Destroys the {@link Application} and releases the allocated resources. */
    virtual ~Application();

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

    void prepare_camera();
    Petr_Math::Matrix perspective(double fov, double aspect, double near, double far);

    /** @copydoc IApplication::update */
    void update(float delta) override;

    /** @copydoc IApplication::render */
    void render() override;

    /** @copydoc IApplication::render_ui */
    void render_ui() override;

    /** @copydoc IApplication::on_resize */
    void on_resize(int width, int height) override;

    /** @copydoc IApplication::on_mouse_move */
    void on_mouse_move(double x, double y) override;

    /** @copydoc IApplication::on_mouse_button */
    void on_mouse_button(int button, int action, int mods) override;

    /** @copydoc IApplication::on_key_pressed */
    void on_key_pressed(int key, int scancode, int action, int mods) override;

    void drawLine(Petr_Math::Vector start, Petr_Math::Vector end, Petr_Math::Vector color);

    void drawCircle();

    void createObjects();

    std::vector<Vertex2> verticesCircle(float radius, int vertices, float angle, float y);


    std::vector<Vertex2> verticesGround();
};
