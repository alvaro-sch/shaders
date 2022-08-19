#ifndef _UTIL_H
#define _UTIL_H

#include <glad/glad.h>
#include <stddef.h>

char *file_to_string(const char *fp, size_t *len);

GLuint shader_from_source(GLenum type, const char *source);

#endif
