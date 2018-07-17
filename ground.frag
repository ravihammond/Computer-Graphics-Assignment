#version 330

// Vectors interpolated from vert shader.
in vec4 vertex;
in vec3 normal;
in vec2 st;

//// Texture to be sampled from.
uniform sampler2D texMap;
const int texture_exists = 1;

uniform mat4 view_matrix;

const vec3 mtl_ambient = vec3(1.0,1.0,1.0);
const vec3 mtl_diffuse = vec3(1.0,1.0,1.0);
const vec3 mtl_specular = vec3(1.0,1.0,1.0);
const float shininess = 64;

uniform vec3 directional_ambient;
uniform vec3 directional_diffuse;
uniform vec3 directional_specular;
uniform vec3 directional_light_direction;

uniform vec3 point_ambient;
uniform vec3 point_diffuse;
uniform vec3 point_specular;
uniform vec4 point_light_position;

out vec4 fragColour;

// Regular phong shading.
vec3 directionalLight(in vec4 position, in vec3 normDir) {
    // create view direction from position.
    vec3 viewDir = normalize(-position.xyz);
    //vec4 transLightDir = view_matrix * vec4(directional_light_direction, 1.0);
    //vec3 lightDir = normalize(vec3(transLightDir));
    vec3 lightDir = normalize(directional_light_direction);

    // calculation to be used in specular blinn-phong optimisation.
    vec3 h = normalize(lightDir + viewDir);

    // The ambient component.
    vec3 ambient = directional_ambient * mtl_ambient;

    // The diffuse component
    float sDotN = max( dot(lightDir, normDir), 0.0 );
    vec3 diffuse = 
        mtl_diffuse
        * directional_diffuse
        * sDotN;

    // The specular component
    vec3 specular = vec3(0.0);
    if ( sDotN > 0.0 )
        specular = directional_specular * mtl_specular * 
            pow( max( dot(normDir, h), 0.0 ), 4 * shininess );

    // Return all three components
    return (ambient + diffuse + specular)
        * (texture_exists == 1 ? vec3(texture(texMap, st)) : vec3(1.0));

}

//vec3 directionalLight(in vec4 position, in vec3 norm) {

    //vec4 light_pos = view_matrix * directional_light_direction;

    //vec3 l = normalize(vec3(light_pos - position));
    //vec3 v = normalize(-position.xyz);
    //vec3 r = reflect( -l, norm );
    
    //vec3 ambient = directional_ambient * mtl_ambient;
	
    //// The diffuse component
    //float sDotN = max( dot(l, norm), 0.0 );
    //vec3 diffuse = directional_diffuse 
        //* mtl_diffuse 
        //* sDotN;

    //// The specular component
    //vec3 spec = vec3(0.0);
    //if ( sDotN > 0.0 )
		//spec = directional_specular * mtl_specular *
            //pow( max( dot(r, v), 0.0 ), shininess );

    //return (ambient + diffuse + spec)
        //* vec3(texture(texMap, st));
//}

vec3 phongPointLight(in vec4 position, in vec3 norm) {

    vec4 light_pos = view_matrix * point_light_position;
    float dist = length(vec3(light_pos - position));

    vec3 l = normalize(vec3(light_pos - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect( -l, norm );
    
    vec3 ambient = point_ambient * mtl_ambient;
	
    // The diffuse component
    float sDotN = max( dot(l, norm), 0.0 );
    vec3 diffuse = point_diffuse 
        * mtl_diffuse 
        * sDotN
        * (1/sqrt(dist / 10));

    // The specular component
    vec3 spec = vec3(0.0);
    if ( sDotN > 0.0 )
		spec = point_specular * mtl_specular *
            pow( max( dot(r, v), 0.0 ), shininess );

    return (ambient + diffuse + spec)
        * vec3(texture(texMap, st));
}

void main(void) {
    vec3 dir = directionalLight(vertex, normalize(normal));
    vec3 point = phongPointLight(vertex, normalize(normal));

    fragColour = vec4(dir + point, 1.0);
}

