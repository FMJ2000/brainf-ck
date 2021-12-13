#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_F "code.bf"
#define BUF_LEN 30000
#define PRINT_LEN 500
#define ESC 27

int bf(char * code);

int main(int argc, char * argv[]) {
	FILE * fptr;
	char * buffer;
	long numbytes;
	
	// obtain brainfuck code
	if (argc >= 2) fptr = fopen(argv[1], "r");
	else fptr = fopen(DEFAULT_F, "r");
	if (fptr == NULL) {
		printf("Error opening file!\r\n");
		return 1;
	}

	// get number of bytes
	fseek(fptr, 0L, SEEK_END);
	numbytes = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);

	// open buffer for memory and read
	buffer = (char *)calloc(numbytes, sizeof(char));
	if (buffer == NULL) return 1;
	fread(buffer, sizeof(char), numbytes, fptr);
	fclose(fptr);

	// print and free
	//printf("File contains:\r\n%s\r\n", buffer);
	bf(buffer);
	free(buffer);

	return 0;
}

int bf(char * code) {
	char buffer[BUF_LEN];
	char input[BUF_LEN];
	char output[BUF_LEN];
	long bufPtr = 0;
	long codePtr = 0;
	long inputPtr = 0;
	long outputPtr = 0;

	// obtain input
	printf("Input: ");
	fgets(input, sizeof(input), stdin);

	// compiler
	while (code[codePtr] && inputPtr < BUF_LEN && outputPtr < BUF_LEN) {
		if (code[codePtr] == '>') bufPtr++;
		else if (code[codePtr] == '<') bufPtr--;
		else if (code[codePtr] == '+') buffer[bufPtr]++;
		else if (code[codePtr] == '-') buffer[bufPtr]--;
		else if (code[codePtr] == ',') buffer[bufPtr] = input[inputPtr++];
		else if (code[codePtr] == '.') output[outputPtr++] = buffer[bufPtr];
		else if (code[codePtr] == '[' && !buffer[bufPtr]) {
			int numBrackets = 0;
			while (code[++codePtr] != ']' || numBrackets > 0) {
				if (code[codePtr] == '[') numBrackets++;
				else if (code[codePtr] == ']') numBrackets--;
			}
		} else if (code[codePtr] == ']') {
			int numBrackets = 0;
			while(code[--codePtr] != '[' || numBrackets > 0) {
				if (code[codePtr] == '[') numBrackets--;
				else if (code[codePtr] == ']') numBrackets++;
			}
			codePtr--;
		}
		
		codePtr++;
	}
	
	// print input code
	printf("\r\n\r\nCode:\r\n%s", code);

	// print data memory
	printf("\r\n\r\nMemory (%ld):\r\n", bufPtr);
	for (int i = 0; i < PRINT_LEN; i++) {
		if (i == (int)bufPtr) printf("%c[1m%d%c[0m ", ESC, buffer[i], ESC);
		else printf("%d ", buffer[i]);
		if ((i+1) % 50 == 0) printf("\r\n");
	}

	// print output
	printf("\r\n\r\nOutput:\r\n%s", output);
	printf("\r\n");
	
	return 0;
}