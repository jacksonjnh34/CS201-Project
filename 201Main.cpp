using namespace std;
#include <iostream>
#include "CircularDynamicArray.cpp"
#define DUMP(C) cout << "size is : " << C.length() << endl << "capacity is : " << C.capacity() << endl; for (int i=0; i< C.length();i++) cout << C[i] << " ";  cout << endl << endl;

int main()
{
	CircularDynamicArray<int> C(10);
	for (int i=0; i< C.length();i++) C[i] = i;
	DUMP(C)
	C.delFront();
	DUMP(C)
	C.delEnd();
	DUMP(C)
	C.addEnd(100);
	DUMP(C)
	C.addEnd(101);
	DUMP(C)
	C.delEnd(); C.delEnd();
	C.addFront(-100); C.addFront(-200);
	DUMP(C)
	C.addFront(37);
	DUMP(C)
	
	for (int i=0; i< 20;i++) C.addEnd(i*i);
	for (int i=0; i< 20;i++) C.addFront(-(i*i));
	DUMP(C)
	
	for (int i=0; i< 25;i++) C.delEnd();
	for (int i=0; i< 10;i++) C.delFront();
	DUMP(C)
	
}