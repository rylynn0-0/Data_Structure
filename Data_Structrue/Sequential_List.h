#pragma once

#include<vector>
class Sequential_List {
	int* mylist;
	int length;
	int max_len;
	bool is_empty;
	bool is_init;

public:
	Sequential_List(int n);//��ʼ����������Ϊn�����Ա�
	bool Destory();//ɾ�����Ա�
	bool Empty();//���ص�ǰ���Ա��Ƿ�Ϊ��
	int Length();//�������Ա�ĳ���
	int GetElem(int index);//��ȡ��index��Ԫ��
	bool Delete(int index);//ɾ����index��Ԫ��
	bool Insert(int index,int x);//��indexλ�ò���Ԫ��x
	void Clear();

};
