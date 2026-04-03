#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture1; // 地球的贴图
uniform vec3 lightPos;      // 太阳的位置
uniform vec3 viewPos;       // 你的摄像机位置 (算高光必须知道你在哪看)
uniform vec3 lightColor;    // 太阳光的颜色

// 【新增】接收外部传来的光照强度
uniform float ambientStrength; 
uniform float specularStrength;

void main() {
    // 1. 环境光 (Ambient) - 极弱的基础光照，防止背面死黑
    vec3 ambient = ambientStrength * lightColor;
    vec3 ambient = ambientStrength * lightColor;
  	
    // 2. 漫反射 (Diffuse) - 核心明暗交界线
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // 如果背向太阳，点乘结果为负，通过 max 截断为 0 (完全处于阴影)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // 3. 高光 
    // 把原本的 float specularStrength = 0.6; 删掉
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
        
    // 最终组合：(环境光 + 漫反射 + 高光) * 贴图原本的颜色
    vec4 texColor = texture(texture1, TexCoords);
    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    
    FragColor = vec4(result, texColor.a);
}