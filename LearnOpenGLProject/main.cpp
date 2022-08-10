#include "head.h"
#include "Shader.h"
unsigned int HEIGHT = 800, WIDTH = 800;
std::string TITLE = u8"05 ����"; //�ַ���ǰu8ǰ׺��ʾutf-8���룬GBK���������
std::string vertexShaderPath = ".\\shader\\vertex.vert";
std::string fragmentShaderPath = ".\\shader\\fragment.frag";


UINT32 getFPS()
{
    static int fps = 0;
    static int lastTime = clock(); // ms
    static int frameCount = 0;
    constexpr int UNITTIME = 100; //����ʱ�䣬��λms
    ++frameCount;

    int curTime = clock();
    if (curTime - lastTime > UNITTIME) // ȡ�̶�ʱ����Ϊ1��
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
    ����ص���������Ӧ�����ź�
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//����esc�˳�����
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

//ʹ��uniform����һ��ˮƽƫ�������ڶ�����ɫ����ʹ�����ƫ�������������ƶ�����Ļ�Ҳࣺ
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
        std::cerr << "��������ʧ��" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window); //����������
    glfwSetKeyCallback(window, key_callback); //���ü��̻ص�����
    //��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "��ʼ��GLADʧ��" << std::endl;
        glfwTerminate();
        return 1;
    }

    //�����ӿ�
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

    //��������ͼƬ
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    int width, height, nrChannels;
    unsigned char* data = stbi_load("img/01/06/container.jpg", &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);    /*
    ��һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
    �ڶ�������Ϊ����ָ���༶��Զ����ļ��������ϣ�������ֶ�����ÿ���༶��Զ����ļ���Ļ�������������0��Ҳ���ǻ�������
    ��������������OpenGL����ϣ����������Ϊ���ָ�ʽ�����ǵ�ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��
    ���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶ȡ�����֮ǰ����ͼ���ʱ�򴢴������ǣ���������ʹ�ö�Ӧ�ı�����
    �¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩��
    ���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
    ���һ��������������ͼ�����ݡ�
    */

    UINT32 VAO, VBO, EBO;
    //0. ���ɶ������
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //1. �󶨶����������
    glBindVertexArray(VAO);
    //2. �Ѷ������鸴�Ƶ�һ�����㻺���У���OpenGL����
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //����VBOΪ���黺����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //����EBOΪ����������
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
        //1. ��Ӧ�¼�
        getFPS();
        //std::cout << "FPS: " << getFPS() << std::endl;
        glfwPollEvents();
        //2. ��Ⱦ
        //��ջ�����
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        glClearColor(51.0 / 255.0, 76.0 / 255.0, 76 / 255.0, 1.0f); //������glClearColor�����������Ļ���õ���ɫ
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //3. �л�������
        glfwSwapBuffers(window);

        //4. TODO ����֡��
        //��ʱû�нϺõĽ���취��sleep���ܴﵽĿ��֡�ʣ�ѭ���ӳ�ռ��CPU���Ҿ��Ȳ���
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;

}