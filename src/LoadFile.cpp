#include "LoadFile.hpp"

#include "stdio.h"
#include "stdlib.h"

int LoadFile(const char* path, char** content) {
	FILE* file;
	char ch;
	int fileSize, index = 0;

	fopen_s(&file, path, "rb");

	if (!file) {
		printf("Unable to find required file");
		return 0;
	}

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	*content = (char*)malloc(fileSize + 1);

	if (!*content) {
		fclose(file);
		return 0;
	}

	while ((ch = fgetc(file)) != EOF) {
		(*content)[index++] = ch;
	}
	(*content)[index] = '\0';
	fclose(file);
	return fileSize;
}