#include <stdio.h>
#include <math.h>
 
int main() {
    int N;
    double x = .1, sum = .0;
    
    printf("N=\t");
    scanf("%d", &N);
    
    if (N<0){
    	printf("error(N<=0)");
	return 0;
	}
    else {
        for(int i = 1; i < N + 1; i++) {
            sum += sin(i * x);
        }
        
        printf("sum=\t%lf", sum);
    }
    return 0;
}

