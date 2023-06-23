#include "path.h"

#include "stack.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->total_weight = 0;
    p->vertices = stack_create(capacity);
    return p;
}

void path_free(Path **pp) {
    if (pp != NULL && *pp != NULL) {
        stack_free(&(*pp)->vertices);
        free(*pp);
        *pp = NULL;
    }
    return;
}

uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    uint32_t distance = 0;
    if (!stack_empty(p->vertices)) {
        uint32_t *last_v = calloc(1, sizeof(uint32_t));
        stack_peek(p->vertices, last_v);
        distance = graph_get_weight(g, *last_v, val);
        free(last_v);
        last_v = NULL;
        p->total_weight += distance;
    } else {
        p->total_weight = 0;
    }
    stack_push((p->vertices), val);
    return;
}

uint32_t path_remove(Path *p, const Graph *g) {
    uint32_t return_val = -1;
    if (!stack_empty(p->vertices)) {
        uint32_t distance = 0;
        uint32_t *last_v = calloc(1, sizeof(uint32_t));
        stack_pop(p->vertices, last_v);
        return_val = *last_v;
        if (path_vertices(p) > 1) {
            uint *last_last_v = calloc(1, sizeof(uint32_t));
            stack_peek(p->vertices, last_last_v);
            distance = graph_get_weight(g, *last_last_v, *last_v);
            free(last_last_v);
            last_last_v = NULL;

            (p->total_weight) -= distance;
        } else {
            p->total_weight = 0;
        }
        free(last_v);
        last_v = NULL;
    }
    return return_val;
}

void path_copy(Path *dst, const Path *src) {
    if (dst) {
        dst->total_weight = src->total_weight;
        stack_copy(dst->vertices, src->vertices);
    }
    return;
}

void path_print(const Path *p, FILE *outfile, const Graph *g) {
    stack_print(p->vertices, outfile, graph_get_names(g));
}
