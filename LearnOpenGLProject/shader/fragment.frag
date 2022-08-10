#version 330 core

in vec3 vertexColor; //顶点着色器颜色输入
in vec2 TexCoord;  //片段着色器纹理坐标输入

out vec4 FragColor;  //片段着色器颜色输出

uniform sampler2D outTexture;
void main()
{
//	FragColor = texture(outTexture,TexCoord);
//	FragColor =  vec4(vertexColor,1.0f);
	FragColor = texture(outTexture,TexCoord) * vec4(vertexColor,1.0f);
}