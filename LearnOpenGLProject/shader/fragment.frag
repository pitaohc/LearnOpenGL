#version 330 core

in vec3 vertexColor; //������ɫ����ɫ����
in vec2 TexCoord;  //Ƭ����ɫ��������������

out vec4 FragColor;  //Ƭ����ɫ����ɫ���

uniform sampler2D outTexture;
void main()
{
//	FragColor = texture(outTexture,TexCoord);
//	FragColor =  vec4(vertexColor,1.0f);
	FragColor = texture(outTexture,TexCoord) * vec4(vertexColor,1.0f);
}