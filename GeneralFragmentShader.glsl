#version 400

in vec4 ex_Color;
out vec4 out_Color;

uniform vec4 light_Color;

void main(void)
{
  out_Color = light_Color * ex_Color;
}