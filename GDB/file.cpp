#include <stdio.h>

int main(int argc, char const *argv[]) {
	FILE *ptr_file;
	int x;

	ptr_file=fopen("output.txt", "w");

	if (!ptr_file)
		return 1;

	for (x=1; x<=10; x++)
		fprintf(ptr_file,"%d\n", x);

	fclose(ptr_file);

	return  0;
}