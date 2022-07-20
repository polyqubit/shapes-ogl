#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
out vec4 ex_Color;

uniform vec4 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
  gl_Position = projection * view * model * in_Position;
  
  ex_Color = color;
}