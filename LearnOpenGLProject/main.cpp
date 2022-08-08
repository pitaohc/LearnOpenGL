#include "head.h"
#include "Shader.h"
unsigned int HEIGHT = 600, WIDTH = 800;
std::string TITLE = u8"04 作业3 位置与颜色"; //字符串前u8前缀表示utf-8编码，GBK编码会乱码
std::string vertexShaderPath = ".\\shader\\vertex.vert";
std::string fragmentShaderPath = ".\\shader\\fragment.frag";


UINT32 getFPS()
{
    static int fps = 0;
    static int lastTime = clock(); // ms
    static int frameCount = 0;
    constexpr int UNITTIME = 100; //更新时间，单位ms
    ++frameCount;

    int curTime = clock();
    if (curTime - lastTime > UNITTIME) // 取固定时间间隔为1秒
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
    输入回调函数，相应键盘信号
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//按下esc退出程序
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

//使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧：
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
        std::cerr << "创建窗口失败" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window); //设置上下文
    glfwSetKeyCallback(window, key_callback); //设置键盘回调函数
    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "初始化GLAD失败" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    //创建视口
    glViewport(0, 0, WIDTH, HEIGHT);

    Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    float vertices[] = {
        // 位置              // 颜色
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
    //0. 生成对象序号
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //1. 绑定顶点数组对象
    glBindVertexArray(VAO);
    //2. 把顶点数组复制到一个顶点缓冲中，供OpenGL调用
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //设置VBO为数组缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //设置EBO为索引缓冲区
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    while (!glfwWindowShouldClose(window))
    {
        //1. 响应事件
        getFPS();
        glfwPollEvents();
        //2. 渲染
        //清空缓冲区
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(51.0 / 255.0, 76.0 / 255.0, 76 / 255.0, 1.0f); //调用了glClearColor来设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区

        shaderProgram.use();
        float timeValue = glfwGetTime();
        float deltaXPos = cos(timeValue) / 4.0; //计算颜色值
        float deltaYPos = sin(timeValue) / 4.0;
        //shaderProgram.setFloat("deltaX", deltaXPos);
        //shaderProgram.setFloat("deltaY", deltaYPos);
        shaderProgram.setVec3f("deltaPos", deltaXPos, deltaYPos, 0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));
        glBindVertexArray(0);
        //3. 切换缓冲区
        glfwSwapBuffers(window);

        //4. TODO 控制帧率
        //暂时没有较好的解决办法，sleep不能达到目标帧率，循环延迟占用CPU，且精度不高
    }
    glfwTerminate();
    return 0;

}