#version 400

out vec4 frag_Color;

in vec3 ex_Norm;  
in vec3 ex_Frag;  
  
uniform vec3 light_Pos; 
uniform vec3 view_Pos; 
uniform vec3 light_Color;
uniform vec3 object_Color;

void main()
{
    // ambient
    float ambient_Strength = 0.1;
    vec3 ambient = ambient_Strength * light_Color;
  	
    // diffuse 
    vec3 norm = normalize(ex_Norm);
    vec3 light_Dir = normalize(light_Pos - ex_Frag);
    float diff = max(dot(norm, light_Dir), 0.0);
    vec3 diffuse = diff * light_Color;
    
    // specular
    float specular_Strength = 0.5;
    vec3 view_Dir = normalize(view_Pos - ex_Frag);
    vec3 reflect_Dir = reflect(-light_Dir, norm);  
    float spec = pow(max(dot(view_Dir, reflect_Dir), 0.0), 32);
    vec3 specular = specular_Strength * spec * light_Color;  
        
    vec3 result = (ambient + diffuse + specular) * object_Color;
    frag_Color = vec4(result, 1.0);
}