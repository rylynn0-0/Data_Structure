#include<bits/stdc++.h>
using namespace std;


map<int,int>mymap; 
queue<queue<int>* >myque;
queue<int> listque[15];
int main(){
	int n,m,x;
	string out;
	cin>>n; 
	for(int i=0;i<n;i++){
		cin>>m;
		for(int j=0;j<m;j++){
			cin>>x;
			mymap[x]=i;
			
		}
	}
	string str;
	cin>>str;
	while(str!="STOP"){
		
		if(str=="ENQUEUE"){
			cin>>x;
			if(listque[mymap[x]].empty()){
				listque[mymap[x]].push(x);
				myque.push(&listque[mymap[x]]);
			}else{
				listque[mymap[x]].push(x);
			}
		} else{
			int temp=myque.front()->front();
			myque.front()->pop();
			if(myque.front()->empty()){
				myque.pop();
			}
			out+=(" "+to_string(temp));
			
		}
		
		cin>>str;
	}
	cout<<out<<endl;
	 
	return 0;
} 
