#version 400

layout(location=0) in vec4 in_Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
  gl_Position = projection * view * model * in_Position;
}