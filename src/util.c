#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

#define DEFAULT_SIZE  2048
#define READ_SIZE     DEFAULT_SIZE
#define GROWTH_FACTOR 2

// it's quite robust
char *file_to_string(const char *fp, size_t *len) {
    FILE *f = fopen(fp, "r");

    if (f == NULL) {
        return NULL;
    }

    size_t size = 0;
    size_t cap = DEFAULT_SIZE;
    char *str = calloc(cap + 1, sizeof(*str));

    if (str == NULL) {
        fclose(f);
        return NULL;
    }

    while (!feof(f)) {
        if (size == cap) {
            cap *= GROWTH_FACTOR;
            char *new_ptr = realloc(str, (cap + 1) * sizeof(*str));

            if (new_ptr == NULL) {
                free(str);
                fclose(f);
                return NULL;
            }

            str = new_ptr;
        }

        size += fread(str + size, sizeof(*str), READ_SIZE, f);
    }

    fclose(f);
    str[size] = '\0';

    if (len != NULL) {
        *len = size;
    }

    return str;
}

GLuint shader_from_source(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (void *) &source, 0);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (!status) {
        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        fprintf(stderr, "failed to compile shader! with error\n%s\n", info_log);
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}
