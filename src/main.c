#include <stdbool.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "defs.h"
#include "util.h"

typedef struct {
    SDL_GLContext context;
    SDL_Window *window;
} state_t;

state_t state_new(const char *title, uint32_t width, uint32_t height) {
    state_t self;

    TRY(!SDL_Init(SDL_INIT_EVERYTHING), "unable to init SDL!\n");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    self.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
    TRY(self.window != NULL, "unable to create widnow with error: %s", SDL_GetError());

    self.context = SDL_GL_CreateContext(self.window);
    TRY(self.context != NULL, "unable to create gl context with error: %s", SDL_GetError());

    SDL_GL_MakeCurrent(self.window, self.context);

    SDL_ShowWindow(self.window);

    gladLoadGL();
    glClearColor(0.2, 0.4, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(self.window);

    return self;
}

void state_free(state_t *self) {
    SDL_GL_DeleteContext(self->context);
    SDL_DestroyWindow(self->window);
}

int main(void) {
    state_t state = state_new("shaders!", 640, 480);

    char *vss = file_to_string("./shaders/vertex.glsl", NULL);
    char *fss = file_to_string("./shaders/1_algorithmic_drawing.glsl", NULL);

    GLuint vs = shader_from_source(GL_VERTEX_SHADER, vss);
    GLuint fs = shader_from_source(GL_FRAGMENT_SHADER, fss);

    GLuint sp = glCreateProgram();
    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);

    float vertices[] = {
         1.0, -1.0, 0.0,
        -1.0, -1.0, 0.0,
         1.0,  1.0, 0.0,
        -1.0,  1.0, 0.0,
    };

    GLushort indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    GLuint buffers[2];
    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(sp);
    glUniform2f(glGetUniformLocation(sp, "res"), 640.0, 480.0);
    glUseProgram(0);

    SDL_Event event;

    float m_x = 0.0;
    float m_y = 0.0;
    float time = SDL_GetTicks64() / 1000.0;

    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case (SDL_QUIT):
                quit = true;
                break;

            case (SDL_MOUSEMOTION):
                m_x = event.motion.x;
                m_y = event.motion.y;
                break;

            default:
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(sp);
        glUniform2f(glGetUniformLocation(sp, "mouse"), m_x, m_y);
        glUniform1f(glGetUniformLocation(sp, "time"), time);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
        SDL_GL_SwapWindow(state.window);

        time = SDL_GetTicks64() / 1000.0;

        SDL_Delay(1000/75);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, buffers);
    glDeleteShader(fs);
    glDeleteShader(vs);
    free(fss);
    free(vss);
}
