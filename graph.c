#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graph { //from asgn4.pdf
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) { // from asgn4.pdf
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    // use calloc to initialize everything with zeroes
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    // allocate g->weights with a pointer for each row
    g->weights = calloc(vertices, sizeof(g->weights[0]));

    // allocate each row in the adjacency matrix
    for (uint32_t i = 0; i < vertices; i++) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }

    return g;
}

void graph_free(Graph **gp) {
    if (gp == NULL || *gp == NULL) {
        return;
    }
    Graph *g = *gp;
    free(g->visited);
    for (uint32_t i = 0; i < (g->vertices); i++) {
        free(g->names[i]);
        free(g->weights[i]);
    }
    free(g->weights);
    free(g->names);
    free(g);
    *gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    return g->vertices;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g->names[v])
        free(g->names[v]);
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    const char *output = NULL;
    if (g->names[v]) {
        output = g->names[v];
    }
    return output;
}

char **graph_get_names(const Graph *g) {
    return g->names;
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    if (g->directed) {
        g->weights[start][end] = weight;
    } else {
        g->weights[start][end] = weight;
        g->weights[end][start] = weight;
    }
    return;
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return g->weights[start][end];
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    return (bool) g->visited[v];
}

void graph_print(const Graph *g) {
    printf("===Graph---\nvertices: %u\n", graph_vertices(g));
    char **nms = graph_get_names(g);
    for (uint32_t i = 0; i < (g->vertices); i++) {
        printf("nm: %s\n", nms[i]);
    }
    uint32_t **weights = g->weights;
    for (uint32_t i = 0; i < (g->vertices); i++) {
        for (uint32_t j = 0; j < (g->vertices); j++) {
            printf("%2u ", weights[i][j]);
        }
        printf("\n");
    }
    return;
}
