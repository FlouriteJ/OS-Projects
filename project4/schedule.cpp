#include<iostream> 	//cout.flags(ios::fixed);cout.precision(2);
#include<string.h>
#include<cmath>
#include<algorithm>
#include<string>
#include<climits>
#include<vector>
#include<cstdio>
#include<stack>
#define memset(a,b) memset(a,b,sizeof(a))
typedef long long ll;
using namespace std;

struct task{
	string name;
	int priority;
	int burst;
	task(string n,int p,int b){
		name = n;
		priority = p;
		burst = b;
	}
	task(const task & t){
		name = t.name;
		priority = t.priority;
		burst = t.burst;
	}
};

vector<task> tasks;

bool cmp(task x,task y)
{
    return x.burst<y.burst;
}

bool cmpp(task x,task y)
{
    return x.priority>y.priority;
}

class CPU{
	int time = 0;
public:	
	void init(){
		time = 0;
	}
	void burst(int b,string name){
		cout<<"from "<<time<<" to "<<(time+b)<<" : "<<name<<endl;
		time+=b;
	}
}cpu;
int main(){
	#ifndef ONLINE_JUDGE
	freopen("./schedule.txt","r",stdin);
	#endif
	string name;
	int priority;
	int burst;
	while(cin>>name){
		name.pop_back();
		cin.get();
		cin>>priority;
		cin.get();
		cin>>burst;

		cout<<name;
		printf(" %d %d\n",priority,burst);
		tasks.push_back(task(name,priority,burst));
	}
	
	cout<<"FCFS"<<endl;
	vector<task> tasks_fcfs = tasks;
	cpu.init();
	for(int i = 0;i<tasks.size();i++){
		cpu.burst(tasks_fcfs[i].burst,tasks_fcfs[i].name);
	}

	cout<<"SJF"<<endl;
	vector<task> tasks_sjs = tasks;
	sort(tasks_sjs.begin(),tasks_sjs.end(),cmp);
	cpu.init();
	for(int i = 0;i<tasks.size();i++){
		cpu.burst(tasks_sjs[i].burst,tasks_sjs[i].name);
	}	
	
	cout<<"PS"<<endl;
	vector<task> tasks_ps = tasks;
	sort(tasks_ps.begin(),tasks_ps.end(),cmpp);
	cpu.init();
	for(int i = 0;i<tasks.size();i++){
		cpu.burst(tasks_ps[i].burst,tasks_ps[i].name);
	}
	
	cout<<"RR"<<endl;
	vector<task> tasks_rr = tasks;
	cpu.init();
	int flag = 0;
	int index = 0;
	while(flag<tasks.size()){
		if(index == tasks.size()){
			index = 0;
		}
		if(tasks_rr[index].burst>0){
			if(tasks_rr[index].burst-10>0){
				cpu.burst(10,tasks_rr[index].name);
				tasks_rr[index].burst -= 10;
			}
			else{
				cpu.burst(tasks_rr[index].burst,tasks_rr[index].name);
				tasks_rr[index].burst = 0;
				flag++;
			}
		}
		index++;
	}
	
	cout<<"PRR"<<endl;
	vector<task> tasks_prr = tasks;
	sort(tasks_prr.begin(),tasks_prr.end(),cmpp);
	cpu.init();
	int begin = 0;
	int end = 0;
	int temp_p = tasks_prr[0].priority;
	for(int i = 0;i<=tasks.size();i++){		
		if(i!=tasks.size() && tasks_prr[i].priority == temp_p){
			end = i;
		}
		else{
			int flag = 0;
			int index = begin;
			while(flag<-begin+end+1){
				if(index == end + 1){
					index = begin;
				}
				if(tasks_prr[index].burst>0){
					if(tasks_prr[index].burst-10>0){
						cpu.burst(10,tasks_prr[index].name);
						tasks_prr[index].burst -= 10;
					}
					else{
						cpu.burst(tasks_prr[index].burst,tasks_prr[index].name);
						tasks_prr[index].burst = 0;
						flag++;
					}
				}
				index++;
			}			
			begin = i;
			end = i;
			temp_p = tasks_prr[i].priority;
		}
	}	
}
