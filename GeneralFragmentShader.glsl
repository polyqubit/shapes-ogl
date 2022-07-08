#version 400

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

out vec4 frag_Color;

in vec2 ex_Tex;
in vec3 ex_Norm;  
in vec3 ex_Frag;  
  
uniform vec3 light_Pos; 
uniform vec3 view_Pos; 
uniform vec3 light_Color;
uniform vec3 object_Color;

void main()
{
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, ex_Tex));
  	
    // diffuse 
    vec3 norm = normalize(ex_Norm);
    vec3 light_Dir = normalize(light_Pos - ex_Frag);
    float diff = max(dot(norm, light_Dir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, ex_Tex));  
    
    // specular
    vec3 view_Dir = normalize(view_Pos - ex_Frag);
    vec3 reflect_Dir = reflect(-light_Dir, norm);  
    float spec = pow(max(dot(view_Dir, reflect_Dir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;  
        
    vec3 result = (ambient + diffuse + specular) * object_Color;
    frag_Color = vec4(result, 1.0);
}