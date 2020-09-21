#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;

double max(double a, double b)
{
    return a>b ? a : b;
}

double mean(vector<double> & v)
{
    double sum=0.0;
    for (auto it : v)
        sum+=it;
    return sum/v.size();
}

class RNG{
public:
   virtual double rand() const=0;
   virtual ~RNG() {};
};

class RUNG: public RNG {
public:
    double rand() const {
        std::mt19937 MersenneTwister;
        uniform_real_distribution<double> generator(0.0, 1.0);
        return generator(MersenneTwister);
    }
    ~RUNG() {};
};

class RENG: public RNG {
public:
    RENG(double para): _para(para) {};
    double rand() const {
        std::mt19937 MersenneTwister;
        exponential_distribution<double> generator(_para);
        return generator(MersenneTwister);
    }
    ~RENG() {};
private:
    double _para;
};

//class MyVector {
//public:
//    MyVector(): _size(0), _ptr(0) {};
//    MyVector(int size): _size(size), _ptr(new int[size]()) {};
//    MyVector(MyVector &mv): _size(mv._size), _ptr(new int[mv._size]()) {memcpy(_ptr, mv._ptr, mv._size);};
//    int &operator[] (int index) {return _ptr[index];};
//    MyVector operator+ (MyVector &mv) {
//        MyVector new_mv(_size+mv._size);
//        return new_mv;
//    };
//    ~MyVector() {delete [] _ptr;};
//private:
//    int _size;
//    int *_ptr;
//};

//class MyVector1 {
//public:
//    void push_back(int val) {
//        if (_size<_capacity) {
//            _ptr[_size++]=val;
//        }
//        else {
//            _capacity*=2;
//            int *new_ptr=new int[_capacity];
//            memcpy(new_ptr, _ptr, _size);
//            delete [] _ptr;
//            _ptr=new_ptr;
//            _ptr[_size++]=val;
//        }
//    };
//private:
//    int _capacity;
//};
void swap(double &a, double &b)
{
    double tmp = a;
    a = b;
    b = tmp;
    return;
}

double sum(const vector<double>& v)
{
    //assert(v.size()>0);
    double m=0.0;
    for (int i=0;i<v.size();i++)
       m+=v[i];
    return m;
}

class car{
public:
virtual double run() const=0;
virtual ~car(){};
};

long long computeFactorial_for(int n)
{
    //assert(n>0);
    long long ret = 1;
    for (; n>0; n--)
        ret *= n;
    return ret;
}

long long computeFactorial_while(int n)
{
    //assert(n>0);
    long long ret = 1;
    while (n>0)
        ret *= n--;
    return ret;
}

long long computeFactorial_dowhile(int n)
{
    //assert(n>0);
    long long ret = 1;
    do {
        ret *= n--;
    } while (n>0);
    return ret;
}


double computeSimpsonRule(double a, double b, int n)
{
    #define f(a, b) (pow(a, -(b*b/2)))
    const double EXP = 2.7183;

    //assert(n>0);
    int sign = 1;
    if (a==b) return 0;
    if (a>b) {
        swap(a,b);
        sign= -1;
    };

    double h = (b-a)/n;
    int j=1;
    double x_j = a;
    double res = f(EXP, x_j);
    while (j<n) {
        int pre = j%2==0 ? 2 : 4;
        x_j += h;
        res += pre * f(EXP, x_j);
    }
    res += f(EXP, x_j+h);
    return sign*res*h/3;
}

//int main1() {
//    double arr[]={1.2, 2.2};
//    vector<double> v1(arr, arr+2);
//    cout<<mean(v1)<<endl;
//
//    RUNG ru;
//    RENG re(2.2);
//    MyVector mm(2);
//    cout<<ru.rand()<<endl;
//    cout<<re.rand()<<endl;
//    mm[0]=1;
//    cout<<mm[0]<<endl;
//    return 0;
//}
