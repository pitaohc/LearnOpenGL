#include "Shader.h"


std::string Shader::readShader(const char* path)
{
    std::string shaderCode; //存放shader代码
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        /*
        程序通过把源文件的流重定向到关联到目的文件的流对象，
        通过 fout<<fin.rdbuf();一句代码就完成了在C语言中
        的循环读写缓冲区的功能，而且C++中使用的是底层的流缓
        冲，效率更高。
        */
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ  " << e.what() << std::endl;
    }
    return shaderCode;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    uint32_t vertex, fragment;
    int status;
    char infoLog[512];
    std::string vertexCode = readShader(vertexPath);
    std::string fragmentCode = readShader(fragmentPath);
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* vertexCodeC = vertexCode.c_str();
    const char* fragmentCodeC = fragmentCode.c_str();
    glShaderSource(vertex, 1, &vertexCodeC, nullptr);
    glShaderSource(fragment, 1, &fragmentCodeC, nullptr);
    glCompileShader(vertex);
    glCompileShader(fragment);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    // 链接着色器
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // 打印连接错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    int valueLocation = glGetUniformLocation(this->ID, name.c_str());
    glUniform1i(valueLocation, value);
}

void Shader::setInt(const std::string& name, int value) const
{
    int valueLocation = glGetUniformLocation(this->ID, name.c_str());
    glUniform1i(valueLocation, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    int valueLocation = glGetUniformLocation(this->ID, name.c_str());
    glUniform1f(valueLocation, value);
}
