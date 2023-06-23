#include "graph.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    printf("Running graph test\n");
    // Create a stack that can only hold one item
    Graph *g = graph_create(20, true);
    // Add 10 to the stack
    //assert(stack_push(s, 10));
    // make srue that the top of the stack is 10 when we peek!
    /*
    uint32_t x = 0;
    assert(stack_peek(s, &x));
    assert(x == 10);
    // make sure the stack is full!
    assert(stack_full(s));
    x = 2;
    // Make sure we can remove the item from the stack
    stack_pop(s, &x);
    assert(x == 10);
    assert(stack_empty(s));

*/
    // Free up so valgrind passes
    graph_add_vertex(g, "namelol", 1);
    graph_add_vertex(g, "nabe2", 2);
    printf("vertices: %u\n", graph_vertices(g));
    char **nms = graph_get_names(g);
    for (uint32_t i = 0; i < (sizeof(nms)); i++) {
        printf("nm: %s\n", nms[i]);
    }
    graph_add_edge(g, 1, 2, 9);
    printf("weight: %u\n", graph_get_weight(g, 1, 2));
    graph_visit_vertex(g, 1);
    graph_visit_vertex(g, 2);
    graph_unvisit_vertex(g, 2);
    printf("1: %d\n", graph_visited(g, 1));
    printf("2: %d\n", graph_visited(g, 2));
    graph_print(g);
    graph_free(&g);
    printf("Graph tests complete. Be sure to rerun with valgrind!\n");

    return 0;
}
