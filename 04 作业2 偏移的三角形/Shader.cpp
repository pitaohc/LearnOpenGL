#include "Shader.h"


std::string Shader::readShader(const char* path)
{
    std::string shaderCode; //���shader����
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        /*
        ����ͨ����Դ�ļ������ض��򵽹�����Ŀ���ļ���������
        ͨ�� fout<<fin.rdbuf();һ�������������C������
        ��ѭ����д�������Ĺ��ܣ�����C++��ʹ�õ��ǵײ������
        �壬Ч�ʸ��ߡ�
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
    // ��ӡ�����������еĻ���
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
    // ������ɫ��
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // ��ӡ���Ӵ�������еĻ���
    glGetProgramiv(ID, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
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
