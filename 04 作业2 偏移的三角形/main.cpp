#include "head.h"
#include "Shader.h"
unsigned int HEIGHT = 600, WIDTH = 800;
std::string TITLE = u8"04 shader"; //�ַ���ǰu8ǰ׺��ʾutf-8���룬GBK���������
std::string vertexShaderPath = ".\\shader\\vertex.vert";
std::string fragmentShaderPath = ".\\shader\\fragment.frag";

#ifdef _CONSOLE
int main()
#else 
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
{

    return 0;

}