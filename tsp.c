#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool all_visited(const Graph *g) {
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        if (graph_visited(g, i) == false) {
            return false;
        }
    }
    return true;
}
void dsf(uint32_t n, Graph *g, uint32_t start, Path *curr, Path *best) {
    //printf("===CURR NODE: %u\n", n);
    graph_visit_vertex(g, n);
    path_add(curr, n, g);
    if ((path_vertices(curr) == graph_vertices(g)) && (all_visited(g))) {
        // if visited all nodes
        //printf("VISITED ALL NODES\n");
        uint32_t dist = graph_get_weight(g, n, start);
        if (dist != 0) {
            // if curr node has path back to start
            //printf("HAS PATH BACK\n");
            dist += path_distance(curr); // adding final distance
            if ((path_vertices(best) == 0) || (dist < path_distance(best))) {
                // if *best is empty or *curr is shorter than best
                //printf("TIME TO COPY\n");
                path_add(curr, start, g);
                //printf("ADDED\n");
                path_copy(best, curr);
                //printf("=========================\n");
                //path_print(curr, stdout, g);
                //printf("===================COPIED\n");
                path_remove(curr, g);
            }
        }
    }
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        if (graph_get_weight(g, n, i) != 0) {
            // if there is an edge from n to i
            if (graph_visited(g, i) == false) {
                // if i is unvisited
                dsf(i, g, start, curr, best);
            }
        }
    }
    graph_unvisit_vertex(g, n);
    //printf("UNVISITED, %u\n", n);
    path_remove(curr, g);
    //printf("REMOVED\n");
}

int main(int argc, char *argv[]) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int DIRECTED = 0;
    int opt_h = 0;
    int opt_i = 0;
    int opt_o = 0;
    int opt_d = 0;
    int option;
    char *input_filename = NULL;
    char *output_filename = NULL;
    uint32_t num_vertices, num_edges, e1, e2, e3; // latter 3 for edge reading from infile
    char vertex_name[100]; // for reading vertex name in infile

    while ((option = getopt(argc, argv, "i:o:dh")) != -1) {
        switch (option) {
        case 'i':
            input_filename = optarg;
            opt_i = 1;
            break;
        case 'o':
            output_filename = optarg;
            opt_o = 1;
            break;
        case 'd': opt_d = 1; break;
        case '?':
            opt_h = 1;
            fprintf(stderr, "Invalid option %c, possible options: -i -o -d -h\n", optopt);
            break;
        case 'h':
        default:
            opt_h = 1;
            printf("Possible options: -i -o -d -h\n");
            break;
        }
    }
    if (opt_h) {
        return 0;
    }
    if (opt_d) {
        DIRECTED = 1;
    }
    if (opt_i) {
        infile = fopen(input_filename, "r");
    }
    // confirm that infile is not NULL
    if (infile == NULL) {
        fprintf(stderr, "tsp: Failed to open in file.\n");
        exit(1);
    }
    // read vertices
    if (fscanf(infile, "%u\n", &num_vertices) != 1) {
        fprintf(stderr, "tsp: error reading number of vertices\n");
        if (infile != stdin)
            fclose(infile);
        exit(1);
    }
    Graph *g = graph_create(num_vertices, ((bool) DIRECTED)); // creating graph
    for (uint32_t i = 0; i < num_vertices; i++) {
        // read vertices
        fscanf(infile, "%[^\n]", vertex_name);
        fscanf(infile, "%*c"); // skip "\n" character
        graph_add_vertex(g, vertex_name, i);
    }
    // read edges
    if (fscanf(infile, "%u\n", &num_edges) != 1) {
        fprintf(stderr, "tsp: error reading number of edges\n");
        if (infile != stdin)
            fclose(infile);
        exit(1);
    }
    for (uint32_t i = 0; i < num_edges; i++) {
        // read edges
        fscanf(infile, "%u %u %u", &e1, &e2, &e3);
        graph_add_edge(g, e1, e2, e3);
    }
    if (infile != stdin)
        fclose(infile);

    //graph_print(g);

    // start dsf algorithm
    Path *curr = path_create(num_vertices + 1); // accounts for edge going back to start
    Path *best = path_create(num_vertices + 1);
    dsf(START_VERTEX, g, START_VERTEX, curr, best);
    // dsf finished running
    //printf("DSF FINISHED\n");
    if (opt_o) {
        outfile = fopen(output_filename, "w");
    }
    if (outfile == NULL) {
        fprintf(stderr, "tsp: Failed to open out file.\n");
        exit(1);
    }
    //printf("OUTFILE OPENED\n");
    if (!path_vertices(best)) {
        // if no vertices in best path
        fprintf(outfile, "No path found! Alissa is lost!\n");
    } else {
        //printf("ELSE\n");
        fprintf(outfile, "Alissa starts at:\n");
        path_print(best, outfile, g);
        fprintf(outfile, "Total Distance: %u\n", path_distance(best));
    }
    //printf("PRINTED\n");
    if (outfile != stdout)
        fclose(outfile);

    path_free(&curr);
    path_free(&best);
    graph_free(&g);
    return 0;
}
