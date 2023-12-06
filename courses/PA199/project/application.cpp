// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "application.hpp"
#include "glad/glad.h"
#include "lodepng.h"
#include <filesystem>
#include <iostream>

#define DEBUG false
#define ballRADIUS 0.01f
#define ballX 0.2f
#define ballZ 0.0005f
#define radiusPaddle 0.295f
#define widthPaddle 0.01f
#define anglePaddle 30.0f
#define brickWidth 0.01f
#define numOfBricks 6
#define radiusBrick 0.04f
#define radiusGround 0.3f
#define ballSpeed 0.25f
#define paddlesSpeed 75.0f
#define FOV 25.0f
#define brickHeight 0.03f
#define gravitySpeed 0.06f
#define maxTimeDelta 0.05f

static GLuint load_shader(std::filesystem::path const& path, GLenum const shader_type)
{
    std::filesystem::path const current = std::filesystem::current_path();
    GLuint const shader = glCreateShader(shader_type);
    assert(glGetError() == 0U && shader != 0);
    std::ifstream ifs(path);
    assert(ifs.is_open());
    std::string const str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    char const* code = str.c_str();
    glShaderSource(shader, 1, &code, nullptr);
    assert(glGetError() == 0U);
    glCompileShader(shader);
    assert(glGetError() == 0U);
    return shader;
}

static GLuint load_texture(std::filesystem::path const& path)
{
    std::vector<unsigned char> texels;
    unsigned int width, height;
    unsigned int error_code = lodepng::decode(texels, width, height, path.string(), LCT_RGBA);
    assert(error_code == 0);

    //flip the image vertically
    for (unsigned int lo = 0, hi = height - 1; lo < hi; ++lo, --hi)
        for (unsigned int*  lo_ptr = (unsigned int*)texels.data() + lo * width,
                         *  lo_end = lo_ptr + width,
                         *  hi_ptr = (unsigned int*)texels.data() + hi * width;
                lo_ptr != lo_end; ++lo_ptr, ++hi_ptr)
            std::swap(*lo_ptr, *hi_ptr);

    GLuint texture;
	glGenTextures(1, &texture);
    assert(glGetError() == 0U);
	glBindTexture(GL_TEXTURE_2D, texture);
    assert(glGetError() == 0U);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)texels.data());
    assert(glGetError() == 0U);
    glGenerateMipmap(GL_TEXTURE_2D);
    assert(glGetError() == 0U);
    return texture;
}

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : IApplication(initial_width, initial_height, arguments)
    ,rotatePaddles(0)
    , model(4, 1.0f, true)
    , vertex_shader(load_shader(lecture_folder_path / "data" / "shaders" / "sample.vert", GL_VERTEX_SHADER))
    , fragment_shader(load_shader(lecture_folder_path / "data" / "shaders" / "sample.frag", GL_FRAGMENT_SHADER))
    , vertex_shader_line(load_shader(lecture_folder_path / "data" / "shaders" / "line.vert", GL_VERTEX_SHADER))
    , fragment_shader_line(load_shader(lecture_folder_path / "data" / "shaders" / "line.frag", GL_FRAGMENT_SHADER))
    , shader_program([](GLuint const  vertex_shader, GLuint const  fragment_shader) -> GLuint {
            
            GLuint const  shader_program = glCreateProgram();
            assert(glGetError() == 0U && shader_program != 0);
            glAttachShader(shader_program,vertex_shader);
            assert(glGetError() == 0U);
            glAttachShader(shader_program,fragment_shader);
            assert(glGetError() == 0U);
            glLinkProgram(shader_program);
            assert(glGetError() == 0U);
            glDetachShader(shader_program, vertex_shader);
            assert(glGetError() == 0U);
            glDetachShader(shader_program, fragment_shader);
            assert(glGetError() == 0U);
            return shader_program;
            }(vertex_shader, fragment_shader))
    , vertex_arrays([]() -> GLuint {
            GLuint vertex_arrays;
            glGenVertexArrays(1, &vertex_arrays);
            glBindVertexArray(vertex_arrays);
            return vertex_arrays;
            }())
    , vertex_buffer([]() -> GLuint {
            struct Vertex { float x,y,z,u,v; };
            std::vector<Vertex> const vertices {
                { -0.5f, -0.125f, 0.0f,       0.0f, 0.0f },
                {  0.5f, -0.125f, 0.0f,       1.0f, 0.0f },
                {  0.5f,  0.125f, 0.0f,       1.0f, 1.0f },
                { -0.5f,  0.125f, 0.0f,       0.0f, 1.0f },
            };
            GLuint vertex_buffer;
            glGenBuffers(1, &vertex_buffer);
            assert(glGetError() == 0U);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            assert(glGetError() == 0U);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
            assert(glGetError() == 0U);
            glEnableVertexAttribArray(0);
            assert(glGetError() == 0U);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
            assert(glGetError() == 0U);
            glEnableVertexAttribArray(1);
            assert(glGetError() == 0U);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
            assert(glGetError() == 0U);
            return vertex_buffer;
            }())
    , index_buffer([]() -> GLuint {
            std::vector<GLuint> const indices { 0U, 1U, 2U,     0U, 2U, 3U };
            GLuint index_buffer;
            glGenBuffers(1, &index_buffer);
            assert(glGetError() == 0U);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
            assert(glGetError() == 0U);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
            assert(glGetError() == 0U);
            return index_buffer;
            }())
    , texture(load_texture(lecture_folder_path / "data" / "textures" / "you_win.png")),
      Groundtexture(load_texture(lecture_folder_path / "data" / "textures" / "ground.png")),

    shader_program_line([](GLuint const  vertex_shader, GLuint const  fragment_shader) -> GLuint {
                GLuint const  shader_program = glCreateProgram();
                assert(glGetError() == 0U && shader_program != 0);
                glAttachShader(shader_program, vertex_shader);
                assert(glGetError() == 0U);
                glAttachShader(shader_program, fragment_shader);
                assert(glGetError() == 0U);
                glLinkProgram(shader_program);
                assert(glGetError() == 0U);
                glDetachShader(shader_program, vertex_shader);
                assert(glGetError() == 0U);
                glDetachShader(shader_program, fragment_shader);
                assert(glGetError() == 0U);
                return shader_program;
        }(vertex_shader_line, fragment_shader_line))
        , VAO([]() -> GLuint {
            GLuint vertex_arrays;
            glGenVertexArrays(1, &vertex_arrays);
            glBindVertexArray(vertex_arrays);
            return vertex_arrays;
        }())
        , VBO([]() -> GLuint {
                struct Vertex { float x, y, z; };
                std::vector<Vertex> const vertices{
                    { 0.0f, 0.0f, 0.0f, },
                    { 1.0f, 0.0f, 0.0f, },
                };
                GLuint vertex_buffer;
                glGenBuffers(1, &vertex_buffer);
                assert(glGetError() == 0U);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
                assert(glGetError() == 0U);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
                assert(glGetError() == 0U);
                glEnableVertexAttribArray(0);
                assert(glGetError() == 0U);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
                assert(glGetError() == 0U);
                return vertex_buffer;
            }()),
            lightColor(1.0f, 1.0f, 1.0f),
            lightPosition(0.0, 1.0f, 0.0f)
{
    lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    deltaTime = 0.0f;
    createObjects();
    prepare_physics();
    prepare_camera();
    glViewport(0, 0, width, height);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
}

Application::~Application()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program_line);
    glDeleteShader(fragment_shader_line);
    glDeleteShader(vertex_shader_line);
    glDeleteVertexArrays(1U, &vertex_arrays);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &Groundtexture);
    glDeleteProgram(shader_program);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    for (auto obj : objects)
    {
        obj.deleteBuffers();
    }
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

Petr_Math::Matrix Application::perspective(double fov, double aspect, double near, double far)
{
    fov = D2R * fov;
    /*float scale = 1.0f / tan(fov * 0.5);
    float r = aspect * scale;
    float l = -r;
    float t = scale;
    float b = -t;
    float dataForMat[16] =
    {
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, -(far * near) / (far - near), -1.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };
    {
        2.0f * near / (r - l), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f * near / (t - b), 0.0f, 0.0f,
        (r + l) / (r - l), (t + b) / (t - b), -(far + near) / (far - near), -1.0f,
        0.0f, 0.0f, -2.0f * far * near / (far - near), 0.0f
    };*/
    float tanHalfFovy = tan(fov / 2.0f);
    float yScale = 1.0f / tanHalfFovy;
    float xScale = 1.0f / (aspect * tanHalfFovy);
    float dist = far - near;
    float dataForMat[16] =
    {
        xScale, 0.0f, 0.0f, 0.0f,
        0.0f, yScale, 0.0f, 0.0f,
        0.0f, 0.0f, -far/dist, -1.0f,
        0.0f, 0.0f, -(far * near) / dist, 0.0f
    };
    return Petr_Math::Matrix(4, 4, dataForMat);
}

void Application::prepare_lights()
{
}

void Application::prepare_physics()
{
    auto positionBall = Petr_Math::Vector(ballX, ballZ, ballRADIUS);
    Petr_Math::PolarCoordinates paddle1(radiusPaddle, 0.0f);
    Petr_Math::PolarCoordinates paddle2(radiusPaddle, 120.0f);
    Petr_Math::PolarCoordinates paddle3(radiusPaddle, 240.0f);
    std::vector<Petr_Math::PolarCoordinates> paddles, bricks;
    paddles.push_back(paddle1);
    paddles.push_back(paddle2);
    paddles.push_back(paddle3);
    for(int i = 0; i < this->bricks.size(); ++i)
    {
        if (this->bricks[i].height < ballRADIUS * 2.0f)
        {
            bricks.push_back(this->bricks[i].polarCoords);
        }
        
    }
    float angleWidthBrick = 360.0f / (float)numOfBricks;
    gamePhysics = Physics(positionBall, paddles, widthPaddle / 2.0f, anglePaddle / 2.0f, brickWidth / 2.0f, angleWidthBrick / 2.0f, radiusGround, ballSpeed);
}

void Application::SetViewTop()
{
    camera.eye_position = Petr_Math::Vector(0.0f, 1.0f, 0.0f);
    camera.set_view_matrix(camera.eye_position, Petr_Math::Vector(3, 0.0f), Petr_Math::Vector(0.0f, 0.0f, -1.0f));
    camera.projection_matrix = orthographic(-1.0f * (float)width/height, 1.0f * (float)width / height, -1.0f, 1.0f, 0.1f, 100.0f, 2.0f);
}

void Application::SetViewSide()
{
    camera.eye_position = Petr_Math::Vector(0.0f, 1.0f, 1.0f);
    camera.set_view_matrix(camera.eye_position, Petr_Math::Vector(3, 0.0f), Petr_Math::Vector(0.0f, 1.0f, -1.0f));
    camera.projection_matrix = perspective(FOV, (float)width/height, 0.1f, 100.0f);
}

void Application::prepare_camera()
{
    SetViewSide();
    top = false;
}

void Application::update(float delta) {}

void Application::render() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    deltaTime = (float)(now - lastTime).count()/1000.0f;
    deltaTime = fminf(deltaTime, maxTimeDelta);
    lastTime = now;
    // Sets the clear color.
    glClearColor(red, green, blue, 1.0f);
    
    // Clears the window using the above color.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (DEBUG)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }    
    /*glUseProgram(shader_program);
    assert(glGetError() == 0U);
    glBindVertexArray(vertex_arrays);
    assert(glGetError() == 0U);
    glActiveTexture(GL_TEXTURE0);
    assert(glGetError() == 0U);
    glBindTexture(GL_TEXTURE_2D, texture);
    assert(glGetError() == 0U);*/
    /*glBindVertexArray(VAO);
    assert(glGetError() == 0U);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    assert(glGetError() == 0U);

    float red[3] = { 1.0f, 0.0f, 0.0f };
    glUniform3fv(glGetUniformLocation(shader_program_line, "color"), 1, red);


    int modelLoc = glGetUniformLocation(shader_program_line, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.getData());
    assert(glGetError() == 0U);
    int viewLoc = glGetUniformLocation(shader_program_line, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera.get_view_matrix().getData());
    assert(glGetError() == 0U);
    int projectionLoc = glGetUniformLocation(shader_program_line, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera.projection_matrix.getData());
    assert(glGetError() == 0U);

    glDrawArrays(GL_LINES, 0, 2);
    assert(glGetError() == 0U);

    */
    auto Vp = Petr_Math::Vector(0.0f, 0.0f, 0.0f) * ballSpeed;
    RotatePaddles(paddlesSpeed * rotatePaddles * deltaTime);
    std::vector<Petr_Math::PolarCoordinates> bricks;
    for (int i = 0; i < this->bricks.size(); ++i)
    {
        if (this->bricks[i].height < ballRADIUS * 2.0f)
        {
            bricks.push_back(this->bricks[i].polarCoords);
        }
    }
    if (this->bricks.size() > 0)
    {
        bool collided = false;
        auto moveVector = gamePhysics.moveBall(paddlesSpeed, rotatePaddles, deltaTime, bricks, collided);
        if (collided)
        {
            ResetBall();
        }
        bool removed = false;
        std::vector<int> removeIndexes;
        for (int i = 0; i < this->bricks.size(); ++i)
        {
            if (this->bricks[i].polarCoords == gamePhysics.lastHit)
            {
                if (this->bricks[i].height < ballRADIUS * 2.0f)
                {
                    this->bricks[i].hit();
                    if (this->bricks[i].active == false)
                    {
                        removed = true;
                        removeIndexes.push_back(i);
                        continue;
                    }
                }
                //Move other bricks down
                else if (removed == true)
                {
                    this->bricks[i].wantedHeight -= brickHeight;
                }
            }
            this->bricks[i].moveOnHeight(deltaTime, gravitySpeed);
        }
        for (int i = 0; i < removeIndexes.size(); ++i)
        {
            this->bricks.erase(this->bricks.begin() + removeIndexes[i]);
        }

        Petr_Math::Matrix newModel(4, 1.0f, true);
        //Model matrix for ball
        newModel.translate(moveVector * deltaTime);
        objects[4].model = objects[4].model * newModel;
    }
    drawObjects();
    Petr_Math::Vector start2(0.0f, 0.0f, 5.0f, 1.0f);
    Petr_Math::Vector start(0.0f, 0.0f, 0.0f);
    Petr_Math::Vector endX(1.0f, 0.0f, 0.0f);
    Petr_Math::Vector endY(0.0f, 1.0f, 0.0f);
    Petr_Math::Vector endZ(0.0f, 0.0f, 1.0f);
    drawLine(start, endX, endX);
    drawLine(start, endY, endY);
    drawLine(start, endZ, endZ);

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    //assert(glGetError() == 0U);
    
}

void Application::render_ui() {}

void Application::RotatePaddles(float angle)
{
    Petr_Math::Matrix newModel(4, 1.0f, true);
    newModel.rotateY(angle);
    //RenderObjects are at 1-3
    for (int i = 1; i <= 3; ++i)
    {
        objects[i].model = objects[i].model * newModel;
        //modify angle of Paddles in physics
        gamePhysics.positionsP[i - 1].angle += angle;
        gamePhysics.positionsP[i - 1].Normalize();
    }
}

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    IApplication::on_resize(width, height);
    // Changes the viewport.
    glViewport(0, 0, width, height);
    if (top)
    {
        SetViewTop();
    }
    else
    {
        SetViewSide();
    }
}

void Application::on_mouse_move(double x, double y) {}

Petr_Math::Matrix Application::orthographic(double left, double right, double bottom, double top, double near, double far, float scale)
{
    float sc = (float)width / height;
    float r = sc;
    float l = 0.0f;
    float t = 1.0f;
    float b = 0.0f;
    float mid_x = (l + r) / 2;
    float mid_y = (b + t) / 2;
    float mid_z = (-near + -far) / 2;
    float dataForMat[16] =
    {
        scale * 2.0f / (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, scale * 2.0f / (top - bottom), 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / (far - near), 0.0f,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f
    };
    /*float dataForMat[16] =
    {
        2.0f / (r - l), 0.0f, 0.0f, -(r + l) / (r - l),
        0.0f, scale * 2.0f / (t - b), 0.0f, -(t + b) / (t - b),
        0.0f, 0.0f,  scale * 2.0f / (far - near), -(far + near) / (far - near),
        0.0f, 0.0f, 0.0f, 1.0f
    };*/
    return Petr_Math::Matrix(4, 4, dataForMat);
}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        red = 0;
        green = 0;
        blue = 0;
        switch (key) {
        case GLFW_KEY_R:
            red = 1;
            break;
        case GLFW_KEY_G:
            green = 1;
            break;
        case GLFW_KEY_B:
            blue = 1;
            break;
        case GLFW_KEY_LEFT:
            rotatePaddles = 1;
            break;
        case GLFW_KEY_RIGHT:
            rotatePaddles = -1;
            break;
        case GLFW_KEY_1:
            SetViewSide();
            top = false;
            break;
        case GLFW_KEY_2:
            top = true;
            SetViewTop();
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key) {
        case GLFW_KEY_LEFT:
            if (rotatePaddles == 1) {
                rotatePaddles = 0;
            }
            break;
        case GLFW_KEY_RIGHT:
            if (rotatePaddles == -1) {
                rotatePaddles = 0;
            }
            break;
        }
    }
}

void Application::drawLine(Petr_Math::Vector start, Petr_Math::Vector end, Petr_Math::Vector color)
{
    glUseProgram(shader_program_line);
    assert(glGetError() == 0U);
    struct Vertex { float x, y, z; };
    std::vector<Vertex> const vertices{
        { start[0], start[1], start[2], },
        { end[0], end[1], end[2] },
    };
    GLuint vertex_buffer2;
    glGenBuffers(1, &vertex_buffer2);
    assert(glGetError() == 0U);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer2);
    assert(glGetError() == 0U);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
    assert(glGetError() == 0U);
    glEnableVertexAttribArray(0);
    assert(glGetError() == 0U);

    glBindVertexArray(VAO);
    assert(glGetError() == 0U);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    assert(glGetError() == 0U);

    

    glUniform3fv(glGetUniformLocation(shader_program_line, "color"), 1, color.getData());

    int modelLoc = glGetUniformLocation(shader_program_line, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.getData());
    assert(glGetError() == 0U);
    int viewLoc = glGetUniformLocation(shader_program_line, "view");
    auto viewMatrix = camera.get_view_matrix().transpose();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix.getData());
    assert(glGetError() == 0U);
    int projectionLoc = glGetUniformLocation(shader_program_line, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera.projection_matrix.getData());
    assert(glGetError() == 0U);

    glDrawArrays(GL_LINES, 0, 2);
    assert(glGetError() == 0U);

    glDeleteBuffers(1, &vertex_buffer2);
}

void Application::drawObjects()
{
    glUseProgram(shader_program);
    assert(glGetError() == 0U);
    int viewLoc = glGetUniformLocation(shader_program, "view");
    auto viewMatrix = camera.get_view_matrix().transpose();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix.getData());
    assert(glGetError() == 0U);
    int projectionLoc = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera.projection_matrix.getData());
    assert(glGetError() == 0U);
    bool ground = true;
    for (auto obj : objects)
    {
        if (ground)
        {
            ground = false;
            obj.Render(shader_program, lightColor, lightPosition, camera.eye_position, Groundtexture);
        }
        else
        {
            obj.Render(shader_program, lightColor, lightPosition, camera.eye_position);
        }
    }
    for (auto obj : bricks)
    {
        if (obj.active == true)
        {
            obj.render.Render(shader_program, lightColor, lightPosition, camera.eye_position);
        }
    }
    
    //objects[1].Render();
}

void Application::createObjects()
{
    //Create ground
    auto vertices = verticesGround();
    Petr_Math::Vector white(1.0f, 1.0f, 1.0f);
    Petr_Math::Vector green(0.0f, 1.0f, 0.0f);
    Petr_Math::Vector red(1.0f, 0.0f, 0.0f);
    Petr_Math::Vector blue(0.0f, 0.0f, 1.0f);
    auto pink = blue + red;
    auto yellow = red + green;

    RenderObject ground(vertices, FAN, white * 0.3f, green, white * 0.4f);
    objects.push_back(ground);
    //Create paddle 1 
    vertices = VerticesPaddle(15, radiusPaddle, 0.0f, 0.01f, widthPaddle, anglePaddle, 0.0f);
    RenderObject paddle1(vertices, INDICES, red + white * 0.3f, red, red + white * 0.4f);
    objects.push_back(paddle1);
    //Create paddle 2
    vertices = VerticesPaddle(15, radiusPaddle, 0.0f, 0.01f, widthPaddle, anglePaddle, 120.0f);
    RenderObject paddle2(vertices, INDICES, red + white * 0.3f, red, red + white * 0.4f);
    objects.push_back(paddle2);
    //Create paddle 3
    vertices = VerticesPaddle(15, radiusPaddle, 0.0f, 0.01f, widthPaddle, anglePaddle, 240.0f);
    RenderObject paddle3(vertices, INDICES, red + white * 0.3f, red, red + white * 0.4f);
    objects.push_back(paddle3);

    vertices = verticesBall(ballRADIUS, 16, 32, 0.0f, ballX, ballZ);
    RenderObject ball(vertices, INDICES, blue * 0.1f, blue * 0.75f, red * 0.4f);
    objects.push_back(ball);

    float step = 360.0f / (float)numOfBricks;
    int numOfRows = 3;
    for (int row = 0; row < numOfRows; ++row)
    {
        for (int i = 0; i < numOfBricks; ++i)
        {
            Petr_Math::PolarCoordinates PC(radiusBrick, i * step + 240.0f + step / 2.0f);
            auto color = (i + row) % 2 == 0 ? pink : yellow;
            vertices = VerticesBrick(15, radiusBrick, 0.0f + brickHeight * row, brickHeight, brickWidth, step, i * step + 240.0f);
            RenderObject brick(vertices, INDICES, color * 0.8f, color, color * 0.4f);
            bricks.push_back(Brick(brick, PC, row * brickHeight));
        }
        
    }
;}

std::vector<Vertex2> Application::verticesCircle(float radius, int verts, float angle, float y, float angleOffset)
{
    std::vector<Vertex2> vertices;
    float x, z, u, v;
    u = 1.f;
    v = 1.f;
    float xMin, xMax, zMin, zMax;
    Vertex2 newVert;
    //To rad
    angle = angle * D2R;
    float step = angle / ((float)verts - 1.0f);
    //MOVE angle by offset
    float offset = angleOffset * D2R;
    for (int i = 0; i < verts; ++i)
    {
        //MOVE angle by offset
        x = cos(step * i + offset) * radius;
        z = sin(step * i + offset) * radius;
        newVert = {x, y, z, u, v};
        vertices.push_back(newVert);
        if (i == 0)
        {
            xMin = x;
            xMax = x;
            zMin = z;
            zMax = z;
        }
        else
        {
            xMin = fminf(x, xMin);
            zMin = fminf(x, zMin);
            xMax = fmaxf(x, xMax);
            zMax = fmaxf(x, zMax);
        }
    }
    for (int i = 0; i < vertices.size(); ++i)
    {
        vertices[i].u = (vertices[i].x - xMin) / (xMax - xMin);
        vertices[i].v = (vertices[i].z - zMin) / (zMax - zMin);
    }
    return vertices;
}

void Application::ResetBall()
{
    objects[4].model = Petr_Math::Matrix(4, 1.0f, true);
    auto positionBall = Petr_Math::Vector(ballX, ballZ, ballRADIUS);
    gamePhysics.ResetBall(positionBall);
}

std::vector<Vertex2> Application::verticesGround()
{
    std::vector<Vertex2> vertices;
    Vertex2 middle = { 0.0f, 0.0f, 0.0f, 0.5f, 0.5f };
    vertices.push_back(middle);
    auto anotherVerts = verticesCircle(radiusGround, 150, 360, 0.0f);
    vertices.insert(vertices.end(), anotherVerts.rbegin(), anotherVerts.rend());
    Petr_Math::Vector normalUp(0.0f, 1.0f, 0.0f);
    SetNormalForEachVertex(vertices, normalUp, vertices.size());
    return vertices;
}
std::vector<Vertex2> Application::VerticesBrick(int points, float R, float yBottom, float height, float width, float angle, float offset)
{
    std::vector<Vertex2> vertices;
    std::vector<Vertex2> verticesTop;
    std::vector<Vertex2> verticesBottom;
    auto verticesTop1 = verticesCircle(R - width/2, points, angle, yBottom + height, offset);
    auto verticesTop2 = verticesCircle(R + width / 2, points, angle, yBottom + height, offset);
    auto verticesBottom1 = verticesCircle(R - width / 2, points, angle, yBottom, offset);
    auto verticesBottom2 = verticesCircle(R + width / 2, points, angle, yBottom, offset);

    Petr_Math::Vector normalUp(0.0f, 1.0f, 0.0f);
    Petr_Math::Vector normalDown(0.0f, -1.0f, 0.0f);
    Vertex2 center = { 0.0f, 0.0f, 0.0f };
    int cnt = 0;
    //NORMAL is anticlockwise up!!!!
    //Counterclockwise is frontface

    //SideOutside
    VerticesToTriangles(verticesBottom2, verticesTop2, vertices, true);
    cnt = vertices.size();
    CalcNormalsToCenter(vertices, center, true, cnt);

    //SideInside
    VerticesToTriangles(verticesBottom1, verticesTop1, vertices, false);
    //Count added vertices
    cnt = vertices.size() - cnt;
    //Calc normals for added vertices
    CalcNormalsToCenter(vertices, center, false, cnt);
    cnt = vertices.size();

    //Top
    VerticesToTriangles(verticesTop1, verticesTop2, vertices, false);
    cnt = vertices.size() - cnt;
    SetNormalForEachVertex(vertices, normalUp, cnt);
    cnt = vertices.size();

    //Bottom
    VerticesToTriangles(verticesBottom1, verticesBottom2, vertices, true);
    cnt = vertices.size() - cnt;
    SetNormalForEachVertex(vertices, normalDown, cnt);

    //SideLeft
    vertices.push_back(verticesBottom1[verticesBottom1.size() - 1]);
    vertices.push_back(verticesBottom2[verticesBottom2.size() - 1]);
    vertices.push_back(verticesTop1[verticesTop1.size() - 1]);
    vertices.push_back(verticesTop1[verticesTop1.size() - 1]);
    vertices.push_back(verticesBottom2[verticesBottom2.size() - 1]);
    vertices.push_back(verticesTop2[verticesTop2.size() - 1]);
    auto v1 = verticesBottom1[verticesBottom1.size() - 1];
    auto v2 = verticesBottom2[verticesBottom2.size() - 1];
    auto v3 = verticesTop1[verticesTop1.size() - 1];
    Petr_Math::Vector Vec1(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    Petr_Math::Vector Vec2(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
    auto normalLeft = Vec2.cross(Vec1).normalize();
    SetNormalForEachVertex(vertices, normalLeft, 6);

    //SideRight
    vertices.push_back(verticesBottom2[0]);
    vertices.push_back(verticesBottom1[0]);
    vertices.push_back(verticesTop2[0]);
    vertices.push_back(verticesTop2[0]);
    vertices.push_back(verticesBottom1[0]);
    vertices.push_back(verticesTop1[0]);
    v1 = verticesBottom2[0];
    v2 = verticesBottom1[0];
    v3 = verticesTop2[0];
    Vec1 = Petr_Math::Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    Vec2 = Petr_Math::Vector(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
    auto normalRight = Vec2.cross(Vec1).normalize();
    SetNormalForEachVertex(vertices, normalRight, 6);

    return vertices;
}


std::vector<Vertex2> Application::VerticesPaddle(int points, float R, float yBottom, float height, float width, float angle, float angleOffset)
{
    std::vector<Vertex2> vertices;
    std::vector<Vertex2> verticesTop;
    std::vector<Vertex2> verticesBottom;
    auto verticesTop1 = verticesCircle(R - width / 2, points, angle, yBottom + height, angleOffset - angle/2);
    auto verticesTop2 = verticesCircle(R + width / 2, points, angle, yBottom + height, angleOffset - angle / 2);
    auto verticesBottom1 = verticesCircle(R - width / 2, points, angle, yBottom, angleOffset - angle / 2);
    auto verticesBottom2 = verticesCircle(R + width / 2, points, angle, yBottom, angleOffset - angle / 2);

    //NORMAL is anticlockwise up!!!!
    //Counterclockwise is frontface

    Petr_Math::Vector normalUp(0.0f, 1.0f, 0.0f);
    Petr_Math::Vector normalDown(0.0f, -1.0f, 0.0f);
    Vertex2 center = { 0.0f, 0.0f, 0.0f };
    int cnt = 0;

    //SideOutside - normals outgoing from center
    VerticesToTriangles(verticesBottom2, verticesTop2, vertices, true);
    cnt = vertices.size();
    CalcNormalsToCenter(vertices, center, true, cnt);

    //SideInside
    VerticesToTriangles(verticesBottom1, verticesTop1, vertices, false);
    //Count added vertices
    cnt = vertices.size() - cnt;
    //Calc normals for added vertices
    CalcNormalsToCenter(vertices, center, false, cnt);
    cnt = vertices.size();

    //Top
    VerticesToTriangles(verticesTop1, verticesTop2, vertices, false);
    cnt = vertices.size() - cnt;
    SetNormalForEachVertex(vertices, normalUp, cnt);
    cnt = vertices.size();

    //Bottom
    VerticesToTriangles(verticesBottom1, verticesBottom2, vertices, true);
    cnt = vertices.size() - cnt;
    SetNormalForEachVertex(vertices, normalDown, cnt);
    cnt = vertices.size();

    //SideLeft
    vertices.push_back(verticesBottom1[verticesBottom1.size() - 1]);
    vertices.push_back(verticesBottom2[verticesBottom2.size() - 1]);
    vertices.push_back(verticesTop1[verticesTop1.size() - 1]);
    vertices.push_back(verticesTop1[verticesTop1.size() - 1]);
    vertices.push_back(verticesBottom2[verticesBottom2.size() - 1]);
    vertices.push_back(verticesTop2[verticesTop2.size() - 1]);
    auto v1 = verticesBottom1[verticesBottom1.size() - 1];
    auto v2 = verticesBottom2[verticesBottom2.size() - 1];
    auto v3 = verticesTop1[verticesTop1.size() - 1];
    Petr_Math::Vector Vec1(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    Petr_Math::Vector Vec2(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
    auto normalLeft = Vec2.cross(Vec1).normalize();
    SetNormalForEachVertex(vertices, normalLeft, 6);

    //SideRight
    vertices.push_back(verticesBottom2[0]);
    vertices.push_back(verticesBottom1[0]);
    vertices.push_back(verticesTop2[0]);
    vertices.push_back(verticesTop2[0]);
    vertices.push_back(verticesBottom1[0]);
    vertices.push_back(verticesTop1[0]);
    v1 = verticesBottom2[0];
    v2 = verticesBottom1[0];
    v3 = verticesTop2[0];
    Vec1 = Petr_Math::Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    Vec2 = Petr_Math::Vector(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
    auto normalRight = Vec2.cross(Vec1).normalize();
    SetNormalForEachVertex(vertices, normalRight, 6);

    return vertices;
}

std::vector<Vertex2> Application::verticesBall(float radius, int rows, int cols, float yBottom, float xCenter, float zCenter)
{
    std::vector<Vertex2> vertices;
    std::vector<std::vector<Vertex2>> rowsOfVertices;
    float angle = 360.0f / cols;
    float angleRow = 180.0f / rows;
    float rowStepY = (radius * 2) / rows;
    float y;
    Vertex2 downPoint = { xCenter, yBottom, zCenter, 0.5f, 0.5f };
    Vertex2 topPoint = { xCenter, yBottom + radius * 2, zCenter, 0.5f, 0.5f };
    std::vector<float> radiuses;
    //Generate radiuses
    for (int row = 1; row < rows; ++row)
    {
        radiuses.push_back(radius * sin(row * D2R * angleRow));
    }
    //first row is point y = yBottom and last row is also point y = (yBottom + radius * 2)
    for (int row = 1; row < rows; ++row)
    {
        y = yBottom + radius - radius * cos(row * angleRow * D2R);
        auto verts = verticesCircle(radiuses[row - 1], cols + 1, 360.0f, y);
        //Move the vertices
        if (xCenter != 0 || zCenter != 0)
        {
            for (int i = 0; i < verts.size(); ++i)
            {
                verts[i].x += xCenter;
                verts[i].z += zCenter;
            }
        }
        rowsOfVertices.push_back(verts);
        if (row == 1)
        {
            VerticesToTriangles(rowsOfVertices[0], downPoint, vertices, false);
        }
        else
        {
            VerticesToTriangles(rowsOfVertices[row - 2], rowsOfVertices[row - 1], vertices, true);
        }
    }
    VerticesToTriangles(rowsOfVertices[rows - 2], topPoint, vertices, true);
    Vertex2 center = { xCenter, yBottom + radius, zCenter };
    CalcNormalsToCenter(vertices, center, true, vertices.size());
    return vertices;
}

void Application::SetNormalForEachVertex(std::vector<Vertex2>& v, Petr_Math::Vector normal, int count)
{
    for (int i = v.size() - count; i < v.size(); ++i)
    {
        v[i].nx = normal[0];
        v[i].ny = normal[1];
        v[i].nz = normal[2];
    }
}

void Application::CalcNormalsToCenter(std::vector<Vertex2>& v, Vertex2 center, bool opposite, int count)
{
    Petr_Math::Vector centerVec(center.x, center.y, center.z);
    if(opposite)
    {
        for (int i = v.size() - count; i < v.size(); ++i)
        {
            Petr_Math::Vector vPosVec(v[i].x, v[i].y, v[i].z);
            auto normal = (vPosVec - centerVec).normalize();
            v[i].nx = normal[0];
            v[i].ny = normal[1];
            v[i].nz = normal[2];
        }
    }
    else
    {
        for (int i = v.size() - count; i < v.size(); ++i)
        {
            Petr_Math::Vector vPosVec(v[i].x, v[i].y, v[i].z);
            auto normal = (centerVec - vPosVec).normalize();
            v[i].nx = normal[0];
            v[i].ny = normal[1];
            v[i].nz = normal[2];
        }
    }
    
}

void Application::VerticesToTriangles(std::vector<Vertex2>& v1, std::vector<Vertex2>& v2, std::vector<Vertex2>& output, bool clockwise)
{
    //Clockwise
    if (clockwise)
    {
        for (int i = 0; i < v1.size() - 1; ++i)
        {
            //first triangle clockwise (opposite normal)
            output.push_back(v1[i]);
            output.push_back(v2[i]);
            output.push_back(v1[i + 1]);
            //second triangle clockwise
            output.push_back(v2[i]);
            output.push_back(v2[i + 1]);
            output.push_back(v1[i + 1]);
        }
    }
    //Anticlockwise
    else
    {
        for (int i = 0; i < v1.size() - 1; ++i)
        {
            //first triangle anticlockwise
            output.push_back(v1[i]);
            output.push_back(v1[i + 1]);
            output.push_back(v2[i]);
            //second triangle anticlockwise
            output.push_back(v2[i]);
            output.push_back(v1[i + 1]);
            output.push_back(v2[i + 1]);
        }
    }
}

void Application::VerticesToTriangles(std::vector<Vertex2>& v1, Vertex2& v2, std::vector<Vertex2>& output, bool clockwise)
{
    //Clockwise
    if (clockwise)
    {
        for (int i = 0; i < v1.size() - 1; ++i)
        {
            //first triangle clockwise (opposite normal)
            output.push_back(v1[i]);
            output.push_back(v2);
            output.push_back(v1[i + 1]);
        }
    }
    //Anticlockwise
    else
    {
        for (int i = 0; i < v1.size() - 1; ++i)
        {
            //first triangle anticlockwise
            output.push_back(v1[i]);
            output.push_back(v1[i + 1]);
            output.push_back(v2);
        }
    }
}
