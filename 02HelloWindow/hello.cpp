#include<glad/glad.h>
#include<glfw3.h>
#include<iostream>
#include<time.h>
#include<Windows.h>
constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH = 800;
constexpr char TITLE[] = "LearnOpenGL";

/*
    窗体大小更新函数
    更新窗体时，同时更新视口的大小
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
    输入处理函数
*/
void processInput(GLFWwindow* window)
{
    /*
        如果检测到按键escape处于按下状态，就设置窗体退出标记为true
    */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
void initGLFW()
{
    glfwInit(); //初始化glfw
    /*
       glfwWindowHint(int hint, int value)配置glfw
       @param hint 选项的名称
       @param value 选项的值
    */
    //接受3.3版本的OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //主版本号3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //次版本号3
    //使用核心模式GLFW_OPENGL_CORE_PROFILE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

}
/*
    OpenGL使用流程
    1. 初始化GLFW
    2. 创建窗体对象
    3. 使用GLAD绑定函数
    4. 设置视口
*/

//int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    return(0);
//
//}
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    initGLFW();
    /*
        GLFWAPI GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

        glfwCreateWindow 函数创建窗体
        @param width 窗体宽度
        @param height 窗体高度
        @param title 窗体标题
        @return GLFWwindow* 返回窗体对象的指针
        创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "创建GLFW窗口失败" << std::endl;
        glfwTerminate(); //终止函数
        return -1;
    }
    glfwMakeContextCurrent(window);

    /*
        GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之
        前我们需要初始化GLAD
        gladLoadGLLoader
        @param GLADloadproc 用来加载系统相关的OpenGL函数指针地址的函数
        @return glfwGetProcAddress 根据我们编译的系统定义了正确的函数
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化GLAD失败" << std::endl;
        return -1;
    }
    /*
        在我们开始渲染之前还有一件重要的事情要做，我们必须告诉OpenGL渲染
        窗口的尺寸大小，即视口(Viewport)，这样OpenGL才只能知道怎样根据
        窗口大小显示数据和坐标。我们可以通过调用glViewport函数来设置窗
        口的维度(Dimension)：
        glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参
        数控制渲染窗口的宽度和高度（像素）。
    */

    glViewport(0, 0, WIDTH, HEIGHT);
    //QUESTION:是否设置窗口修改似乎不影响视口的显示
    //注册修改窗口大小的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*  渲染循环
        glfwWindowShouldClose函数在我们每次循环的开始前检查一次
        GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结
        束了，之后为我们就可以关闭应用程序了。

        glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标
        移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法
        手动设置）。

        glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每
        一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作
        为输出显示在屏幕上。
    */
    while (!glfwWindowShouldClose(window))
    {
        //输入
        processInput(window);
        //渲染指令
        //do something
        /*
            glClearColor函数是一个状态设置函数，而glClear函数则
            是一个状态使用的函数，它使用了当前的状态来获取应该清除
            为的颜色。
        */
        glClearColor(.2f, .3f, .3f, 1.0f); //调用了glClearColor来设置清空屏幕所用的颜色
        /*
            可以通过调用glClear函数来清空屏幕的颜色缓冲，它接受一个
            缓冲位(Buffer Bit)来指定要清空的缓冲，可能的缓冲位有
            GL_COLOR_BUFFER_BIT， 颜色缓冲区
            GL_DEPTH_BUFFER_BIT， 深度缓冲区
            GL_STENCIL_BUFFER_BIT。模板缓冲区
            由于现在我们只关心颜色值，所以我们只清空颜色缓冲。
        */
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
        //检查并调用事件，交换缓冲区
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /*
        当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。
        我们可以在main函数的最后调用glfwTerminate函数来完成。
    */
    glfwTerminate();
    return 0;
}