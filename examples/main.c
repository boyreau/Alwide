

int sum(int a, int b) {
	return a + b;
}


int main(int argc, char **args) {

	for (int i = 0 ; i < argc ; i++) {
		int total = sum(i, i);
	}

	return 0;
}