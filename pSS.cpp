#include<iostream>
#include<vector>
#include<random>
#include<ctime>
#include<fstream>
#include<climits>
#include<thread>
#include<mutex>
#include<algorithm>
#include<cstdio>
#include<ctype.h>
#include<cfloat>
#include<sstream>
#include<cstring>
#include<cmath>
#include<bitset>
#include<iomanip>
#include<ios>
#include<numeric>
#include <omp.h>
using namespace std;
random_device                  rand_dev;
mt19937                        gen(rand_dev());
void randPermutation(vector<int>& O, int n){
	
    int i;
	#pragma omp parallel for
	for(i = 0;i < n;i++)
		O[i] = i;
	
	shuffle(O.begin(),O.end(),gen);
    }
    
int greedyColoring(vector<vector<int>>& A, vector<int>& O, vector<int>& C){
        
        int n = C.size();
        int x,i,j;
	
        
        #pragma omp parallel for
        for( x = 0; x < n;x++)
            C[x] = 0;
	

        vector<bool> N_v(n);
        
        int max_c = (n > 0);
            
        for(i = 0;i < n; i++){
            int v = O[i];
            C[v] = 1;

           
            #pragma omp parallel  for 
            for(x = 0; x < i+1;x++)
                N_v[x] = 0;


            
            for(j = 0; j < A[v].size();j++){
                if(C[A[v][j]] != 0)
                    N_v[C[A[v][j]] -1] = 1;
                }
	    		
            
            for(int j = 0;j < i+1;j++)
                if(N_v[j] == 0){
                    C[v] = j+1;
                    max_c = max(C[v], max_c);
                    break;
                    }
        }
        return max_c;
    }
    
int fast_recoloring(vector<vector<int>>& A, vector<int>& O, vector<int>& C, int max_c){
        vector<int> I(max_c), It(max_c), Ot(max_c);
        
	
        #pragma omp  parallel for 
        for(int i = 0; i < max_c;i++){
			It[i] = I[i] = 0;
			}
	
        int n = C.size();
        randPermutation(Ot, max_c);
       
       
	
        #pragma omp  parallel for
        for(int i = 0; i < n;i++){
            I[Ot[C[i] - 1]] += 1;
            }
	
        for(int i = 1, sum = I[0]; i < max_c;i++){
            sum += I[i];
            I[i] = sum - I[i];
            }
        I[0] = 0;
        for(int i = 0;i < n;i++){
            int c = Ot[C[i] - 1];
            O[I[c] + It[c]] = i;
            It[c]++;
            }
        return greedyColoring(A, O, C);
    }
    
int SS(vector<vector<int>>& A,  int n){
        vector<int> O(n),C(n);
        randPermutation(O, n);
        int max_c = greedyColoring(A, O, C);
        
        int t = max_c;
        while(1){
            max_c = fast_recoloring(A, O, C, max_c);
            if(max_c < t){
				cout << max_c << endl;	
                t = max_c;
            }
            }
        return max_c;
    }
    
int main(int argc,char* argv[]){
    int e;
    int n;
    cin >> n >> e;
    
    
    vector<vector<int>> A(n);

    
    #pragma omp  parallel for
    for(int x = 0;x < n;x++){
        A[x].resize(0);
    }
    
    for(int i = 0; i < e;i++){
        int x,y;
        cin >> x >> y;
        A[x-1].push_back(y-1);
        A[y-1].push_back(x-1);  
    }

    SS(A,n);
    
    return 0;
    }
