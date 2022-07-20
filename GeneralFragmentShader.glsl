#version 400

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
uniform Material material;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadravious;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 5
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct Light {
    vec3 position;
    vec3 direction;
    float cutoff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadravious;
};
uniform Light movlight;

out vec4 frag_Color;

in vec2 ex_Tex;
in vec3 ex_Norm;  
in vec3 ex_Frag;  

uniform vec3 view_Pos; 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(ex_Norm);
    vec3 viewDir = normalize(view_Pos - ex_Frag);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, ex_Frag, viewDir);    
    // phase 3: Spot light
    // result += CalcSpotLight(spotLight, norm, ex_Frag, viewDir);    
    
    frag_Color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, ex_Tex));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, ex_Tex));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, ex_Tex));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 5.0 / (light.constant + light.linear * distance + light.quadravious * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, ex_Tex));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, ex_Tex));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, ex_Tex));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 