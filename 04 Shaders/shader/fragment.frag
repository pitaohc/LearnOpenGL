#version 330 core
in vec3 vertexColor; //������ɫ����ɫ����
out vec4 FragColor;  //Ƭ����ɫ����ɫ���
//uniform vec4 ourColor;// ��OpenGL����������趨�������
/*
uniform ��Ҫ������ʱ��̬ȡ��ID���������ܿ���
*/
void main()
{
	FragColor = vec4(vertexColor,1.0f);
}