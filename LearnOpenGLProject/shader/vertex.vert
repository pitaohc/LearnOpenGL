#version 330 core
layout(location = 0) in vec3 aPos; //λ��
layout(location = 1) in vec3 aColor; //��ɫ
layout(location = 2) in vec2 aTexCoord; //����λ��

out vec3 vertexColor; //��ɫ���
out vec2 TexCoord;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    
    vertexColor = aColor; //�����ɫ��Ƭ����ɫ��
    TexCoord = aTexCoord;
}