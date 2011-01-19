#include "utils.h"
#include <math.h>



//--------------------------itos------------------------------------
//	converts an integer to a string
//------------------------------------------------------------------		
string itos(int arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}


//--------------------------ftos------------------------------------
//	converts a float to a string
//------------------------------------------------------------------		
string ftos(float arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}
//-------------------------------------Clamp()-----------------------------------------
//
//	clamps the first argument between the second two
//
//-------------------------------------------------------------------------------------
void Clamp( double &arg, float min, float max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}
///////////////////////////////
//
// a & b determine a line .the arg dist is distance of point c to point a
/////////////////////
void GetPointFromLine(SPoint &c ,const SPoint &a,const SPoint &b,float dist)
{
		float _dist=(float)abs(dist);	

		float k=(a.y-b.y)/(a.x-b.x);
		float bb=a.y-k*(a.x);
		float A=(1+k*k);
		float B=-(2*a.x+2*a.y*k-2*k*bb);
		float C=a.x*a.x+bb*bb-2*a.y*bb+a.y*a.y-_dist*_dist;

		float X1=(float) ((-B)+sqrt(B*B-4*A*C))/(2*A);
		float Y1=k*X1+bb;

		float X2=(float) ((-B)+sqrt(B*B-4*A*C))/(2*A);
		float Y2=k*X2+bb;
	
		if(dist>0)
		{
			c.x=X1;
			c.y=Y1;
		}else
		{
			c.x=X2;
			c.y=Y2;
		}
		
	
}

//has konowed k,b
void GetPointFormLine(SPoint &c,const SPoint &a,const float &k,const float &b,float dist)
{
	float _dist = (float)abs(dist);
		float A=(1+k*k);
		float B=-(2*a.x+2*a.y*k-2*k*b);
		float C=a.x*a.x+b*b-2*a.y*b+a.y*a.y-_dist*_dist;

		float X1=(float) ((-B)+sqrt(B*B-4*A*C))/(2*A);
		float Y1=k*X1+b;

		float X2=(float) ((-B)-sqrt(B*B-4*A*C))/(2*A);
		float Y2=k*X2+b;

		if(dist>0)
		{
			c.x=X1;
			c.y=Y1;
		}else
		{
			c.x=X2;
			c.y=Y2;
		}
		
}

	bool Equal(float f1, float f2) 
	{
		return (abs(f1 - f2) < 1e-4f);
	}
	//�ж������Ƿ����
	bool operator==(const SPoint &p1, const SPoint &p2) 
	{
		return (Equal(p1.x, p2.x) && Equal(p1.y, p2.y));
	}

	//�Ƚ����������С���ȱȽ�x���꣬����ͬ��Ƚ�y����
	bool operator> (const SPoint &p1, const SPoint &p2) 
	{
		return (p1.x > p2.x || (Equal(p1.x, p2.x) && p1.y > p2.y));
	}

	//�������������
	float operator^(const SPoint &p1, const SPoint &p2)
	{
		return (p1.x * p2.y - p1.y * p2.x);
	}





//�ж����߶�λ�ù�ϵ�����������(�������)��
//���غϣ���ȫ�غ�(6)��1���˵��غ��ҹ���(5)�������غ�(4)
//���غϣ����˵��ཻ(3)����������(2)������(1)���޽�(0)����������(-1)
int Intersection(SPoint p1, SPoint p2, SPoint p3, SPoint p4, SPoint &Int) {
	//��֤����p1!=p2��p3!=p4
	if (p1 == p2 || p3 == p4) {
		return -1; //����-1����������һ���߶���β�غϣ����ܹ����߶�
	}
	//Ϊ�������㣬��֤���߶ε������ǰ���յ��ں�
	if (p1 > p2) {
		swap(p1, p2);
	}
	if (p3 > p4) {
		swap(p3, p4);
	}
	//������߶ι��ɵ�����
	SPoint v1 (p2.x - p1.x, p2.y - p1.y);
	SPoint  v2 (p4.x - p3.x, p4.y - p3.y);
	//�������������ƽ��ʱ���Ϊ0
	float Corss = v1 ^ v2;
	//�ж����߶��Ƿ���ȫ�غ�
	if (p1 == p3 && p2 == p4) {
		return 6;
	}
	//�������غ�
	if (p1 == p3) {
		Int = p1;
		//����غ��ҹ���(ƽ��)����5����ƽ�����ڶ˵㣬����3
		return (Equal(Corss, 0) ? 5 : 3);
	}
	//����յ��غ�
	if (p2 == p4) {
		Int = p2;
		//�յ��غ��ҹ���(ƽ��)����5����ƽ�����ڶ˵㣬����3
		return (Equal(Corss, 0) ? 5 : 3);
	}
	//������߶���β����
	if (p1 == p4) {
		Int = p1;
		return 3;
	}
	if (p2 == p3) {
		Int = p2;
		return 3;
	}//���������жϣ���β�����ص���������ų���
	//���߶ΰ���������������߶�1�����ϴ������߶ν���
	if (p1 > p3) {
		swap(p1, p3);
		swap(p2, p4);
		//����ԭ�ȼ���������������
		swap(v1, v2);
		Corss = v1 ^ v2;
	}
	//�������߶�ƽ�е����
	if (Equal(Corss, 0)) {
		//������v1(p1, p2)��vs(p1,p3)��������ж��Ƿ���
		SPoint vs (p3.x - p1.x, p3.y - p1.y);
		//���Ϊ0����ƽ���߶ι��ߣ������ж��Ƿ����غϲ���
		if (Equal(v1 ^ vs, 0)) {
			//ǰһ���ߵ��յ���ں�һ���ߵ���㣬���ж������غ�
			if (p2 > p3) {
				Int = p3;
				return 4; //����ֵ4�����߶β����غ�
			}
		}//�����㲻���ߣ���������ƽ���߶αز����ߡ�
		//�����߻��ߵ����غϵ�ƽ���߾��޽���
		return 0;
	} //����Ϊ��ƽ�е�������Ƚ��п����ų�����
	//x���������򣬿�ֱ�ӱȽϡ�y����Ҫ�������߶ε�������Сֵ
	float ymax1 = p1.y, ymin1 = p2.y, ymax2 = p3.y, ymin2 = p4.y;
	if (ymax1 < ymin1) {
		swap(ymax1, ymin1);
	}
	if (ymax2 < ymin2) {
		swap(ymax2, ymin2);
	}
	//��������߶�Ϊ�Խ��ߵľ��β��ཻ�����޽���
	if (p1.x > p4.x || p2.x < p3.x || ymax1 < ymin2 || ymin1 > ymax2) {
		return 0;
	}//������п�������
	SPoint vs1  (p1.x - p3.x, p1.y - p3.y);
	SPoint vs2  (p2.x - p3.x, p2.y - p3.y);
	SPoint vt1  (p3.x - p1.x, p3.y - p1.y);
	SPoint vt2  (p4.x - p1.x, p4.y - p1.y);

	float s1v2, s2v2, t1v1, t2v1;
	//�����������ж���������
	if (Equal(s1v2 = vs1 ^ v2, 0) && p4 > p1 && p1 > p3) {
		Int = p1;
		return 2;
	}
	if (Equal(s2v2 = vs2 ^ v2, 0) && p4 > p2 && p2 > p3) {
		Int = p2;
		return 2;
	}
	if (Equal(t1v1 = vt1 ^ v1, 0) && p2 > p3 && p3 > p1) {
		Int = p3;
		return 2;
	}
	if (Equal(t2v1 = vt2 ^ v1, 0) && p2 > p4 && p4 > p1) {
		Int = p4;
		return 2;
	} //δ�������ϣ����ж��Ƿ��ཻ
	if(s1v2 * s2v2 > 0 || t1v1 * t2v1 > 0) {
		return 0;
	} //����Ϊ�ཻ��������㷨����ĵ�
	//�����������ʽ������������
	float ConA = p1.x * v1.y - p1.y * v1.x;
	float ConB = p3.x * v2.y - p3.y * v2.x;
	//��������ʽD1��D2��ֵ������ϵ������ʽ��ֵ���õ���������
	Int.x = (ConB * v1.x - ConA * v2.x) / Corss;
	Int.y = (ConB * v1.y - ConA * v2.y) / Corss;
	//��������1
	return 1;
}

void DrawLine(HDC surface,const SPoint& a,const SPoint &b)
{
		MoveToEx(surface,(LONG)a.x,(LONG)a.y,NULL);
		LineTo(surface,(LONG)b.x,(LONG)b.y);
}