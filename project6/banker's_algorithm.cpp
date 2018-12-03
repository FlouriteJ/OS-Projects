#include<iostream> 	//cout.flags(ios::fixed);cout.precision(2);
#include<fstream>
#include<string.h>
#include<cmath>
#include<algorithm>
#include<string>
#include<climits>
#include<vector>
#include<cstdio>
#include<stack>
#define memset(a,b) memset(a,b,sizeof(a))
#define memcpy(a,b) memcpy(a,b,sizeof(b))
typedef long long ll;
using namespace std;

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

bool safe(){
	bool finish[NUMBER_OF_CUSTOMERS];
	int work[NUMBER_OF_RESOURCES];
	memset(finish,0);
	memcpy(work,available);
	int count = 0;
	while(count<NUMBER_OF_CUSTOMERS){
		bool exists = false;
		for(int i = 0;i<NUMBER_OF_CUSTOMERS;i++){
			if(finish[i]==true) continue;
			int token = true;
			for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
				if(need[i][j] > work[j]){
					token = false;
					break;
				}
			}
			
			if(token){
				finish[i] = true;
				exists = true;
				count++;
				for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
					work[j]+=allocation[i][j];
				}
			}
		}
		if (!exists){
			break;
		}
	}
	return count==NUMBER_OF_CUSTOMERS;
}

int request_resources(int customer_num, int request[]){
	for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
		if(request[j]>available[j]){
			 return 1;
		}
	}

	for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
		available[j]-=request[j];
		allocation[customer_num][j]+=request[j];
		need[customer_num][j]=(maximum[customer_num][j] - allocation[customer_num][j]>=0)?(maximum[customer_num][j] - allocation[customer_num][j]):0;
	}
	if (safe()){
		return 0;
	}
	else{
		for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
			available[j]+=request[j];
			allocation[customer_num][j]-=request[j];
			need[customer_num][j]=(maximum[customer_num][j] - allocation[customer_num][j]>=0)?(maximum[customer_num][j] - allocation[customer_num][j]):0;
		}
		return 1;
	}
}
int release_resources(int customer_num, int release[]){
	for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
		if(release[j]>allocation[customer_num][j]){
			 return -1;
		}
	}
	for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
		available[j]+=release[j];
		allocation[customer_num][j]-=release[j];
		need[customer_num][j]=(maximum[customer_num][j] - allocation[customer_num][j]>=0)?(maximum[customer_num][j] - allocation[customer_num][j]):0;
	}
	return 0;
}

int main(){
	ifstream infile;
	infile.open("./input.txt");
	memset(available,0);
	for(int i = 0;i<NUMBER_OF_CUSTOMERS;i++){
		for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
			infile>>maximum[i][j];
			allocation[i][j] = 0;
			need[i][j] = maximum[i][j];
			cout<<maximum[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"----------"<<endl;
	cout<<"Input: # resources"<<endl;
	for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
		cin>>available[j];
	}
	if(!safe()){
		cout<<"Not safe!"<<endl;
		return 0;
	}
	cout<<"----------"<<endl;
	infile.close();
	
	string first;
	int customer_num;
	int arr[NUMBER_OF_RESOURCES];
	while(true){
		cout<<"Input: command"<<endl;
		cin>>first;
		if (first == "*"){
			cout<<"available matrix"<<endl;
			for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
				cout<<available[j]<<" ";
			}
			cout<<endl;
			cout<<"maximum matrix"<<endl;
			for(int i = 0;i<NUMBER_OF_CUSTOMERS;i++){
				for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
					cout<<maximum[i][j]<<" ";		
				}
				cout<<endl;
			}				
			cout<<"allocation matrix"<<endl;
			for(int i = 0;i<NUMBER_OF_CUSTOMERS;i++){
				for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
					cout<<allocation[i][j]<<" ";		
				}
				cout<<endl;
			}				
			cout<<"need matrix"<<endl;
			for(int i = 0;i<NUMBER_OF_CUSTOMERS;i++){
				for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
					cout<<need[i][j]<<" ";		
				}
				cout<<endl;
			}				
		}
		else if (first == "RQ"){
			cin>>customer_num;
			for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
				cin>>arr[j];
			}
			request_resources(customer_num,arr);
		}
		else if (first == "RL"){
			cin>>customer_num;
			for(int j = 0;j<NUMBER_OF_RESOURCES;j++){
				cin>>arr[j];
			}
			release_resources(customer_num,arr);			
		}
		cout<<"----------"<<endl;
	}
}