#version 400

in vec4 ex_Color;
out vec4 out_Color;

uniform vec4 light_Color;

void main(void)
{
    float ambient_Strength = 0.1;

    out_Color = ambient_Strength * light_Color * ex_Color;
}