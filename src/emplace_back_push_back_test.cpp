#include <iostream>
#include <vector>

using namespace std;

class A {
public:
	A() { cout<<"A constructor"<<endl;}
	A(int i) {cout<<"i constrctor"<<endl;}
	A(const A &a) {cout<<"A copy constructor"<<endl;}	
private:
	int i;
};

//测试push_back和emplace_back区别
int main(int argc, char **argv)
{
	A b;
	vector<int> v;

	v.push_back(5);
//	v.emplace_back(b);
	return 0;

}
