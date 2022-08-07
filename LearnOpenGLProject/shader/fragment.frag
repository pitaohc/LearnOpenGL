#version 330 core
in vec3 vertexColor; //顶点着色器颜色输入
out vec4 FragColor;  //片段着色器颜色输出
//uniform vec4 ourColor;// 在OpenGL程序代码中设定这个变量
/*
uniform 需要在运行时动态取得ID，增加性能开销
*/
void main()
{
	FragColor = vec4(vertexColor.x,vertexColor.y,vertexColor.z+0.7f,1.0f); //添加一点蓝色
}