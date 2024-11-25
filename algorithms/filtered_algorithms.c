#include "filtered_algorithms.h"
#include <float.h>

PQueue FilteredGreedySearch(Vector* G, int R, int dim, int vecs, float** vectors, float* xq, int fq, Map filter_med, int L, int k, Set* V) {
    
    // initialise knn and visited set as empty
    PQueue knn = pqueue_create(NULL);
    Set visited_nodes = set_Create();

    // since the query xq has only one filter we can simply check which starting point corresponds with this flter in the map
    int s = vec_get_at(map_find_values(filter_med, fq), 0);
    // and insert it into the k-nearest-neighbours priority queue
    float* vec_s = (float*)malloc((dim-2) * sizeof(float));
    printf("vec_s: ");
    int count = 0;
    for (int i=2; i<dim; i++) {
        vec_s[count] = vectors[i][s];
        printf("%f ", vec_s[count]);
        count++;
    }
    printf("\n");
    pqueue_insert(knn, s, euclidean_distance(vec_s, xq, dim-2));

    int flag = 1;
    int node_value, xp = -1;
    float min_dist;

    float* vec_p = (float*)malloc((dim-2) * sizeof(float));

    //create a distance matrix with the euclidean distance of each vector to xq and initiallize it to -1
    float* dist_matrix = (float*)malloc(vecs * sizeof(float*));
    for (int j = 0; j < vecs; j++) {
        dist_matrix[j] = -1;
    }

    float prev_max; 
    int prev_max_pos;

    while(flag == 1) {
        // we check the flag up front in case we do not have any unvisited nodes in the first iteration
        flag = 0;
        // for every iteration we have to reinitiallize the minimum
        xp = -1;
        // check for all the unvisited nodes in knn
        Vector knn_vec = knn->vector;
        for (VecNode node = vec_first(knn_vec); node != VECTOR_EOF; node = vec_next(knn_vec, node)) {
            node_value = node->value;
            //printf("node_value: %d\n", node_value);
            if (S_find_equal(visited_nodes->root, node_value) == SET_EOF) {
                flag = 1;   // unvisited nodes where indeed found              

                // if the next node xp is not initialized yet initialize it with the first unvisited node you encounter
                // otherwise check if its distance to xq is smaller than the current min_dist
                if (dist_matrix[node_value] == -1) {    // if the distance has not been computed before, compute it
                    count = 0;
                    for (int i=2; i<dim; i++) {
                        vec_p[count] = vectors[i][node_value];
                        count++;
                    }
                    dist_matrix[node_value] = euclidean_distance(vec_p, xq, dim-2);
                }
                float dist = dist_matrix[node_value];

                if (xp == -1 || dist <= min_dist) {
                    xp = node_value;
                    min_dist = dist;
                }
            }
        }

        // if there where no more unvisited nodes found there is no point in continuing with the loop
        if (flag == 0) break; 

        // add node xp to te visited_nodes set
        set_insert(visited_nodes, xp);

        // now we add all the outgoing neighbours of the node xp that also have the same filter as xq in knn
        // the outgoing neighbours of a node are the one that are included in said node's column
        // the incoming neighbours are the other node's columns that the node is part of
        for (int i=0; i<G[xp]->size; i++) {
            int point = vec_get_at(G[xp], i);

            // if the point has a different filter than the query ignore it
            int f = (int)vectors[0][point];
            if (f != fq) continue;

            // if the distance has not been computed before, compute it
            if (dist_matrix[point] == -1) {
                count = 0;
                for (int i=2; i<dim; i++) {
                    vec_p[count] = vectors[i][point];
                    count++;
                }
                dist_matrix[point] = euclidean_distance(vec_p, xq, dim-2);
            }
            pqueue_insert(knn, point, dist_matrix[point]);
        }

        // if the knn_set is bigger than L
        if (knn->vector->size > L) {
            // remove the most distant neighbours of xq from knn_set until its equal to L
            while(knn->vector->size > L) {
                pqueue_remove(knn);
            }
        }

    }

    // now we have to prune the set again to only keep xq's k nearest neighbours
    if (knn->vector->size > k) {
        // remove the most distant neighbours of xq from knn_set until its equal to k
        while(knn->vector->size > k) {
            pqueue_remove(knn);
        }
    }

    printf("knn_set: ");
    for (VecNode node = vec_first(knn->vector); node != VECTOR_EOF; node = vec_next(knn->vector, node)) { 
        printf("%d ", node->value);
    }
    printf("\n");

    printf("visited_set: ");
    for (set_Node node = find_min(visited_nodes->root); node != SET_EOF; node = set_next(visited_nodes, node)) { 
        printf("%d ", node->value);
    }
    printf("\n");

    *V = visited_nodes;

    free(vec_s);
    free(dist_matrix);
    free(vec_p);

    return knn;
}

void FilteredRobustPrune(Vector** G, int p ,Set* V, int a, int R , int dim , int vecs , float** vectors, float** dist_m) {

}

int FilteredMedoid(float** dataset, int vecs, int comps, float*** dist_m) {
    float** temp_m =  *dist_m;

    float* vec_p1 = (float*)malloc((comps-2) * sizeof(float));
    float* vec_p2 = (float*)malloc((comps-2) * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;
    float dist;
    float dist_sum;

    // for each vector in the dataset
    for (int j=0; j<vecs; j++) {

        dist_sum = 0.0f;
        int count = 0;
        for (int i=2; i<comps; i++) {
            vec_p1[count] = dataset[i][j];
            count++;
        }

        
        // compute its distance to all other vectors
        for (int z=0; z<vecs; z++) {
            count = 0;
            for (int i=2; i<comps; i++) {
                vec_p2[count] = dataset[i][z];
                count++;
            }

            // and add it to the total sum
            if (j != z)  {
                // compute the distance only if it has not been computed before
                if (temp_m[j][z] == 0) {
                    dist = squared_euclidean_distance(vec_p1, vec_p2, comps-2);
                    temp_m[j][z] = dist;
                    temp_m[z][j] = dist;
                }
                dist_sum = dist_sum + temp_m[j][z];
            }
        }
        
        // then find which vector has the lowest total sum
        if (dist_sum <= min_sum) {
            min_sum = dist_sum;
            min_p = j;
        }
    }

    free(vec_p1);
    free(vec_p2);

    *dist_m = temp_m;

    printf("medoid sum: %f\n", min_sum);
    return min_p;
}

Map FindMedoid(float** dataset, int vecs, float min_f, float max_f, Map filtered_data, int t) {    // initialize M as an empty map
    // M mapps filters to start nodes
    Map M = map_create(min_f, max_f);

    // initialize T as an empty map
    // T mapps point ids to how often they have been used as medoids
    Map T = map_create(0, vecs);
    // and initialize it to 0
    for (int i=0; i<vecs; i++) {
        map_insert(T, i, 0);
    }

    srand((unsigned int)time(NULL));

    // for each different filter f
    for (MapNode node = map_first(filtered_data); node != MAP_EOF; node = map_next(filtered_data, node)) {
        // let P_f denote the ids of all points matching filter f
        // (which are the contents of the corresponding bucket in the hash map)
        Vector P_f = node->values;

        // R_f holds t randomly sampled data point ids from P_f
        Set R_f = set_Create();

        if (t <= P_f->size) {
            for (VecNode v_node = vec_first(P_f); v_node != VECTOR_EOF; v_node = vec_next(P_f, v_node)) { 
                set_insert(R_f, v_node->value);
            }
        } else {
            while (R_f->size < t) {
                int x = rand() % (P_f->size-1);
                set_insert(R_f, vec_get_at(P_f, x));
            }
        }

        // find the point of the filter f(node) with the minimum uses as medoid
        int p_star;
        int min_T = max_f;
        for (set_Node s_node = find_min(R_f->root); s_node != SET_EOF; s_node = set_next(R_f, s_node)) { 
            
            Vector val = map_find_values(T, s_node->value);
            int count = vec_get_at(val, 0);
            
            if (count < min_T) {
                p_star = s_node->value;
                min_T = count;
            }
        }

        // update it in map M 
        map_insert(M, node->key, p_star);

        // and increase the count in map T
        // obtain current count
        int cur_count = vec_get_at(map_find_values(T, p_star), 0);
        // remove it to ensure that the map always contains only the latest count
        map_remove(T, p_star);
        // insert the key p_star again with the updated count
        map_insert(T, p_star, cur_count++);

        set_destroy(R_f);

    }

    map_destroy(T);

    return M;
}

Vector* FilteredVamanaIndexing(float** dataset, float min_f, float max_f, int vecs, int comps, int L, int R, int neigh, int a, int* med, int t) {
    // first we initialize G to an empty graph
    Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   
    for (int i=0; i<vecs; i++) {
        G[i] = vec_Create(0);
    }

    // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
    float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
        dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
    }

    // and initiallize it with zeros
    for(int i=0; i<vecs; i++) {
        for(int j=0; j<vecs; j++) {
            dist_matrix[i][j] = 0;
        }
    }

    // now we find the medoid of the dataset that will be our starting point s
    int s = FilteredMedoid(dataset, vecs, comps, &dist_matrix);
    printf("vamana found medoid: %d\n", s);
    *med = s;

    // now we will have to map each point of the dataset to each corresponding filter
    // we will do that by using a hashmap structure(filter --> key) that has an array big enough
    // so that every key-filter hashes to a different position
    // by doing that we ensure that every position in the hash map only holds the points
    // corresponding to a certain filter f(values --> set of points with filter f)
    Map filtered_data = map_create(min_f, max_f);

    for (int j=0; j<vecs; j++) {
        map_insert(filtered_data, (int)dataset[0][j], j);
    }

    Vector values;
    for (MapNode node = map_first(filtered_data); node != MAP_EOF; node = map_next(filtered_data, node)) {
        printf("filter %d: ", node->key);
        values = node->values;
        for (VecNode s_node = vec_first(values); s_node != VECTOR_EOF; s_node = vec_next(values, s_node)) {
            printf("%d ", s_node->value);
        }
        printf("\n");
    }

    int x;
    float* vec_x = (float*)malloc(comps * sizeof(float));
    float* vec_j = (float*)malloc(comps * sizeof(float));

    // for every vector in the dataset
    for (int j = 0; j < vecs; j++) {
        for (int i=0; i<comps; i++) {
            vec_j[i] = dataset[i][j];
        }
        printf("vector %d:", j);

        // find its filter
        int filter = dataset[0][j];
        Vector same_f_values = map_find_values(filtered_data, filter);
    
        // for every one of its neigh neighbours, or until all the same filter points have been checked
        int count_n = 0;
        VecNode v_node = vec_first(same_f_values);
        while (count_n < neigh || count_n < same_f_values->size) {
            // insert them in the vector of out going neighbours of vector j
            if (v_node->value != j) {
                // temporarilly store the vectors j and x to compute their distance
                for (int i=0; i<comps; i++) {
                    vec_x[i] = dataset[i][x];
                }
                float dist = euclidean_distance(vec_j, vec_x, comps);
                vec_insert(G[j], v_node->value, dist);
                printf(" %d, %f ", v_node->value, dist);
                count_n++;
            }
            v_node = vec_next(same_f_values, v_node);

            if (v_node == VECTOR_EOF) break;
            
        }
        printf("\n");
    }
    free(vec_x);
    free(vec_j);

    Map filter_medoids = FindMedoid(dataset, vecs, min_f, max_f, filtered_data, t);

    // create a random permutation of 1...vecs (really from 0 to vecs-1) 
    // and store it in the array per
    int* per = (int*)malloc(vecs * sizeof(int*));
    for (int i=0; i<vecs; i++) {
        per[i] = i;
    }

    srand((unsigned int)time(NULL));

    // this is done by swaping the index in the ith position 
    // with that in the position given by randIdx
    for(int i=0; i<vecs; ++i){
        int randIdx = rand() % (vecs - 1);
        // swap per[i] with per[randIdx]
        int t = per[i];
        per[i] = per[randIdx];
        per[randIdx] = t;
    }

    printf("random permutation:\n");
    for (int i=0; i<vecs; i++) {
        printf("%d ", per[i]);
    }
    printf("\n");

    float* xq = (float*)malloc(comps * sizeof(float));
    for (int i=0; i<vecs; i++) {

        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        printf("query vector %d: ", query_pos);
        for (int i=0; i<comps; i++) {
            xq[i] = dataset[i][query_pos];
            printf("%f ", xq[i]);
        }
        printf("\n");

        // find the filter of the vector with possition per[i]
        int query_fltr = (int)dataset[0][query_pos];
        // and retrieve the start node of said filter from the filter_medoids map
        int s = vec_get_at(map_find_values(filter_medoids, query_fltr), 0);

        Set V;
        PQueue knn = FilteredGreedySearch(G, R, comps, vecs, dataset, xq, query_fltr, filter_medoids, L, 0, &V);

        set_destroy(V);
        pqueue_destroy(knn);

    }

    map_destroy(filtered_data);
    free_matrix_fvecs(dist_matrix, vecs);


    return G;
}


