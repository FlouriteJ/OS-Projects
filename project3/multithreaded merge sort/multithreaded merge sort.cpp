#include <iostream> 
#include <thread> 
#include <vector>
#define memset(a,b) memset(a,b,sizeof(a))
using namespace std;
vector<int> v;

void mergeSort(int left, int right){
	if(left>= right){
		return;
	}
	int mid = (left + right)/2;
	// Single thread version
/* 	mergeSort(left,mid);
	mergeSort(mid + 1,right); */
	thread tleft(mergeSort,left,mid);
	thread tright(mergeSort,mid + 1,right);
	tleft.join();
	tright.join();
	
	int i = left;
	int j = mid + 1;
	vector<int> arr(right - left + 1,0);
	int t = 0;
/* 	cout<<"start"<<endl;
	for(int p = left;p<= right; p++){
		cout<<v[p]<<" ";
	}
	cout<<endl; */
	while(i <= mid || j <= right){
		if (j > right || i <= mid && v[i] < v[j] ){
			arr[t++] = v[i]; 
			i++;
		}
		else{
			arr[t++] = v[j];
			j++;
		}	
/* 		for(int p = 0;p<=right - left; p++){
			cout<<arr[p]<<" ";
		}
		cout<<endl;	 */
	}
	
	for(i = left;i<=right;i++){
		v[i] = arr[i - left];
	}
/* 	cout<<"end"<<endl;
	for(int p = left;p<= right; p++){
		cout<<v[p]<<" ";
	}
	cout<<endl;	 */
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
	mergeSort(0,v.size() - 1);
	
	cout<<"after sorting:\t";
	for(int n:v){
		cout<<n<<" ";
	}
	cout<<endl;	
}