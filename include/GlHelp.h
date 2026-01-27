#pragma once

#include "glad/glad.h"
#include "stdio.h"
#include "Windows.h"

#define GlCall(x) GlClearError();\
	x;\
	if(GlLogError( #x, __FILE__, __LINE__)) DebugBreak();\

void GlClearError();
bool GlLogError(const char* function, const char* file, int line);