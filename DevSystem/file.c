#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
	float x;
	float y;
} point_t;

/**
 *
 * fwrite (pointer, size, count, stream)
 *
 * void clearerr(FILE *stream);
 * int ferror(FILE *stream);
 *
 */

void bufferStream() {
	/** buffer specific functions */
	char buffer[256];

	// set buffer[256] as stdout buffer using 256 bytes
	// IOLBF = IO Line Buffered (buffer data until a \n is uncountered)
	setvbuf (stdout, buffer, _IOLBF, 256);

	char chaine[10];

	// You can see the behavior when no \n is displayed the data is not flushed
	// stderr is by default unbuffered
	fprintf(stdout, "1 stdout : ligne + \\n\n");

	fprintf(stdout, "2 stdout : ligne seule");
	printf("2 stdout : ligne seule");
	fprintf(stderr, "\n3 stderr : avant fflush(stdout)\n");
	fprintf(stderr, "Buffer : %s\n", buffer);
	fflush(stdout);
	fprintf(stderr, "\n4 stderr : après fflush (stdout)\n");
	fprintf(stdout, "5 stdout : ligne seule ");
	fprintf(stderr, "\n6 stderr : avant fgets(stdin)\n");
	fgets(chaine, 10, stdin);
	fprintf(stderr, "\n7 stderr : après fgets(stdin)\n");

	// IONBF = IO Not Buffered (No data buffer) on stdout
	setvbuf(stdout, NULL, _IONBF, 0);
}

int main (void) {

	bufferStream();

	/** file specific functions */
	FILE * file = fopen("file.txt", "a+"); // open in append mode
	if(file==NULL) {perror("fopen"); return -1; }

	int file_number = fileno(file); // get the file descriptor
	fprintf(stderr, "fileno : %d\n", file_number);

	int ret = fseek(file, -3, SEEK_END); // move to end
	if (ret!=0) {perror("fseek"); return -1; }

	point_t position = {15.5f, 5.5f};

	size_t size = fwrite(&position, sizeof(position), 1, file);
	if(size<=0) {perror("fwrite"); return -1; }

	size = fwrite(".more", 1, sizeof(".more"), file);
	if(size<=0) {perror("fwrite"); return -1; }

	fprintf(stderr, "written %zu bytes\n", size);

	int eof = feof(file);

	fprintf(stderr, "eof : %d\n", eof);

	ret = fseek(file, 0, SEEK_END);
	if (ret!=0) {perror("fseek"); return -1; }

	long pos=ftell(file); // tell the cursor position
	fprintf(stderr, "size : pos : %ld\n", pos);

	rewind(file); // move cursor to begining
	char * buf = (char *)malloc(pos); // malloc until cursor

	ret=fread(buf, 1, pos, file); // copy to buf data read from start to cursor pos
	if(ret!=pos) {perror("fread"); return -1; }

	fprintf(stderr, "%.*s\n", (int)pos, buf);

	free(buf);

	int ch = fgetc(file);
	fprintf(stderr, "last char is %#x\n", ch);

	eof = feof(file);
	fprintf(stderr, "eof : %d\n", eof);

	pos=ftell(file);
	fprintf(stderr, "pos [rewind]: %ld\n", pos);

	ret=fflush(file); // flush persistant bytes
	if(ret==EOF) {perror("fread"); return -1; }

	ret = fclose(file);
	if (ret!=0) {
		perror("fclose");
		return -1;
	}

	//fopen64(const char *restrict __filename, const char *restrict __modes)
	//fread_unlocked(void *restrict __ptr, size_t __size, size_t __n, FILE *restrict __stream)
	//fsetpos

	return 0;


}
