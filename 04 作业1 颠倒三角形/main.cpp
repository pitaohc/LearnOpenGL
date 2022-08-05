#include "head.h"
#include "Shader.h"
unsigned int HEIGHT = 600, WIDTH = 800;
std::string TITLE = u8"04 shader"; //字符串前u8前缀表示utf-8编码，GBK编码会乱码
std::string vertexShaderPath = ".\\shader\\vertex.vert";
std::string fragmentShaderPath = ".\\shader\\fragment.frag";

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
#ifdef _CONSOLE
int main()
#else 
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
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

    //创建shader程序
    Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());


    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    UINT32 indexs[]{
        0,1,2,
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
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //先存放顶点，颜色会有3个浮点数的偏移
    glEnableVertexAttribArray(1); //layout(location = 1) in vec3 aColor;
    while (!glfwWindowShouldClose(window))
    {
        //输入
        processInput(window);
        //渲染指令
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        //rgb(51, 76, 76)
        glClearColor(51.0 / 255.0, 76.0 / 255.0, 76 / 255.0, 1.0f); //调用了glClearColor来设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
        // 记得激活着色器
        shaderProgram.use();

        //更新位置和颜色

        /*
        // 更新uniform颜色
        float timeValue = glfwGetTime();
        float greenValue = (cos(timeValue) / 4.0) + 0.75; //计算颜色值
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");//得到变量位置
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        */

        //绘制三角形
        //三角形的颜色会由三个顶点进行插值
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents(); //响应事件
        Sleep(9);  //控制在120hz
    }

    glfwTerminate();
    return 0;

}