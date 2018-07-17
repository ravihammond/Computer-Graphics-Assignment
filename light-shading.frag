
#version 330

// Vectors interpolated from vert shader.
in vec4 vertex;
in vec3 normal;
in vec2 st;

// Texture to be sampled from.
uniform sampler2D texMap;
// State of shader.
uniform int state;
// Knows if texture exists.
uniform int texture_exists;
// direction of rotating light.
uniform vec4 rotate_pos;

uniform mat4 view;

// All colour values loaded from material.
uniform vec3 mtl_ambient;
uniform vec3 mtl_diffuse;
uniform vec3 mtl_specular;
uniform float shininess;

// State 0 lighting
const vec3 light_ambient_0 = vec3(0.0,0.0,0.0);
const vec3 light_diffuse_0 = vec3(1.0,0.5,0.5);
const vec3 light_specular_0 = vec3(0.5,0.5,1.0);

// State 1 lighting
const vec3 light_ambient_1 = vec3(0.0,0.0,0.0);
const vec3 light_diffuse_1 = vec3(0.5,1.0,0.5);
const vec3 light_specular_1 = vec3(0.5,1.0,0.5);

// State 2 lighting
const vec3 light_ambient_2 = vec3(0.0,0.0,0.0);
const vec3 light_diffuse_2 = vec3(1.0,1.0,0.0);
const vec3 light_specular_2 = vec3(1.0,1.0,0.0);

out vec4 fragColour;

// Regular phong shading.
vec3 blinnPhongPointLight(in vec4 position, in vec3 normDir,
         vec3 light_ambient, vec3 light_diffuse, vec3 light_specular) {
    // create view direction from position.
    vec3 viewDir = normalize(-position.xyz);
    vec3 lightDir;

    // Change light direction depending on state.
    switch (state) {
        case 0:
            // Set light direction to come from directly above.
            lightDir = vec3(0,1,0);
            break;
        case 1:
            // Set light direction to come directly from camera.
            lightDir = normalize(viewDir - vec3(view * vec4(0)));
            break;
        case 2:
            // Set light direction to come from rotating point.
            lightDir = normalize(vec3(rotate_pos - position));
            break;
        default:
            // Set light direction to come from viewer by default.
            lightDir = viewDir;
            break;
    }

    // calculation to be used in specular blinn-phong optimisation.
    vec3 h = normalize(lightDir + viewDir);

    // The ambient component.
    vec3 ambient = light_ambient * mtl_ambient;

    // The diffuse component
    float sDotN = max( dot(lightDir, normDir), 0.0 );
    vec3 diffuse = 
        mtl_diffuse
        * light_diffuse
        * sDotN;

    // The specular component
    vec3 specular = vec3(0.0);
    if ( sDotN > 0.0 )
        specular = light_specular * mtl_specular * 
            pow( max( dot(normDir, h), 0.0 ), 4 * shininess );

    // Return all three components
    return (ambient + diffuse + specular)
        * (texture_exists == 1 ? vec3(texture(texMap, st)) : vec3(1.0));
}

void main(void) {
    vec3 color;
    // Change colour of frag depending on shader state.
    switch (state) {
        case 0:
            // directional light coming from above. Red difuse, blue specular.
            color = blinnPhongPointLight(vertex, normalize(normal),
                    light_ambient_0, light_diffuse_0, light_specular_0);
            break;
        case 1:
            // point light coming from users camera. Green diffuse.
            color = blinnPhongPointLight(vertex, normalize(normal),
                    light_ambient_1, light_diffuse_1, light_specular_1);
            break;
        case 2:
            // Point light rotating around the object. Yellow diffuse.
            color = blinnPhongPointLight(vertex, normalize(normal),
                    light_ambient_2, light_diffuse_2, light_specular_2);
            break;
        case 3:
        default:
            // Purely show the texture colour without lighting.
            color = texture_exists == 1 ? vec3(texture(texMap, st)) : vec3(1.0);
            break;
    }

    fragColour = vec4(color, 1.0);
}


