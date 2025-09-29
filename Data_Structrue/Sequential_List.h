#pragma once

#include<vector>
class Sequential_List {
	int* mylist;
	int length;
	int max_len;
	bool is_empty;
	bool is_init;

public:
	Sequential_List(int n);//初始化创建长度为n的线性表
	bool Destory();//删除线性表
	bool Empty();//返回当前线性表是否为空
	int Length();//返回线性表的长度
	int GetElem(int index);//获取第index个元素
	bool Delete(int index);//删除第index个元素
	bool Insert(int index,int x);//在index位置插入元素x
	void Clear();

};
