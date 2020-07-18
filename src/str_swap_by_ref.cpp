#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <map>
using namespace std;
void swap(char *& str1, char *& str2);
int main(int argc, char **argv)
{
	char *ap = "aa";
	char *bp = "bb";
	swap(ap, bp);
	cout<<ap<<endl<<bp<<endl;
	return 0;
}

void swap(char *& str1, char *& str2)
{
	char *temp = str1;
	str1=str2;
	str2=temp;

}


