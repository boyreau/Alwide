#include <stdio.h>
int my_func(int a, int b){
	return 0;
}

int main(int argc, char **arg){

	for (int i = 0 ; i<  100 ; i++){
		my_func(i, i);
		fprintf(stderr, "Ceci est un test !. Bonjour à tous !");
	}

	return 0;
}