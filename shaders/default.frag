#version 330 core

#define NUM_LIGHTS 2

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;
uniform vec3 light_dir;

uniform PointLight lights[NUM_LIGHTS];

in vec3 vert_color;
in vec3 normal;
in vec2 tex_coords;
in vec3 view_pos;
in vec3 frag_pos;
in vec4 frag_pos_light_space;

out vec4 FragColor;

vec3 calculatePointLight(PointLight p, vec3 frag_position, vec3 view_position, vec3 normal, vec3 color) {
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(p.position - frag_position);
    float distance = length(p.position - frag_position);
    float attenuation = 1.0 / (p.constant + p.linear * distance + p.quadratic * distance * distance);
    float diff = max(dot(norm, light_dir), 0.0);

    vec3 view_direction = normalize(view_position - frag_position);
    vec3 reflect_direction = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 16);
    float specular_strength = 0.5f;

    vec3 ambient = p.ambient * attenuation;
    vec3 diffuse = p.diffuse * diff * attenuation;
    vec3 specular = p.specular * spec * specular_strength * attenuation;
    vec3 result = (specular + diffuse + ambient) * color;
    return result;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(normal);
    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main() {

    // vec3 color_out = vec3(0.0);
    // vec3 color = mix(texture(ourTexture, tex_coords) * vec4(vert_color, 1.0f), vec4(normal, 1.0f), 0.0f).xyz;//vec4(0.0f, 0.0f, vertex_color, 1.0f);
    // for(int i = 0; i < NUM_LIGHTS; i++)
    //     color_out += calculatePointLight(lights[i], frag_pos, view_pos, normal, color);
    // FragColor = vec4(color_out, 1.0);

    vec3 color = texture(ourTexture, tex_coords).rgb;
    vec3 normal = normalize(normal);
    vec3 lightColor = vec3(0.6);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    // vec3 lightDir = normalize(lightPos - frag_pos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(view_pos - frag_pos);
    vec3 reflectDir = reflect(-light_dir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(light_dir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(frag_pos_light_space);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    // vec3 lighting = shadow * color;    
    
    FragColor = vec4(lighting, 1.0);
}