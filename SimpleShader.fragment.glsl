#version 400

in vec4 ex_Color;
out vec4 out_Color;

uniform vec4 TimeColor;

void main(void)
{
  out_Color = TimeColor;
}