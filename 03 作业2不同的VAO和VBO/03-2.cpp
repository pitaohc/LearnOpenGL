#include"header.h"

unsigned int HEIGHT = 600, WIDTH = 800;
std::string TITLE = u8"03 作业2 不同的VAO和VBO"; //字符串前u8前缀表示utf-8编码，GBK编码会乱码
std::string vertexShaderPath = ".\\shader\\vertex.vert";
std::string fragmentShaderPath = ".\\shader\\fragment.frag";

/*
    读取文件到字符串中
    传入文件路径，输出内容字符串
    目的为了直接读取shader代码
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
    输入处理函数
*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // 设置OpenGL版本
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cout << "创建窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//通知GLFW将我们窗口的上下文设置为当前线程的主上下文

    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化GLAD失败" << std::endl;
        glfwTerminate();
        return -1;
    }

    //创建视口
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //添加并编译几何着色器
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
        std::cout << "Error: 创建几何着色器失败！\n"
            << infoLog << std::endl;
        return -1;

    }

    //添加并编译片段着色器
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
        std::cout << "Error: 创建片段着色器失败！\n"
            << infoLog << std::endl;
        return -1;
    }

    //链接两个着色器代码形成着色器程序
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
        std::cout << "Error: 创建着色器程序失败！\n"
            << info << std::endl;
        return -1;

    }
    //删除着色器对象，我们不再需要它们了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    float vertices[]{
         0.0f, 1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         0.5f,-0.5f, 0.0f,
        -0.5f,-0.5f, 0.0f,
        -1.0f, 0.0f, 0.0f,
    };

    UINT32 indexs[]{
        0,1,2,
        0,2,3,
        0,3,4,
    };
    UINT32 VAO, VBO, EBO;
    //0. 生成对象序号
    glGenVertexArrays(1, &VAO); //1只表示个数，第二个参数可以传入一个数组
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //1. 绑定顶点数组对象
    glBindVertexArray(VAO);//此函数是上下文相关的，注意顺序
    //2. 把顶点数组复制到一个顶点缓冲中，供OpenGL调用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //此函数是上下文相关的，注意顺序
    //3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
    //4. 设定顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //参数0与着色器代码中的location有关，启用了顶点着色器的（location = 0）的属性变量

    while (!glfwWindowShouldClose(window))
    {
        //输入
        processInput(window);
        //渲染指令
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        glClearColor(.2f, .3f, .3f, 1.0f); //调用了glClearColor来设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents(); //响应事件
        Sleep(9);  //控制在120hz
    }

    glfwTerminate();
    return 0;
}
