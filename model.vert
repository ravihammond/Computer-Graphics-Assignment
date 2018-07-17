
#version 330

// Transformation uniform matrices.
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

// Vertex, normal, and texture coordinates.
layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;
layout (location = 3) in vec2 a_tex_coord;

// Vertex to be interpolated.
out vec4 vertex;
// Normals to be interpolated.
out vec3 normal;
// texture coordinate to be interpolated
out vec2 st;

void main(void) {
    // Location of vertex in world space to be passed to frag shader.
    vertex = view_matrix * model_matrix * vec4(a_vertex, 1.0);
    // Normal in world space to be passed to frag shader.
    normal = normalize(mat3(view_matrix * model_matrix) * a_normal);
    // Texture coordinate to be passed to frg shader.
    st = a_tex_coord;

    // Muliplies vertex by all matrices and passes to frag shader.
    gl_Position = projection_matrix * vertex;
}


