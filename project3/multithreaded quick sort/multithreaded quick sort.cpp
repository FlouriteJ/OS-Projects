#include <iostream> 
#include <thread> 
#include <vector>
#define memset(a,b) memset(a,b,sizeof(a))
using namespace std;
vector<int> v;

void quickSort(int left, int right){
	if(left>= right){
		return;
	}
	int p = left; //pivot
	int i = left;
	int j = right;

	while(i<j){
		while(i<j && v[j]>v[p]){
			j--;
			
		}
		if(i<j){
			int tmp = v[p];
			v[p] = v[j];
			v[j] = tmp;
		}
		while(i<j && v[i]<v[p]){
			i++;
		}
		if(i<j){
			int tmp = v[p];
			v[p] = v[i];
			v[i] = tmp; 
		}		
	}
	// Single thread version
/* 	quickSort(left,mid);
	quickSort(mid + 1,right); */
	thread tleft(quickSort,left,p - 1);
	thread tright(quickSort,p + 1,right);
	tleft.join();
	tright.join();
}

int main(){
	freopen("./in.txt","r",stdin);
	int tmp;
	while(cin>>tmp){
		v.push_back(tmp);
	}
	cout<<"before sorting:\t";
	for(int n:v){
		cout<<n<<" ";
	}
	cout<<endl;
	quickSort(0,v.size() - 1);
	
	cout<<"after sorting:\t";
	for(int n:v){
		cout<<n<<" ";
	}
	cout<<endl;	
}