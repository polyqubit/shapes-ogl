#version 400

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
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

uniform Light light;
uniform Material material;

out vec4 frag_Color;

in vec2 ex_Tex;
in vec3 ex_Norm;  
in vec3 ex_Frag;  
  
uniform vec3 view_Pos; 

void main()
{
    
    float distance = length(light.position - ex_Frag);
    float attenuation = 15.0 / (light.constant + light.linear * distance + light.quadravious * (distance * distance));

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, ex_Tex).rgb;
  	ambient *= attenuation;

    // diffuse 
    vec3 norm = normalize(ex_Norm);
    vec3 lightDir = normalize(light.position - ex_Frag);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, ex_Tex).rgb;  
    diffuse *= attenuation;
    
    // specular
    vec3 viewDir = normalize(view_Pos - ex_Frag);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (texture(material.specular, ex_Tex).rgb * spec) * light.specular;
    specular *= attenuation;
        
    vec3 result = ambient + diffuse + specular;
    frag_Color = vec4(result, 1.0);
}