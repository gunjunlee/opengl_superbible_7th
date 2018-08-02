//
// Created by gunjun on 2018-08-01.
//

#include <sb7.h>

GLuint compile_shaders(void) {

    static const GLchar *vertex_shader_source[] = {
            "#version 430 core\n"
            "layout (location=0) in vec4 offset;\n"
            "layout (location=1) in vec4 color;\n"
            "out VS_OUT{"
            "   vec4 color;"
            "} vs_out;"
            "void main(void){\n"
            "   const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),\n"
            "                                    vec4(-0.25, -0.25, 0.5, 1.0),\n"
            "                                    vec4(0.25, 0.25, 0.5, 1.0));\n"
            "   gl_Position = vertices[gl_VertexID] + offset;"
            "   vs_out.color = color;"
            "}"
    };

    static const GLchar *fs_source[] = {
            "#version 430 core\n"
            "in vec4 vs_color;"
            "in VS_OUT{"
            "   vec4 color;"
            "} fs_in;"
            "out vec4 color;\n"
            "void main(void){\n"
            "   color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,\n"
            "                cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,\n"
            "                sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),\n"
            "                1.0f);\n"
            "}"
    };

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fs_source, NULL);
    glCompileShader(fragment_shader);


    GLuint program = glCreateProgram();
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

        glPointSize(4.0f);
        GLfloat attrib[]={
                (float)sin(currentTime)*0.5f,
                (float)cos(currentTime)*0.5f,
                0.0f, 0.0f
        };
        GLfloat attrib_color[]={
                (float)sin(currentTime)*0.5f,
                (float)cos(currentTime)*0.5f,
                0.0f, 0.0f
        };
        glVertexAttrib4fv(0, attrib);
        glVertexAttrib4fv(1, attrib_color);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    GLuint rendering_program;
    GLuint vertex_array_object;
};

DECLARE_MAIN(my_application);