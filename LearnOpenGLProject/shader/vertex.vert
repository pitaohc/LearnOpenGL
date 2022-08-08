#version 330 core
layout(location = 0) in vec3 aPos; //λ��
layout(location = 1) in vec3 aColor; //λ��
uniform vec3 deltaPos;
out vec3 vertexColor; //��ɫ���
void main()
{
    gl_Position = vec4( (aPos.x+deltaPos.x)/(aPos.z+deltaPos.z+2),
                        (aPos.y+deltaPos.y)/(aPos.z+deltaPos.z+2),
                        aPos.z+deltaPos.z, 
                        1.0);
    
    vertexColor = aColor; //�����ɫ��Ƭ����ɫ��
    /*
    �ش����⣺��֪��Ϊʲô���½��Ǻ�ɫ����
    ----------------------------------------------------------------------
    ��һ�룺����Ƭ����ɫ��������������εģ���ֵ�����ꡣ �������������½ǵ������Ƕ��٣�
    ���ǣ�-0.5f��-0.5f��0.0f���� ���� xy ֵΪ�������Ǳ�����Ϊ 0.0f ��ֵ�� ��һֱ��
    ���������ε����ıߣ���Ϊ�Ӹõ㿪ʼ��ֵ���ٴα�����ֵ�� 0.0f ��ֵ��Ȼ�Ǻ�ɫ�ģ���
    �����������εĺ�ɫ�ߡ�

    ��ֵ���Ϊ0�����룺�������1����Ϊ1
    */
}