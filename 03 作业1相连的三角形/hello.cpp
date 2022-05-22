#include"header.h"

unsigned int HEIGHT = 600, WIDTH = 800;
std::string TITLE = u8"03 ��ҵ1 ���������"; //�ַ���ǰu8ǰ׺��ʾutf-8���룬GBK���������
std::string vertexShaderPath = "I:\\Courses\\LearnOpenGL\\LearnOpenGL\\03 ��ҵ1������������\\shader\\vertex.vert";
std::string fragmentShaderPath = "I:\\Courses\\LearnOpenGL\\LearnOpenGL\\03 ��ҵ1������������\\shader\\fragment.frag";

/*
    ��ȡ�ļ����ַ�����
    �����ļ�·������������ַ���
    Ŀ��Ϊ��ֱ�Ӷ�ȡshader����
*/
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


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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

    //��Ӳ����뼸����ɫ��
    std::string vertexShaderSource = readShader(vertexShaderPath);
    const char* vertexShaderSourceC = vertexShaderSource.c_str();
    UINT32 vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, nullptr);
    glCompileShader(vertexShader);
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Error: ����������ɫ��ʧ�ܣ�\n"
            << infoLog << std::endl;
        return -1;

    }

    //��Ӳ�����Ƭ����ɫ��
    std::string fragmentShaderSource = readShader(fragmentShaderPath);
    const char* fragmentShaderSourceC = fragmentShaderSource.c_str();
    UINT32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Error: ����Ƭ����ɫ��ʧ�ܣ�\n"
            << infoLog << std::endl;
        return -1;
    }

    //����������ɫ�������γ���ɫ������
    UINT32 shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
        std::cout << "Error: ������ɫ������ʧ�ܣ�\n"
            << info << std::endl;
        return -1;

    }
    //ɾ����ɫ���������ǲ�����Ҫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    float vertices[]{
        0.5f, 0.5f, 0.0f, //P0��һ����
       -0.5f, 0.5f, 0.0f, //P1�ڶ�����
       -0.5f,-0.5f, 0.0f, //P2��������

        0.5f, 0.5f, 0.0f, //P0��һ����
       -0.5f,-0.5f, 0.0f, //P2��������
        0.5f,-0.5f, 0.0f, //P3��������

    };

    UINT32 indexs[]{
        0, 1, 2,
        0, 2, 3,
    };
    UINT32 VAO,VBO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(EBO, sizeof(indexs), indexs, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        //����
        processInput(window);
        //��Ⱦָ��
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(.2f, .3f, .3f, 1.0f); //������glClearColor�����������Ļ���õ���ɫ
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        //��鲢�����¼�������������
        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(9);  //������120hz
    }

    glfwTerminate();
    return 0;
}
