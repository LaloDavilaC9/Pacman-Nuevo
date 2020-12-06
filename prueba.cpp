#include <iostream>
#include <cstring>
using namespace std;
int main(){
	string hola;
	cin>>hola;
	cout<<hola.size();
	char p[24];
	strcpy(p,"hola brayan");
	hola=p;
	cout<<hola<<endl;
}
