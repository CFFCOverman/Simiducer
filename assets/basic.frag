#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float u_Year; // 接收来自 C++ 的年份数据

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    float timeShift = (u_Year - 2026.0) / 1000.0; 

    if (timeShift < 0.0) {
        texColor.r += timeShift * 0.5; 
        texColor.b -= timeShift * 0.8; 
        texColor.g -= timeShift * 0.3;
    } else {
        texColor.r += timeShift * 0.8;
        texColor.g += timeShift * 0.4;
        texColor.b -= timeShift * 1.2;
    }

    texColor = clamp(texColor, 0.0, 1.0);
    FragColor = texColor;
}