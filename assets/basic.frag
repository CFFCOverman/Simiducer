#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float u_Year; // 接收来自 C++ 的年份数据

void main()
{
    // 1. 获取地球本来的真实颜色
    vec4 texColor = texture(texture1, TexCoord);

    // 2. 计算时间偏移量 (以 2026 年为正常基准点)
    // 除以 1000 是为了让变化显得平滑一些
    float timeShift = (u_Year - 2026.0) / 1000.0; 

    if (timeShift < 0.0) {
        // 【过去：冰河世纪】
        // 降低红色，增加蓝色和绿色（偏青白色）
        texColor.r += timeShift * 0.5; 
        texColor.b -= timeShift * 0.8; 
        texColor.g -= timeShift * 0.3;
    } else {
        // 【未来：温室效应】
        // 增加红色和绿色（变黄变焦），大幅降低蓝色（海洋干涸）
        texColor.r += timeShift * 0.8;
        texColor.g += timeShift * 0.4;
        texColor.b -= timeShift * 1.2;
    }

    // 确保颜色值在 0.0 到 1.0 之间
    texColor = clamp(texColor, 0.0, 1.0);

    FragColor = texColor;
}