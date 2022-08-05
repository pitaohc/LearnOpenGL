#include "head.h"
#include "Shader.h"
unsigned int HEIGHT = 600, WIDTH = 800;
std::string TITLE = u8"04 shader"; //�ַ���ǰu8ǰ׺��ʾutf-8���룬GBK���������
std::string vertexShaderPath = ".\\shader\\vertex.vert";
std::string fragmentShaderPath = ".\\shader\\fragment.frag";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
    ���봦����
*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
#ifdef _CONSOLE
int main()
#else 
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
{
    // ����OpenGL�汾
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ��������
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cout << "��������ʧ��" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������

    //��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        glfwTerminate();
        return -1;
    }

    //�����ӿ�
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //����shader����
    Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());


    float vertices[] = {
        // λ��              // ��ɫ
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
    };

    UINT32 indexs[]{
        0,1,2,
    };
    UINT32 VAO, VBO, EBO;
    //0. ���ɶ������
    glGenVertexArrays(1, &VAO); //1ֻ��ʾ�������ڶ����������Դ���һ������
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //1. �󶨶����������
    glBindVertexArray(VAO);//�˺�������������صģ�ע��˳��
    //2. �Ѷ������鸴�Ƶ�һ�����㻺���У���OpenGL����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //�˺�������������صģ�ע��˳��
    //3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
    //4. �趨��������ָ��
    // λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //�ȴ�Ŷ��㣬��ɫ����3����������ƫ��
    glEnableVertexAttribArray(1); //layout(location = 1) in vec3 aColor;
    while (!glfwWindowShouldClose(window))
    {
        //����
        processInput(window);
        //��Ⱦָ��
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        //rgb(51, 76, 76)
        glClearColor(51.0 / 255.0, 76.0 / 255.0, 76 / 255.0, 1.0f); //������glClearColor�����������Ļ���õ���ɫ
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
        // �ǵü�����ɫ��
        shaderProgram.use();

        //����λ�ú���ɫ

        /*
        // ����uniform��ɫ
        float timeValue = glfwGetTime();
        float greenValue = (cos(timeValue) / 4.0) + 0.75; //������ɫֵ
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");//�õ�����λ��
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        */

        //����������
        //�����ε���ɫ��������������в�ֵ
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents(); //��Ӧ�¼�
        Sleep(9);  //������120hz
    }

    glfwTerminate();
    return 0;

}