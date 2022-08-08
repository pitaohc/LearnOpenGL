#version 330 core
layout(location = 0) in vec3 aPos; //位置
layout(location = 1) in vec3 aColor; //位置
uniform vec3 deltaPos;
out vec3 vertexColor; //颜色输出
void main()
{
    gl_Position = vec4( (aPos.x+deltaPos.x)/(aPos.z+deltaPos.z+2),
                        (aPos.y+deltaPos.y)/(aPos.z+deltaPos.z+2),
                        aPos.z+deltaPos.z, 
                        1.0);
    
    vertexColor = aColor; //输出颜色给片段着色器
    /*
    回答问题：你知道为什么左下角是黑色的吗？
    ----------------------------------------------------------------------
    想一想：我们片段颜色的输出等于三角形的（插值）坐标。 我们三角形左下角的坐标是多少？
    这是（-0.5f，-0.5f，0.0f）。 由于 xy 值为负，它们被限制为 0.0f 的值。 这一直发
    生到三角形的中心边，因为从该点开始，值将再次被正插值。 0.0f 的值当然是黑色的，这
    解释了三角形的黑色边。

    负值会变为0，猜想：如果大于1则会变为1
    */
}