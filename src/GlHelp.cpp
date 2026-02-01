#include "GlHelp.h"

void GlClearError() {
	while (glGetError());
}

static const char* GetGLErrorString(GLenum error) {
    switch (error) {
    case GL_INVALID_ENUM:                  return "INVALID_ENUM";
    case GL_INVALID_VALUE:                 return "INVALID_VALUE";
    case GL_INVALID_OPERATION:             return "INVALID_OPERATION";
    case GL_STACK_OVERFLOW:                return "STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:               return "STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
    default:                               return "UNKNOWN_ERROR";
    }
}

bool GlLogError(const char* function, const char* file, int line) {
    bool hasError = false;
    // Use a loop because OpenGL can store multiple errors in a queue
    while (GLenum err = glGetError()) {
        char buffer[1024];
        sprintf_s(buffer,
            "[OpenGL Error %s (0x%04X)]\n"
            "File: %s\nFunction: %s\nLine: %d\n\n",
            GetGLErrorString(err), err, file, function, line);
        OutputDebugStringA(buffer);
        hasError = true;
    }
    return hasError;
}