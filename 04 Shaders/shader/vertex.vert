#version 330 core
layout(location = 0) in vec3 aPos; //λ��
layout(location = 1) in vec3 aColor; //��ɫ

out vec3 vertexColor; //��ɫ���
void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor; //�����ɫ��Ƭ����ɫ��
//    vertexColor = vec4(0.5,0.0,0.0,1.0);
}