#include "algorithms.h"

float euclidean_distance(float* vec1, float* vec2, int comps) {
    float res = 0.0f;
    for (int i = 0; i < comps; i++) {
        res = res + pow(vec1[i] - vec2[i], 2);
    }
    res = sqrt(res);
    return res;
}

Set greedySearch(int** G, int R, int dim, int vecs, float** vectors, int s, float* xq, int L, int k, Set* V) {
    printf("GREEDY SEARCH\n");

    // initializing the knn set with the starting point s
    Set knn_set = set_Create();
    set_insert(knn_set, s);

    // initializing the empty set for visited nodes
    Set visited_nodes = set_Create();

    int flag = 1;

    set_Node node_v;
    int node_value, xp = -1;
    float min_dist;

    float* vec_p = (float*)malloc(dim * sizeof(float));

    // create a distance matrix with the euclidean distance of each vector to xq
    float* dist_matrix = (float*)malloc(vecs * sizeof(float*));
    for (int j = 0; j < vecs; j++) {
        for (int i=0; i<dim; i++) {
            vec_p[i] = vectors[i][j];
        }
        dist_matrix[j] = euclidean_distance(vec_p, xq, dim);
    }
    printf("dist_matrix\n");
    for (int i=0; i<vecs; i++) {
        printf("%f ", dist_matrix[i]);
    }
    printf("\n");

    float prev_max; 
    int prev_max_pos;

    while (flag == 1) {
        // we check the flag up front in case we do not have any unvisited nodes in the first iteration
        flag = 0;
        // for every iteration we have to reinitiallize the minimum
        xp = -1;
        // check for all the unvisited nodes on the knn_set
        for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) {
            node_value = node->value;
            if (S_find_equal(visited_nodes->root, node_value) == SET_EOF) {
                flag = 1;   // unvisited nodes where indeed found              

                // if the next node xp is not initialized yet initialize it with the first unvisited node you encounter
                // otherwise check if its position is smaller than the current min_pos
                float dist = dist_matrix[node_value];
                if (xp == -1 || dist_matrix[node_value] <= min_dist) {
                    xp = node_value;
                    min_dist = dist;
                }
            }
        }
        //printf("min_dist: %f, xp: %d\n", min_dist, xp);

        // if there where no more unvisited nodes found there is no point in continuing with the loop
        if (flag == 0) break; 

        // now we add all the outgoing neighbours of the node xp in the knn_set
        // the outgoing neighbours of a node are the one that are included in said node's column
        // the incoming neighbours are the other node's columns that the node is part of
        for (int i=0; i<R; i++) {
            set_insert(knn_set, G[i][xp]);
        }

        // printf("knn_set: ");
        // for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) { 
        //     printf("%d ", node->value);
        // }
        // printf("\n");

        // add node xp to te visited_nodes set
        set_insert(visited_nodes, xp);

        // printf("visited_set: ");
        // for (set_Node node = find_min(visited_nodes->root); node != SET_EOF; node = set_next(visited_nodes, node)) { 
        //     printf("%d ", node->value);
        // }
        // printf("\n");

        // if the knn_set is bigger than L
        if (knn_set->size > L) {
            // remove the most distant neighbours of xq from knn_set until its equal to L
            prev_max = 0.0;
            prev_max_pos = -1;
            while(knn_set->size > L) {
                float max_dist = 0.0;
                int max_pos = -1;
                for (int i=0; i<vecs; i++) {
                    if (dist_matrix[i] >= max_dist && (prev_max == 0.0 || prev_max >= dist_matrix[i])) {
                        if (i != prev_max_pos) {
                            max_dist = dist_matrix[i];
                            max_pos = i;
                        }
                    } 
                }
                //printf("max_dist: %f, max_pos: %d\n", max_dist, max_pos);
                set_Node node = S_find_equal(knn_set->root, max_pos);
                if (node != SET_EOF) {
                    set_remove(knn_set, max_pos);
                }
                prev_max = max_dist;
                prev_max_pos = max_pos;
            }
        }
    }
    
    // now we have to prune the set again to only keep xq's k nearest neighbours
    if (knn_set->size > k) {
        // remove the most distant neighbours of xq from knn_set until its equal to k
        while(knn_set->size > k) {
            float max_dist = 0.0;
            int max_pos = -1;
            for (int i=0; i<vecs; i++) {
                if (dist_matrix[i] >= max_dist && (prev_max == 0.0 || prev_max >= dist_matrix[i])) {
                    if (i != prev_max_pos) {
                        max_dist = dist_matrix[i];
                        max_pos = i;
                    }
                } 
            }
            //printf("max_dist: %f, max_pos: %d\n", max_dist, max_pos);
            set_Node node = S_find_equal(knn_set->root, max_pos);
            if (node != SET_EOF) {
                set_remove(knn_set, max_pos);
            }
            prev_max = max_dist;
            prev_max_pos = max_pos;
        }
    }

    printf("knn_set: ");
    for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) { 
        printf("%d ", node->value);
    }
    printf("\n");

    printf("visited_set: ");
    for (set_Node node = find_min(visited_nodes->root); node != SET_EOF; node = set_next(visited_nodes, node)) { 
        printf("%d ", node->value);
    }
    printf("\n");

    
    *V = visited_nodes;
    
    free(vec_p);
    free(dist_matrix);

    //set_destroy()
    return knn_set;

}

//Implementation of Robust Prune function
void RobustPrune(int *** G, int p ,Set * V, int a, int R , int * neigh_count ,int dim , int vecs , float **vectors){
    printf("ROBUST PRUNE\n");
    
    Set temp = *V;
    // printf("V: ");
    // for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) 
    //         printf("%d ", node->value);
    // printf("\n");
    int ** temp_G = *G;
    for (int i = 0; i <= R - 1 ;i++){           //Inserting our Nout(p) to our V set
        set_insert(temp, temp_G[i][p]);
    }

    set_remove(temp, p);                           //And checking if we have inserted our p so that we remove it

    printf("Nout(%d): ", p);
    for (int i=0; i<dim; i++) {
        printf("%d ", temp_G[i][p]);
    }
    printf("\n");

    for (int i = 0; i <= R - 1 ;i++){           //Now we have to empty our Nout(p)
        temp_G[i][p] = -1;                           //And to do that we will set everything to -1 to know that it is supposed to be empty

    }
    
    
    // printf("V: ");
    // for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) 
    //         printf("%d ", node->value);
    // printf("\n");

    int i_count = 0;

    float* dist_matrix_2 = (float*)malloc(vecs * sizeof(float*));
    float* vec_of_p_star = (float*)malloc(dim * sizeof(float));
    float* vec_p = (float*)malloc(dim * sizeof(float));             //And to do that we create a distance matrix with the euclidean distance
    float* vec_of_p = (float*)malloc(dim * sizeof(float));
    float* dist_matrix = (float*)malloc(vecs * sizeof(float*));

    for (int i=0; i<dim; i++) {
        vec_of_p[i] = vectors[i][p];
    }

    for (int j = 0; j < vecs; j++) {
        for (int i=0; i<dim; i++) {
        vec_p[i] = vectors[i][j];
        }
        dist_matrix[j] = euclidean_distance(vec_p, vec_of_p, dim);
    }

    // printf("dist_matrix\n");
    // for (int i=0; i<vecs; i++) {
    //     printf("%f ", dist_matrix[i]);
    // }
    // printf("\n");
    
    while(temp->size != 0){
        
        float min_dist;         //Now we have to find minimum distance to each vector from our point p
        int p_star = -1;

        for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) {
            int node_value = node->value;
            float dist = dist_matrix[node_value];
            if (p_star == -1 || dist_matrix[node_value] <= min_dist) {
                p_star = node_value;
                min_dist = dist;
            }
        }
        printf("p*: %d\n", p_star);
        temp_G[i_count][p] = p_star;
        i_count ++;

        if(i_count == R)
            break;

        printf("vec p*: ");
        for (int i=0; i<dim; i++) {
            vec_of_p_star[i] = vectors[i][p_star];
            printf("%f ", vec_of_p_star[i]);
        }
        printf("\n");

        for (int j = 0; j < vecs; j++) {
            for (int i=0; i<dim; i++) {
                vec_p[i] = vectors[i][j];
            }
            dist_matrix_2[j] = euclidean_distance(vec_p, vec_of_p_star, dim);
        }

        // printf("dist_matrix2\n");
        // for (int i=0; i<vecs; i++) {
        //     printf("%f ", dist_matrix_2[i]);
        // }
        // printf("\n");

        set_Node next = find_min(temp->root);
        while(next != SET_EOF){
            int node_value = next->value;
            if(a * dist_matrix_2[node_value] <= dist_matrix[node_value]){
                next = set_next(temp, next);
                //printf("remove %d\n", node_value);
                set_remove(temp,node_value);
            } else {
                next = set_next(temp, next);
            }
        }

    }

    printf("Nout(%d): ", p);
    for (int i=0; i<dim; i++) {
        printf("%d ", temp_G[i][p]);
    }
    printf("\n");

    *V = temp;
    *G = temp_G;
    *neigh_count = i_count;

    free(dist_matrix);
    free(dist_matrix_2);
    
    free(vec_p);
    free(vec_of_p);
    free(vec_of_p_star);
}
