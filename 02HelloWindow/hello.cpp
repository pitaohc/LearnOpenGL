#include<glad/glad.h>
#include<glfw3.h>
#include<iostream>
#include<time.h>
#include<Windows.h>
constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH = 800;
constexpr char TITLE[] = "LearnOpenGL";

/*
    �����С���º���
    ���´���ʱ��ͬʱ�����ӿڵĴ�С
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
    ���봦����
*/
void processInput(GLFWwindow* window)
{
    /*
        �����⵽����escape���ڰ���״̬�������ô����˳����Ϊtrue
    */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
void initGLFW()
{
    glfwInit(); //��ʼ��glfw
    /*
       glfwWindowHint(int hint, int value)����glfw
       @param hint ѡ�������
       @param value ѡ���ֵ
    */
    //����3.3�汾��OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //���汾��3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //�ΰ汾��3
    //ʹ�ú���ģʽGLFW_OPENGL_CORE_PROFILE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

}
/*
    OpenGLʹ������
    1. ��ʼ��GLFW
    2. �����������
    3. ʹ��GLAD�󶨺���
    4. �����ӿ�
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

        glfwCreateWindow ������������
        @param width ������
        @param height ����߶�
        @param title �������
        @return GLFWwindow* ���ش�������ָ��
        �����괰�����ǾͿ���֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳�����������
    */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "����GLFW����ʧ��" << std::endl;
        glfwTerminate(); //��ֹ����
        return -1;
    }
    glfwMakeContextCurrent(window);

    /*
        GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮
        ǰ������Ҫ��ʼ��GLAD
        gladLoadGLLoader
        @param GLADloadproc ��������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
        @return glfwGetProcAddress �������Ǳ����ϵͳ��������ȷ�ĺ���
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        return -1;
    }
    /*
        �����ǿ�ʼ��Ⱦ֮ǰ����һ����Ҫ������Ҫ�������Ǳ������OpenGL��Ⱦ
        ���ڵĳߴ��С�����ӿ�(Viewport)������OpenGL��ֻ��֪����������
        ���ڴ�С��ʾ���ݺ����ꡣ���ǿ���ͨ������glViewport���������ô�
        �ڵ�ά��(Dimension)��
        glViewport����ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ���
        ��������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
    */

    glViewport(0, 0, WIDTH, HEIGHT);
    //QUESTION:�Ƿ����ô����޸��ƺ���Ӱ���ӿڵ���ʾ
    //ע���޸Ĵ��ڴ�С�Ļص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*  ��Ⱦѭ��
        glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��
        GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ�����
        ���ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�

        glfwPollEvents���������û�д���ʲô�¼�������������롢���
        �ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص�����
        �ֶ����ã���

        glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿ
        һ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����
        Ϊ�����ʾ����Ļ�ϡ�
    */
    while (!glfwWindowShouldClose(window))
    {
        //����
        processInput(window);
        //��Ⱦָ��
        //do something
        /*
            glClearColor������һ��״̬���ú�������glClear������
            ��һ��״̬ʹ�õĺ�������ʹ���˵�ǰ��״̬����ȡӦ�����
            Ϊ����ɫ��
        */
        glClearColor(.2f, .3f, .3f, 1.0f); //������glClearColor�����������Ļ���õ���ɫ
        /*
            ����ͨ������glClear�����������Ļ����ɫ���壬������һ��
            ����λ(Buffer Bit)��ָ��Ҫ��յĻ��壬���ܵĻ���λ��
            GL_COLOR_BUFFER_BIT�� ��ɫ������
            GL_DEPTH_BUFFER_BIT�� ��Ȼ�����
            GL_STENCIL_BUFFER_BIT��ģ�建����
            ������������ֻ������ɫֵ����������ֻ�����ɫ���塣
        */
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
        //��鲢�����¼�������������
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /*
        ����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ��
        ���ǿ�����main������������glfwTerminate��������ɡ�
    */
    glfwTerminate();
    return 0;
}