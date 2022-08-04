#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
private:
    std::string readShader(const char* path);
public:
    int ID; //shader program ���
    Shader(const char* vertexPath, const char* fragmentPath); //����Shader����
    void use(); //����shader����
    void setBool(const std::string& name,bool value)const; //����bool
    void setInt(const std::string& name,int value)const; //����int
    void setFloat(const std::string& name,float value)const; //����float
};
#endif

