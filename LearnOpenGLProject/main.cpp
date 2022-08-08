#include "head.h"
#include "Shader.h"
unsigned int HEIGHT = 600, WIDTH = 800;
std::string TITLE = u8"04 ��ҵ3 λ������ɫ"; //�ַ���ǰu8ǰ׺��ʾutf-8���룬GBK���������
std::string vertexShaderPath = ".\\shader\\vertex.vert";
std::string fragmentShaderPath = ".\\shader\\fragment.frag";


UINT32 getFPS()
{
    static int fps = 0;
    static int lastTime = clock(); // ms
    static int frameCount = 0;
    constexpr int UNITTIME = 100; //����ʱ�䣬��λms
    ++frameCount;

    int curTime = clock();
    if (curTime - lastTime > UNITTIME) // ȡ�̶�ʱ����Ϊ1��
    {
        fps = frameCount * 1000 / (curTime - lastTime);
        frameCount = 0;
        lastTime = curTime;
        std::cout<<"FPS: " << fps << std::endl;
    }
    return fps;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
    ����ص���������Ӧ�����ź�
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//����esc�˳�����
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

//ʹ��uniform����һ��ˮƽƫ�������ڶ�����ɫ����ʹ�����ƫ�������������ƶ�����Ļ�Ҳࣺ
#ifdef _CONSOLE
int main()
#else 
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
{
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "��������ʧ��" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window); //����������
    glfwSetKeyCallback(window, key_callback); //���ü��̻ص�����
    //��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "��ʼ��GLADʧ��" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    //�����ӿ�
    glViewport(0, 0, WIDTH, HEIGHT);

    Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    float vertices[] = {
        // λ��              // ��ɫ
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // A
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // B
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // C
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // D
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   // E
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   // F
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   // G
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   // H

    };
    UINT32 indexs[]{
    0,1,2,
    0,2,3,
    0,1,4,
    1,4,5,
    4,5,6,
    4,5,7,
    2,6,7,
    2,3,7,
    0,4,7,
    0,3,7,
    1,5,6,
    1,2,6,
    };
    UINT32 VAO, VBO, EBO;
    //0. ���ɶ������
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //1. �󶨶����������
    glBindVertexArray(VAO);
    //2. �Ѷ������鸴�Ƶ�һ�����㻺���У���OpenGL����
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //����VBOΪ���黺����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //����EBOΪ����������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    while (!glfwWindowShouldClose(window))
    {
        //1. ��Ӧ�¼�
        getFPS();
        glfwPollEvents();
        //2. ��Ⱦ
        //��ջ�����
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(51.0 / 255.0, 76.0 / 255.0, 76 / 255.0, 1.0f); //������glClearColor�����������Ļ���õ���ɫ
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������

        shaderProgram.use();
        float timeValue = glfwGetTime();
        float deltaXPos = cos(timeValue) / 4.0; //������ɫֵ
        float deltaYPos = sin(timeValue) / 4.0;
        //shaderProgram.setFloat("deltaX", deltaXPos);
        //shaderProgram.setFloat("deltaY", deltaYPos);
        shaderProgram.setVec3f("deltaPos", deltaXPos, deltaYPos, 0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));
        glBindVertexArray(0);
        //3. �л�������
        glfwSwapBuffers(window);

        //4. TODO ����֡��
        //��ʱû�нϺõĽ���취��sleep���ܴﵽĿ��֡�ʣ�ѭ���ӳ�ռ��CPU���Ҿ��Ȳ���
    }
    glfwTerminate();
    return 0;

}