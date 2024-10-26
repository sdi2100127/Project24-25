#include "algorithms.h"

float euclidean_distance(float* vec1, float* vec2, int comps) {
    float res;
    for (int i = 0; i < comps; i++) {
        res = res + pow(vec1[i] - vec2[i], 2);
    }
    res = sqrt(res);
    return res;
}

Set greedySearch(int** G, int R, int dim, int vecs, float** vectors, int s, float* xq, int L, int k, Set V) {
    // initializing the knn set with the starting point s
    Set knn_set = set_Create();
    set_insert(knn_set, s);

    // initializing the empty set for visited nodes
    Set visited_nodes = set_Create();

    int flag = 1;

    set_Node node_v;
    int node_value, xp = -1;
    float min_dist;
    float* dist_matrix = (float*)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
      dist_matrix[i] = -1.0;
    }
    float* vec_p = (float*)malloc(dim * sizeof(float));
    while (flag == 1) {
        // we check the flag up front in case we do not have any unvisited nodes in the first iteration
        flag = 0;
        // check for all the unvisited nodes on the knn_set
        for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) {
            node_value = node->value;
            if (S_find_equal(visited_nodes->root, node_value) == SET_EOF) {
                flag = 1;   // unvisited nodes where indeed found

                // find the correspoding vector of the index node_value and store it temporarilly in vec_p
                for (int i=0; i<dim; i++) {
                    vec_p[i] = vectors[i][node_value];
                    printf("%f ", vec_p[i]);
                }
                printf("\n");

                // compute its distance to the query vector
                float dist = euclidean_distance(vec_p, xq, dim);
                // and add it to the distance matrix
                dist_matrix[node_value] = dist;                

                // if the next node xp is not initialized yet initialize it with the first unvisited node you encounter
                // otherwise check if its position is smaller than the current min_pos
                if (xp == -1 || dist <= min_dist) {
                    xp = node_value;
                    min_dist = dist;
                }
            }
        }
        printf("min_dist: %f, xp: %d\n", min_dist, xp);

        // if there where no more unvisited nodes found there is no point in continuing with the loop
        if (flag == 0) break; 

        // now we add all the outgoing neighbours of the node xp in the knn_set
        // the outgoing neighbours of a node are the one that are included in said node's column
        // the incoming neighbours are the other node's columns that the node is part of
        for (int i=0; i<R; i++) {
            set_insert(knn_set, G[i][xp]);
        }

        printf("knn_set: ");
        for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) { 
            printf("%d ", node->value);
        }
        printf("\n");

        // add node xp to te visited_nodes set
        set_insert(visited_nodes, xp);

        printf("visited_set: ");
        for (set_Node node = find_min(visited_nodes->root); node != SET_EOF; node = set_next(visited_nodes, node)) { 
            printf("%d ", node->value);
        }
        printf("\n");

        printf("dist_matrix\n");
        for (int i=0; i<vecs; i++) {
            printf("%f ", dist_matrix[i]);
        }
        printf("\n");

        // if the knn_set is bigger than L
        if (knn_set->size > L) {
            printf("knn_set->size > L\n");
            // remove the most distant neighbours of xq from knn_set until its equal or smaller than L
            while(knn_set->size > L) {
                printf("size: %d\n", knn_set->size);
                float max_dist = 0.0;
                int max_pos = -1;
                for (int i=0; i<vecs; i++) {
                    if (dist_matrix[i] >= max_dist) {
                        max_dist = dist_matrix[i];
                        max_pos = i;
                        dist_matrix[i] = -1.0;
                    }
                }
                printf("max_dist: %f, max_pos: %d\n", max_dist, max_pos);
                set_Node node = S_find_equal(knn_set->root, max_pos);
                if (node != SET_EOF) {
                    set_remove(knn_set, max_pos);
                }
            }
        }
        
    }
    
    // now we have to prune the set again to only keep xq's k nearest neighbours
    if (knn_set->size > k) {
        // remove the most distant neighbours of xq from knn_set until its equal or smaller than L
        while(knn_set->size > k) {
            float max_dist = 0.0;
            int max_pos = -1;
            for (int i=0; i<vecs; i++) {
                if (dist_matrix[i] >= max_dist) {
                    max_dist = dist_matrix[i];
                    max_pos = i;
                    dist_matrix[i] = -1.0;
                }
            }
            set_Node node = S_find_equal(knn_set->root, max_pos);
            if (node != SET_EOF) {
                set_remove(knn_set, max_pos);
            }
        }
    }

    V = visited_nodes;

    free(vec_p);
    free(dist_matrix);

    return knn_set;

}


// Set greedySearch(int** G, int nn, int vecs, int s, int xq, int k, int L, Set V) {
//     // initializing the knn set with the starting point s
//     Set knn_set = set_Create();
//     set_insert(knn_set, s);

//     // initializing the empty set for visited nodes
//     Set visited_nodes = set_Create();

//     int flag = 1;

//     set_Node node_v;
//     int node_value, xp = -1;
//     int pos, min_pos;

//     while (flag == 1) {
//         // we check the flag up front in case we do not have any unvisited nodes in the first iteration
//         flag = 0;

//         // check for all the unvisited nodes on the knn_set
//         for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) {
//             node_value = node->value;
//             if (S_find_equal(visited_nodes->root, node_value) == SET_EOF) {
//                 flag = 1;   // unvisited nodes where indeed found
                
//                 // refer to the groundtruth graph to find the position of the unvisited node (node_value) in the column of the query node (xq)
//                 // the smaller the position the smallest the euclidean distance between the unvisited node (node_value) and the query node (xq) 
//                 for (int i=0; i<nn; i++) {
//                     if (G[i][xq] == node_value) {
//                         pos = i;
//                         break;
//                     }
//                 }

//                 // if the next node xp is not initialized yet initialize it with the first unvisited node you encounter
//                 // otherwise check if its position is smaller than the current min_pos
//                 if (xp == -1 || pos <= min_pos) {
//                     xp = node_value;
//                     min_pos = pos;
//                 }
//             }
//         }

//         // if there where no more unvisited nodes found there is no point in continuing with the loop
//         if (flag == 0) break; 

//         // now we add all the outgoing neighbours of the node xp in the knn_set
//         // the outgoing neighbours of a node are the one that are included in said node's column
//         // the incoming neighbours are the other node's columns that the node is part of
//         for (int i=0; i<nn; i++) {
//             set_insert(knn_set, G[i][xp]);
//         }

//         // add node xp to te visited_nodes set
//         set_insert(visited_nodes, xp);

//         int count = 0;
//         Set pruned_set;
//         // if the knn_set is bigger than L
//         if (knn_set->size > L) {
//             // create a new set (pruned_set) and fill it with the L closest to xq nodes that are also in knn_set
//             for (int i=0; i<nn; i++) {
//                 node_value = G[i][xq];
//                 if (S_find_equal(knn_set->root, node_value) != SET_EOF) {
//                     set_insert(pruned_set, node_value);
//                     count++;
//                     if (count == L) break;
//                 }
//             }
//         }

//         // now we have to prone the set again to only keep xq's k nearest neighbours
//         knn_set = pruned_set;
//         if (knn_set->size > k) {
//             // create a new set (pruned_set) and fill it with the L closest to xq nodes that are also in knn_set
//             for (int i=0; i<nn; i++) {
//                 node_value = G[i][xq];
//                 if (S_find_equal(knn_set->root, node_value) != SET_EOF) {
//                     set_insert(pruned_set, node_value);
//                     count++;
//                     if (count == k) break;
//                 }
//             }
//         }
//         knn_set = pruned_set;
//         V = visited_nodes;

//         return knn_set;

//     }

// }