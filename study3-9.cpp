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

    static const char *tcs_source[] ={
            "#version 430 core                                                                 \n"
            "                                                                                  \n"
            "layout (vertices = 3) out;                                                        \n"
            "                                                                                  \n"
            "void main(void)                                                                   \n"
            "{                                                                                 \n"
            "    if (gl_InvocationID == 0)                                                     \n"
            "    {                                                                             \n"
            "        gl_TessLevelInner[0] = 5.0;                                               \n"
            "        gl_TessLevelOuter[0] = 5.0;                                               \n"
            "        gl_TessLevelOuter[1] = 5.0;                                               \n"
            "        gl_TessLevelOuter[2] = 5.0;                                               \n"
            "    }                                                                             \n"
            "    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;     \n"
            "}                                                                                 \n"
    };

    static const char *tes_source[] ={
            "#version 430 core                                                                 \n"
            "                                                                                  \n"
            "layout (triangles, equal_spacing, cw) in;                                         \n"
            "                                                                                  \n"
            "void main(void)                                                                   \n"
            "{                                                                                 \n"
            "    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"
            "                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
            "                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
            "}                                                                                 \n"
    };

    static const char * gs_source[] =
            {
                    "#version 410 core                                                                  \n"
                    "                                                                                   \n"
                    "layout (triangles) in;                                                             \n"
                    "layout (points, max_vertices = 3) out;                                             \n"
                    "                                                                                   \n"
                    "void main(void)                                                                    \n"
                    "{                                                                                  \n"
                    "    int i;                                                                         \n"
                    "                                                                                   \n"
                    "    for (i = 0; i < gl_in.length(); i++)                                           \n"
                    "    {                                                                              \n"
                    "        gl_Position = gl_in[i].gl_Position;                                        \n"
                    "        EmitVertex();                                                              \n"
                    "    }                                                                              \n"
                    "}                                                                                  \n"
            };

    static const GLchar *fs_source[] = {
            "#version 430 core\n"
            "in vec4 vs_color;"
            "in VS_OUT{"
            "   vec4 color;"
            "} fs_in;"
            "out vec4 color;\n"
            "void main(void){\n"
            "   color = fs_in.color;\n"
            "}"
    };

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fs_source, NULL);
    glCompileShader(fragment_shader);

    GLuint tcs_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tcs_shader, 1, tcs_source, NULL);
    glCompileShader(tcs_shader);

    GLuint tes_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tes_shader, 1, tes_source, NULL);
    glCompileShader(tes_shader);

    GLuint g_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(g_shader, 1, gs_source, NULL);
    glCompileShader(g_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glAttachShader(program, tcs_shader);
    glAttachShader(program, tes_shader);
    glAttachShader(program, g_shader);

    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(tcs_shader);
    glDeleteShader(tes_shader);
    glDeleteShader(g_shader);

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

        glDrawArrays(GL_PATCHES, 0, 3);
    }

private:
    GLuint rendering_program;
    GLuint vertex_array_object;
};

DECLARE_MAIN(my_application);