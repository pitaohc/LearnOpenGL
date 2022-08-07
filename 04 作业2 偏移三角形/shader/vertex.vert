#version 330 core
layout(location = 0) in vec3 aPos; //λ��
layout(location = 1) in vec3 aColor; //��ɫ

uniform vec3 deltaPos;

out vec3 vertexColor; //��ɫ���
void main()
{
    gl_Position = vec4(aPos.x+deltaPos.x,aPos.y+deltaPos.y,aPos.z+deltaPos.z, 1.0);
    vertexColor = aColor; //�����ɫ��Ƭ����ɫ��
//    vertexColor = vec4(0.5,0.0,0.0,1.0);
}