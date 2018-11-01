// usage: g++ -pthread -o pthread_merge_sort *.cpp -std=c++11 && ./pthread_merge_sort
#include <iostream> 
#include <pthread.h> 
#include <vector>
#define memset(a,b) memset(a,b,sizeof(a))
#if defined(__cplusplus)
extern "C"
#endif

using namespace std;
vector<int> v;

struct thread_args
 {
	int left;
    int right;
};

void *mergeSort(void* ptr){
	thread_args * Structthread = (thread_args *)ptr;
	int left = Structthread->left;
	int right = Structthread->right;
	if(left>= right){
		return NULL;
	}
	int mid = (left + right)/2;
	// Single thread version
/* 	mergeSort(left,mid);
	mergeSort(mid + 1,right); */
	
	pthread_t thread1, thread2;
	struct thread_args Structthread1;
	struct thread_args Structthread2;
	
	Structthread1.left = left;
	Structthread1.right = mid;
	Structthread2.left = mid+1;
	Structthread2.right = right;
	pthread_create( &thread1, NULL,mergeSort, (void *) &Structthread1);
	pthread_create( &thread2, NULL, mergeSort, (void *) &Structthread2);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL); 

	
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
	
	struct thread_args Structmain;
	Structmain.left = 0;
	Structmain.right = v.size() - 1;
	(*mergeSort)((void *) & Structmain);
	
	cout<<"after sorting:\t";
	for(int n:v){
		cout<<n<<" ";
	}
	cout<<endl;	
}