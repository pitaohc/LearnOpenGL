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
std::string vertexShaderPath = "I:\\Courses\\LearnOpenGL\\LearnOpenGL\\03-1 旋转的三角形\\shader\\vertex.vert";
std::string fragmentShaderPath = "I:\\Courses\\LearnOpenGL\\LearnOpenGL\\03-1 旋转的三角形\\shader\\fragment.frag";
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
    double rad = 3.1415926 / 180 * theta; //角度转弧度
    double sinTheta = sin(theta); //临时存放sin
    double cosTheta = cos(theta); //临时存放cos
    // 使用代数运算计算
    float xp = cosTheta * vertic[0] - sinTheta * vertic[1];
    float yp = sinTheta * vertic[0] + cosTheta * vertic[1];
    vertic[0] = xp, vertic[1] = yp;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
    输入处理函数
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
    glfwInit(); //初始化glfw
    //接受3.3版本的OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //主版本号3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //次版本号3
    //使用核心模式GLFW_OPENGL_CORE_PROFILE
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
        std::cout << "创建GLFW窗口失败" << std::endl;
        glfwTerminate(); //终止函数
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化GLAD失败" << std::endl;
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //此处成功执行

    std::string vertexShaderSource = readShader(vertexShaderPath);
    const char* vertexShaderSourceC = vertexShaderSource.c_str();

    //创建vertex_shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //把这个着色器源码附加到着色器对象上，然后编译它
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, nullptr);
    glCompileShader(vertexShader);

    //检查是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //创建片段着色器
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

    //着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); //附加着色器
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); //链接着色器程序

    success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    //激活这个程序对象
    //glUseProgram(shaderProgram);

    //在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    //存入顶点数据到现存
    float vertices[] = {
     0.0f,  1.0f, 0.0f,
     0.87f, -0.5f, 0.0f,
     -0.87f,  -0.5f, 0.0f
    };
    /*
        顶点缓冲对象是我们在OpenGL教程中第一个出现的OpenGL对象。就像
        OpenGL中的其它对象一样，这个缓冲有一个独一无二的ID，所以我们可
        以使用glGenBuffers函数和一个缓冲ID生成一个VBO对象：
    */
    unsigned int VBO; //Vertex Buffer Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    /*
        OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是
        GL_ARRAY_BUFFER。OpenGL允许我们同时绑定多个缓冲，只要它们
        是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓
        冲绑定到GL_ARRAY_BUFFER目标上：
    */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    glVertexAttribPointer(
        0, //指定我们要配置的顶点属性 layout(location = 0)
        3, //指定顶点属性的大小 vec3
        GL_FLOAT, //vec是float对象
        GL_FALSE, //我们是否希望数据被标准化(Normalize)
        3 * sizeof(float), //步长
        (void*)0 //数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0
    );
    glEnableVertexAttribArray(0);
    while (!glfwWindowShouldClose(window))
    {
        //输入
        processInput(window);

        for (int i = 0; i < 3; i++)
        {
            rotate(vertices + i * 3, rotateTheta);
        }
        /*
        glBufferData, glVertexAttribPointer
        */
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
        glVertexAttribPointer(
            0, //指定我们要配置的顶点属性 layout(location = 0)
            3, //指定顶点属性的大小 vec3
            GL_FLOAT, //vec是float对象
            GL_FALSE, //我们是否希望数据被标准化(Normalize)
            3 * sizeof(float), //步长
            (void*)0 //数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0
        );
        //glEnableVertexAttribArray(0);

        //渲染指令
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(.2f, .3f, .3f, 1.0f); //调用了glClearColor来设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //检查并调用事件，交换缓冲区
        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(9);  //控制在120hz
    }

    glfwTerminate();
    return 0;
}