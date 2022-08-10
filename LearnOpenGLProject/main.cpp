#include "head.h"
#include "Shader.h"
unsigned int HEIGHT = 800, WIDTH = 800;
std::string TITLE = u8"05 纹理"; //字符串前u8前缀表示utf-8编码，GBK编码会乱码
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
        std::cout << "FPS: " << fps << std::endl;
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
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  // top left 
    };
    UINT32 indexs[] = {
        0, 1, 3, // first triangle
        1, 2, 3,  // second triangle
    };

    //加载纹理图片
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char* data = stbi_load("img/01/06/container.jpg", &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);    /*
    第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
    第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
    第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
    第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
    下个参数应该总是被设为0（历史遗留的问题）。
    第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
    最后一个参数是真正的图像数据。
    */

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindTexture(GL_TEXTURE_2D, texture);


    while (!glfwWindowShouldClose(window))
    {
        //1. 响应事件
        getFPS();
        //std::cout << "FPS: " << getFPS() << std::endl;
        glfwPollEvents();
        //2. 渲染
        //清空缓冲区
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        glClearColor(51.0 / 255.0, 76.0 / 255.0, 76 / 255.0, 1.0f); //调用了glClearColor来设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //3. 切换缓冲区
        glfwSwapBuffers(window);

        //4. TODO 控制帧率
        //暂时没有较好的解决办法，sleep不能达到目标帧率，循环延迟占用CPU，且精度不高
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;

}