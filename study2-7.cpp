//
// Created by gunjun on 2018-08-01.
//

#include <sb7.h>

GLuint compile_shaders(void) {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar *vertex_shader_source[] = {
            "#version 430 core\n"
            "void main(void){\n"
            "   gl_Position = vec4(0, 0, 0.5, 1.0);\n"
            "}"
    };
    static const GLchar *fragment_shader_source[] = {
            "#version 430 core\n"
            "out vec4 color;\n"
            "void main(void){\n"
            "   color = vec4(0, 0.8, 1.0, 1.0);\n"
            "}"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);


    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

class my_application:public sb7::application{
public:
    void startup(){
        rendering_program = compile_shaders();
        glGenVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    void shutdown(){
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
        glDeleteVertexArrays(1, &vertex_array_object);
    }

    void render(double currentTime){
        const GLfloat color[] = {
                (float)sin(currentTime)*0.5f+0.5f,
                (float)cos(currentTime)*0.5f+0.5f,
                0.0f, 1.0f
        };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(rendering_program);
        glPointSize(40.0f);
        glDrawArrays(GL_POINTS, 0, 1);
        glDrawArrays(GL_POINTS, 1, 1);
    }

private:
    GLuint rendering_program;
    GLuint vertex_array_object;
};

DECLARE_MAIN(my_application);