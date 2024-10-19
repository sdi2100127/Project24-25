#include "algorithms.h"

Set greedySearch(int** G, int nn, int vecs, int s, int xq, int k, int L, Set V) {
    // initializing the knn set with the starting point s
    Set knn_set;
    set_insert(knn_set, s);

    // initializing the empty set for visited nodes
    Set visited_nodes;

    int flag = 1;

    set_Node node_v;
    int node_value, xp = -1;
    int pos, min_pos;

    while (flag == 1) {
        // we check the flag up front in case we do not have any unvisited nodes in the first iteration
        flag = 0;

        // each time check if all the nodes on the knn_set have been visited
        for (set_Node node = set_first(knn_set); node != set_last(knn_set); node = set_next(knn_set, node)) {
            node_value = *set_node_value(knn_set, node);
            // if not the loop should continue
            if (set_find_node(visited_nodes, &node_value) == SET_EOF) {
                flag = 1;
                break;
            }
        }

        if (flag == 0) break;
        
        // for every unvisited node in the knn_set
        for (set_Node node = set_first(knn_set); node != set_last(knn_set); node = set_next(knn_set, node)) {
            node_value = *set_node_value(knn_set, node);
            if (set_find_node(visited_nodes, &node_value) == SET_EOF) {
                // refer to the groundtruth graph to find the position of the unvisited node (node_value) in the column of the query node (xq)
                // the smaller the position the smallest the euclidean distance between the unvisited node (node_value) and the query node (xq) 
                for (int i=0; i<nn; i++) {
                    if (G[i][xq] == node_value) {
                        pos = i;
                        break;
                    }
                }

                // if the next node xp is not initialized yet initialize it with the first unvisited node you encounter
                // otherwise check if its position is smaller than the current min_pos
                if (xp == -1 || pos <= min_pos) {
                    xp = node_value;
                    min_pos = pos;
                }   
            }
        }

        // we need the outgoing neighbours of xp !!!!!!!!!!!
        for (int i=0; i<nn; i++) {
            // add all the neighbours of the node xp in the knn_set
            set_insert(knn_set, G[i][xp]);
        }

        // add node xp to te visited_nodes set
        set_insert(visited_nodes, xp);

        int count = 0;
        Set pruned_set;
        // if the knn_set is bigger than L
        if (set_size(knn_set) > L) {
            // create a new set (pruned_set) and fill it with the L closest to xq nodes that are also in knn_set
            for (int i=0; i<nn; i++) {
                node_value = G[i][xq];
                if (set_find_node(knn_set, &node_value) != SET_EOF) {
                    set_insert(pruned_set, node_value);
                    count++;
                    if (count == L) break;
                }
            }
        }
        knn_set = pruned_set;
        V = visited_nodes;

        return knn_set;

    }

}