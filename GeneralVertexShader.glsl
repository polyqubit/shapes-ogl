#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Norm;
layout(location=2) in vec2 in_Tex;
out vec2 ex_Tex;
out vec3 ex_Norm;
out vec3 ex_Frag;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    ex_Frag = vec3(model * vec4(in_Position, 1.0));
    ex_Norm = mat3(transpose(inverse(model))) * in_Norm;  
    
    gl_Position = projection * view * vec4(ex_Frag, 1.0);
}