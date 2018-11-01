#include <iostream> 
#include <thread> 
#define memset(a,b) memset(a,b,sizeof(a))
using namespace std;
int borad[9][9];
int tt;

void rowCheck(int row){
	bool hash[10];
	memset(hash,0);
	for(int j = 0;j < 9;j++){
		if(borad[row][j] > 9 || borad[row][j] < 0){
			return;
		}
		else if(hash[borad[row][j]] == true){
			return;			
		}
		else{
			hash[borad[row][j]] = true;
		}
	}
	tt++;
}
void colCheck(int col){
	bool hash[10];
	memset(hash,0);
	for(int i = 0;i < 9;i++){
		if(borad[i][col] > 9 || borad[i][col] < 0){
			return;
		}
		else if(hash[borad[i][col]] == true){
			return;			
		}
		else{
			hash[borad[i][col]] = true;
		}
	}
	tt++;
}
void gridCheck(int x, int y){
	x*=3;
	y*=3;
	bool hash[10];
	memset(hash,0);
	for(int i = x;i < x + 3;i++){	
		for(int j = y; j < y + 3;j++){
			if(borad[i][j] > 9 || borad[i][j] < 0){
				return;
			}
			else if(hash[borad[i][j]] == true){
				return;			
			}
			else{
				hash[borad[i][j]] = true;
			}
		}
	}
	tt++;
	// cout<<"grid\t"<<x<<"\t"<<y<<endl;
}
int main(){
	freopen("./in.txt","r",stdin);
	for(int i = 0;i < 9;i++){
		for(int j = 0; j < 9;j++){
			cin>>borad[i][j];
		}
	}
	tt = 0;
	thread trow[9];
	thread tcol[9];
	thread tgrid[9];
	
	for(int i = 0;i < 9;i++){
		trow[i] = thread(rowCheck, i); 
		tcol[i] = thread(colCheck, i); 
		tgrid[i] = thread(gridCheck, i/3, i%3);
	}
	
	for(int i = 0;i < 9;i++){
		trow[i].join();
		tcol[i].join();
		tgrid[i].join();
	}	
	
	cout<<((tt==27)?"True":"False")<<endl;
}