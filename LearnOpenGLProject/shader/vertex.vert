#version 330 core
layout(location = 0) in vec3 aPos; //位置
layout(location = 1) in vec3 aColor; //颜色
layout(location = 2) in vec2 aTexCoord; //纹理位置

out vec3 vertexColor; //颜色输出
out vec2 TexCoord;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    
    vertexColor = aColor; //输出颜色给片段着色器
    TexCoord = aTexCoord;
}