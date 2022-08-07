#version 330 core
layout(location = 0) in vec3 aPos; //位置
layout(location = 1) in vec3 aColor; //颜色

uniform vec3 deltaPos;

out vec3 vertexColor; //颜色输出
void main()
{
    gl_Position = vec4(aPos.x+deltaPos.x,aPos.y+deltaPos.y,aPos.z+deltaPos.z, 1.0);
    vertexColor = aColor; //输出颜色给片段着色器
//    vertexColor = vec4(0.5,0.0,0.0,1.0);
}