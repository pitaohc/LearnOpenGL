#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
private:
    std::string readShader(const char* path);
public:
    int ID; //shader program 句柄
    Shader(const char* vertexPath, const char* fragmentPath); //构建Shader程序
    void use(); //调用shader程序
    void setBool(const std::string& name,bool value)const; //传入bool
    void setInt(const std::string& name,int value)const; //传入int
    void setFloat(const std::string& name,float value)const; //传入float
};
#endif

