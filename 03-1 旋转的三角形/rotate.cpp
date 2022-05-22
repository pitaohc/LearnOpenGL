#include<glad/glad.h>
#include<glfw3.h>
#include<iostream>
#include<string>
#include<cmath>
#include<time.h>
#include<fstream>
#include<sstream>
#include<Windows.h>
constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH = 600;
constexpr char TITLE[] = "03 Hello Triangle";
constexpr double rotateTheta = 0.005;
std::string vertexShaderPath = "I:\\Courses\\LearnOpenGL\\LearnOpenGL\\03-1 ��ת��������\\shader\\vertex.vert";
std::string fragmentShaderPath = "I:\\Courses\\LearnOpenGL\\LearnOpenGL\\03-1 ��ת��������\\shader\\fragment.frag";
std::string readShader(const std::string& path)
{
    std::fstream file(path);
    std::string res, temp;
    while (!file.eof())
    {
        temp = "";
        getline(file, temp);
        res += temp + '\n';
    }
    return res;
}
void rotate(float* vertic, double theta)
{
    double rad = 3.1415926 / 180 * theta; //�Ƕ�ת����
    double sinTheta = sin(theta); //��ʱ���sin
    double cosTheta = cos(theta); //��ʱ���cos
    // ʹ�ô����������
    float xp = cosTheta * vertic[0] - sinTheta * vertic[1];
    float yp = sinTheta * vertic[0] + cosTheta * vertic[1];
    vertic[0] = xp, vertic[1] = yp;
}
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
void initGLFW()
{
    glfwInit(); //��ʼ��glfw
    //����3.3�汾��OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //���汾��3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //�ΰ汾��3
    //ʹ�ú���ģʽGLFW_OPENGL_CORE_PROFILE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int main()
{

    initGLFW();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "����GLFW����ʧ��" << std::endl;
        glfwTerminate(); //��ֹ����
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //�˴��ɹ�ִ��

    std::string vertexShaderSource = readShader(vertexShaderPath);
    const char* vertexShaderSourceC = vertexShaderSource.c_str();

    //����vertex_shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //�������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ�������
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, nullptr);
    glCompileShader(vertexShader);

    //����Ƿ����ɹ�
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //����Ƭ����ɫ��
    std::string fragmentShaderSource = readShader(fragmentShaderPath);
    const char* fragmentShaderSourceC = fragmentShaderSource.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, nullptr);
    glCompileShader(fragmentShader);

    success = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //��ɫ������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); //������ɫ��
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); //������ɫ������

    success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    //��������������
    //glUseProgram(shaderProgram);

    //�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    //���붥�����ݵ��ִ�
    float vertices[] = {
     0.0f,  1.0f, 0.0f,
     0.87f, -0.5f, 0.0f,
     -0.87f,  -0.5f, 0.0f
    };
    /*
        ���㻺�������������OpenGL�̳��е�һ�����ֵ�OpenGL���󡣾���
        OpenGL�е���������һ�������������һ����һ�޶���ID���������ǿ�
        ��ʹ��glGenBuffers������һ������ID����һ��VBO����
    */
    unsigned int VBO; //Vertex Buffer Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    /*
        OpenGL�кܶ໺��������ͣ����㻺�����Ļ���������
        GL_ARRAY_BUFFER��OpenGL��������ͬʱ�󶨶�����壬ֻҪ����
        �ǲ�ͬ�Ļ������͡����ǿ���ʹ��glBindBuffer�������´����Ļ�
        ��󶨵�GL_ARRAY_BUFFERĿ���ϣ�
    */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    //����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
    glVertexAttribPointer(
        0, //ָ������Ҫ���õĶ������� layout(location = 0)
        3, //ָ���������ԵĴ�С vec3
        GL_FLOAT, //vec��float����
        GL_FALSE, //�����Ƿ�ϣ�����ݱ���׼��(Normalize)
        3 * sizeof(float), //����
        (void*)0 //�����ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0
    );
    glEnableVertexAttribArray(0);
    while (!glfwWindowShouldClose(window))
    {
        //����
        processInput(window);

        for (int i = 0; i < 3; i++)
        {
            rotate(vertices + i * 3, rotateTheta);
        }
        /*
        glBufferData, glVertexAttribPointer
        */
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        //����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
        glVertexAttribPointer(
            0, //ָ������Ҫ���õĶ������� layout(location = 0)
            3, //ָ���������ԵĴ�С vec3
            GL_FLOAT, //vec��float����
            GL_FALSE, //�����Ƿ�ϣ�����ݱ���׼��(Normalize)
            3 * sizeof(float), //����
            (void*)0 //�����ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0
        );
        //glEnableVertexAttribArray(0);

        //��Ⱦָ��
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(.2f, .3f, .3f, 1.0f); //������glClearColor�����������Ļ���õ���ɫ
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //��鲢�����¼�������������
        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(9);  //������120hz
    }

    glfwTerminate();
    return 0;
}