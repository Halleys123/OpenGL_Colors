#include "GlHelp.h"

void GlClearError() {
	while (glGetError());
}

bool GlLogError(const char* function, const char* file, int line) {
	GLenum err;

	if (err = glGetError()) {
		printf("\x1b[38;2;200;20;0m[OpenGL Error]\x1b[0m\nError in file: %s\nFunction Name: %s\nLine Number: %d\n", file, function, line);
		return true;
	}
	return false;
}