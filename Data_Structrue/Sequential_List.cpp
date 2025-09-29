#include"Sequential_List.h"

Sequential_List::Sequential_List(int n) {
	max_len = n;
	length = 0;
	is_empty = 1;
	mylist = new int(n+1);
	is_init = 1;
}

bool Sequential_List::Destory() {
	if (!is_init) {return 0;}

	max_len = 0;
	length = 0;
	is_empty = 0;
	delete mylist;
	is_init = 0;
	return 1;

}

bool Sequential_List::Empty() {
	return is_empty;
}

int Sequential_List::Length() {
	return length;
}

int Sequential_List::GetElem(int index) {
	return mylist[index];
}

bool Sequential_List::Delete(int index) {
	if (index<=0 || index>length)return 0;
	for (int i = index; i <= length; i++) {
		mylist[i] = mylist[i + 1];
	}length--;
	if (length == 0)is_empty = 1;
	return 1;
}

bool Sequential_List::Insert(int index, int x) {
	if (index <= 0 || index > length+1)return 0;
	if (length + 1 > max_len)return 0;
	for (int i = length; i >= index; i--) {
		mylist[i + 1] = mylist[i];
	}mylist[index] = x;
	length++;
	is_empty = 0;
	return 1;
}

void Sequential_List::Clear() {
	length = 0;
	is_empty = 1;
	is_init = 1;
}

