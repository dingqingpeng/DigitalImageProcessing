#include<iostream>
#include<list>
#include<vector>
using namespace std;
float Data[] = {1,2,3,4,5,6,7,8};//进行FFT的数据
struct Complex{  float imaginary;  float real; };
Complex ComplexMulti( Complex One, Complex Two);   //复数乘法函数
Complex StoreResult[sizeof(Data) / sizeof(float)];    //存储计算的结果
void DataReRank(int num);
void FFT();
int DataLength = sizeof(Data) / sizeof(float);  //数据长度
const int PI = 3.1415926;
int main(){
	DataReRank(8);
	FFT();
	for (auto data : StoreResult) cout << sqrt(pow(data.imaginary, 2) + pow(data.real, 2))<<endl;
	return 0;
}
void DataReRank(int num){
	int BitNum = log2(num);
	int flag = 1, index = 0, flag2 = 0;
	list<int> Bit;   //存放二进制数，重复使用，每次用完需清零
	vector<float> DataTemp; //暂时存放重新排列过后的FFT数据
	for (int i = 0; i < num; i++){
		for (int j = 0; j < BitNum; j++){              //十进制转化为长度一致的二进制数
			int x = (i & flag)>0 ? 1 : 0;
			Bit.push_front(x);
			flag <<= 1;
		}
		for (auto it :Bit)  index += it*pow(2, flag2++); //将原数组的索引倒序
		DataTemp.push_back(Data[index]);
		Bit.clear();
		flag = 1; flag2 = 0; index = 0;
	}
	for (int i = 0; i < num; i++) Data[i]=DataTemp[i];  //将DataTemp中数据复制到Data
}
Complex ComplexMulti(Complex One, Complex Two){
	Complex Temp;
	Temp.imaginary = One.imaginary *Two.real + One.real*Two.imaginary;
	Temp.real = One.real*Two.real - One.imaginary*Two.imaginary;
	return Temp;
}
void FFT(){
	int Level = log2(DataLength);      //需要运算的级数
	Complex Temp,up;
	//先将数据转移到复数结构里
	for (int i = 0; i < DataLength; i++) StoreResult[i].real = Data[i];
	for (int i = 1; i <= Level; i++){
		Complex Factor;  //定义旋转因子
		int BowknotDis = 2 << (i - 1);     //没有交叉的蝶形结的距离
		int CalDis = BowknotDis / 2;   //同一蝶形计算中两个数字的距离
		for (int j = 0; j < CalDis; j++) {   //每一级蝶形运算中有CalDis个不同旋转因子
			Factor.real = cos(2 * PI /pow(2,i)*j);
			Factor.imaginary = -sin(2 * PI /pow(2,i)*j);
			for (int k = j; k < DataLength - 1; k += BowknotDis){
				Temp = ComplexMulti(Factor, StoreResult[k + CalDis]);
				up.imaginary = StoreResult[k].imaginary + Temp.imaginary;
				up.real = StoreResult[k].real + Temp.real;
				StoreResult[k + CalDis].imaginary = StoreResult[k].imaginary - Temp.imaginary;
				StoreResult[k + CalDis].real = StoreResult[k].real - Temp.real;
				StoreResult[k] = up;
			}
		}
	}
}