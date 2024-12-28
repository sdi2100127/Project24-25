#include "filtered_algorithms.h"
#include <float.h>

float euclidean_distance_f(float* vec1, float* vec2, int comps) {
    float res = 0.0f;
    for (int i = 2; i < comps; i++) {
        float diff = vec1[i] - vec2[i];
        res += diff*diff;
    }
    res = sqrt(res);
    return res;
}

float squared_euclidean_distance_f(float* vec1, float* vec2, int comps) {
    float res = 0.0f;
    for (int i = 2; i < comps; i++) {
        float diff = vec1[i] - vec2[i];
        res += diff*diff;
    }
    
    return res;
}

PQueue FilteredGreedySearch(Vector* G, int R, int dim, int vecs, float** vectors, float* xq, int fq, Map filter_med, int med, int L, int k, Set* V) {
    printf("GREEDY SEARCH\n");

    // initialise knn and visited set as empty
    PQueue knn = pqueue_create(NULL);
    Set visited_nodes = set_Create();

    // since the query xq has only one filter we can simply check which starting point corresponds with this flter in the map
    Vector values = map_find_values(filter_med, fq);
    int s;

    // if the filter of the query is not in the dataset map return NULL 
    // as there are not any nodes with the same filter in the dataset
    if (values == NULL) {
        return NULL;
    }

    s = vec_get_at(values, 0);

    //printf("s: %d\n", s);

    // then insert it into the k-nearest-neighbours priority queue
    float* vec_s = (float*)malloc((dim-2) * sizeof(float));
    //printf("vec_s: ");
    int count = 0;
    for (int i=2; i<dim; i++) {
        vec_s[count] = vectors[i][s];
        //printf("%f ", vec_s[count]);
        count++;
    }
    //printf("\n");
    float dist_s = euclidean_distance(vec_s, xq, dim-2);
    pqueue_insert(knn, s, dist_s);

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
        // (or all if xq has no filter)
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

    //if (vec_find_node(knn->vector, s) == VECTOR_EOF) pqueue_insert(knn, s, dist_s);

    printf("knn_set: ");
    for (VecNode node = vec_first(knn->vector); node != VECTOR_EOF; node = vec_next(knn->vector, node)) { 
        printf("%d -> %f  ", node->value , vectors[0][node->value]);
    }
    printf("\n");

    // printf("visited_set: ");
    // for (set_Node node = find_min(visited_nodes->root); node != SET_EOF; node = set_next(visited_nodes, node)) { 
    //     printf("%d ", node->value);
    // }
    printf("\n");

    *V = visited_nodes;

    free(vec_s);
    free(dist_matrix);
    free(vec_p);

    return knn;
}

void FilteredRobustPrune(Vector** G, int p ,Set* V, int a, int R , int dim , int vecs , float** vectors, float** dist_m) {
    printf("ROBUST PRUNE\n");

    // we start out by creating some temporary structs to store the visited nodes set, as well as the graph G
    Set temp = *V;
    Vector* temp_G = *G;

    // then we insert all the outgoing neighbours of p (Nout(p)) to the V set
    //printf("Nout(p): ");
    for (int i = 0; i < temp_G[p]->size; i++){           
        //printf("%d ", vec_get_at(temp_G[p], i));
        set_insert(temp, vec_get_at(temp_G[p], i));
    }
    //printf("\n");
    // And we check if we have inserted our p so that we remove it
    set_remove(temp, p);   

    // printf("V: ");
    // for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) {
    //     printf("%d ", node->value);
    // }
    // printf("\n");     

    int prev_p_star = -1;
    float prev_min_d = FLT_MIN;                   

    // Now we have to empty our Nout(p)
    while(temp_G[p]->size != 0) vec_remove(temp_G[p]);

    while(temp->size != 0){
        //Now we have to find which vector of V has minimum distance to our point p
        float min_dist;        
        int p_star = -1;

        for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) {
            int node_value = node->value;
            float dist = dist_m[node_value][p];
            if (p_star == -1 || dist <= min_dist) {
                if (dist <= prev_min_d || node_value == prev_p_star) continue;
                else {
                    p_star = node_value;
                    min_dist = dist;
                }
            }
        }

        prev_p_star = p_star;
        prev_min_d = min_dist;
        
        // insert p* to the outgoing neighbours of p
        vec_insert(temp_G[p], p_star, min_dist);

        // if we found R neighbours --> stop
        if(temp_G[p]->size == R || temp_G[p]->size >= temp->size)
            break;

        // for every point p' in V perform the necessary pruning by removing certain points
        set_Node next = find_min(temp->root);
        while(next != SET_EOF){
            int node_value = next->value;
            if(vectors[0][node_value] == vectors[0][p] && vectors[0][p] == vectors[0][p_star]) {
                if(a * dist_m[node_value][p_star] <= dist_m[node_value][p]){
                    next = set_next(temp, next);
                    set_remove(temp,node_value);
                } else {
                    next = set_next(temp, next);
                }
            } else {next = set_next(temp, next);}
        
        }

    }

    // printf("V: ");
    // for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) {
    //     printf("%d ", node->value);
    // }
    // printf("\n");

    printf("Nout(%d): ", p);
    for (int i=0; i<temp_G[p]->size; i++) {
        printf("%d ", vec_get_at(temp_G[p], i));
    }
    printf("\n");

    *V = temp;
    *G = temp_G;
    
    return;
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

Map FindMedoid(float** dataset, int vecs, float min_f, float max_f, Map filtered_data, int t) {   
    printf("FIND MEDOID\n");

    // initialize M as an empty map
    // M maps filters to start nodes
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

        if (t >= P_f->size) {
            for (VecNode v_node = vec_first(P_f); v_node != VECTOR_EOF; v_node = vec_next(P_f, v_node)) { 
                set_insert(R_f, v_node->value);
            }
        } else {
            while (R_f->size < t) {
                if (P_f->size > 1) {
                    int x = rand() % (P_f->size - 1);
                    set_insert(R_f, vec_get_at(P_f, x));
                } else {
                    set_insert(R_f, vec_get_at(P_f, 0));
                }

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

Vector* FilteredVamanaIndexing(float** dataset, float min_f, float max_f, int vecs, int comps, int q_comps, int L, int R, int a, Map* med, int* medoid, int t) {
    
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

    // now we find the medoid of the dataset
    int fmedoid = FilteredMedoid(dataset, vecs, comps, &dist_matrix);
    printf("vamana found medoid: %d\n\n", fmedoid);
    *medoid = fmedoid;

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
        //printf("filter %d: ", node->key);
        values = node->values;
        for (VecNode s_node = vec_first(values); s_node != VECTOR_EOF; s_node = vec_next(values, s_node)) {
            //printf("%d ", s_node->value);
        }
        //printf("\n");
    }
    printf("vamana filtered data\n\n");

    Map filter_medoids = FindMedoid(dataset, vecs, min_f, max_f, filtered_data, t);
    printf("found starting points\n\n");
    *med = filter_medoids;

    // create a random permutation of 1...vecs (really from 0 to vecs-1) 
    // and store it in the array per
    int* per = (int*)malloc(vecs * sizeof(int));
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
    printf("\n\n");

    //int query_size = q_comps-4;
    float* xq = (float*)malloc((comps-2) * sizeof(float));
    float* point_vec = (float*)malloc((comps-2) * sizeof(float));

    for (int i=0; i<vecs; i++) {

        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        printf("query vector %d: ", query_pos);
        int c = 0;
        for (int i=2; i<comps; i++) {
            xq[c] = dataset[i][query_pos];
            c++;
            printf("%f ", xq[i]);
        }
        printf("\n");

        // find the filter of the vector with possition per[i]
        int query_fltr = (int)dataset[0][query_pos];
        // and retrieve the start node of said filter from the filter_medoids map
        int s = vec_get_at(map_find_values(filter_medoids, query_fltr), 0);

        Set V;
        PQueue knn = FilteredGreedySearch(G, R, comps, vecs, dataset, xq, query_fltr, filter_medoids, fmedoid, L, 0, &V);
        FilteredRobustPrune(&G, query_pos, &V, a, R, comps, vecs, dataset, dist_matrix);

        set_destroy(V);
        pqueue_destroy(knn);

        for (int j=0; j<G[query_pos]->size; j++) {
            
            // for each point j that is an outgoing neighbour of the query point
            int point = vec_get_at(G[query_pos], j);
            printf("Neighbour %d has %d neighbours\n", point, G[query_pos]->size);

            // int c_i = 0;
            // for (int i=2; i<comps; i++) {
            //     point_vec[c_i] = dataset[i][point];
            //     c_i++;
            // }

            // // update Nout of j with the query point
            // vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps-2));

            // // now check the size of Nout
            // if (G[point]->size == R) {

            //     // we create a set Nout_j with the outgoing neighbours of j as well as the current query point
            //     Set Nout_j = set_Create();
            //     for (int n=0; n<G[point]->size; n++) {
            //         set_insert(Nout_j, vec_get_at(G[point], n));
            //     }
            //     set_insert(Nout_j, query_pos);
            //     // and we call robust prune
            //     FilteredRobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);

            //     set_destroy(Nout_j);

            // } 

            // OLD WAY
            // check if it has less outgoing neighbours than R 
            // so that we can also add the query point as a neighbour

            // if we cannot
            if (G[point]->size == R) {

                // we create a set Nout_j with the outgoing neighbours of j as well as the current query point
                Set Nout_j = set_Create();
                for (int n=0; n<G[point]->size; n++) {
                    set_insert(Nout_j, vec_get_at(G[point], n));
                }
                set_insert(Nout_j, query_pos);
                // and we call robust prune
                FilteredRobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);
                set_destroy(Nout_j);

            } else {    // if the query point fits and is not already included
                // add it to the neighbours of j
                int flag = 1;
                for (int n=0; n<G[point]->size; n++) {
                    if (vec_get_at(G[point], n) == query_pos) {flag = 0; break;}
                }

                for (int i=2; i<comps; i++) {
                    point_vec[i] = dataset[i][point];
                }

                if (flag == 1) vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps-2));
                
            }


        }

    }

    free(xq);
    free(point_vec);
    free(per);
    map_destroy(filtered_data);
    free_matrix_fvecs(dist_matrix, vecs);


    return G;
}

Vector* FilteredVamanaIndexing_randomG(float** dataset, float min_f, float max_f, int vecs, int comps, int q_comps, int L, int R, int neigh, int a, Map* med, int* medoid, int t) {
    
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

    // now we find the medoid of the dataset
    int fmedoid = FilteredMedoid(dataset, vecs, comps, &dist_matrix);
    printf("vamana found medoid: %d\n\n", fmedoid);
    *medoid = fmedoid;

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
        //printf("filter %d: ", node->key);
        values = node->values;
        for (VecNode s_node = vec_first(values); s_node != VECTOR_EOF; s_node = vec_next(values, s_node)) {
            //printf("%d ", s_node->value);
        }
        //printf("\n");
    }
    printf("vamana filtered data\n\n");

    // CODE TO CREATE A RANDOM STARTING GRAPH !!!

    printf("creating random starting graph\n\n");

    int x;
    //printf("%d");
    float* vec_x = (float*)malloc((comps-2) * sizeof(float));
    float* vec_j = (float*)malloc((comps-2) * sizeof(float));

    // for every vector in the dataset
    for (int j = 0; j < vecs; j++) {
        int count_j = 0;
        for (int i=2; i<comps; i++) {
            vec_j[count_j] = dataset[i][j];
            count_j++;
        }
        //printf("vector %d:", j);

        // find its filter
        int filter = dataset[0][j];
        Vector same_f_values = map_find_values(filtered_data, filter);
    
        // for every one of its neigh neighbours, or until all the same filter points have been checked
        int count_n = 0;
        VecNode v_node = vec_first(same_f_values);
        while (count_n < neigh && count_n < same_f_values->size) {
            // insert them in the vector of out going neighbours of vector j
            x = v_node->value;
            if (x != j) {
                // temporarilly store the vectors j and x to compute their distance
                int count_x = 0;
                for (int i=2; i<comps; i++) {
                    vec_x[count_x] = dataset[i][x];
                    count_x++;
                }
                float dist = euclidean_distance(vec_j, vec_x, comps-2);
                vec_insert(G[j], v_node->value, dist);
                //printf(" %d, %f ", v_node->value, dist);
                count_n++;
            }
            v_node = vec_next(same_f_values, v_node);

            if (v_node == VECTOR_EOF) break;
            
        }
        //printf("\n");
    }
    free(vec_x);
    free(vec_j);

    printf("found random starting graph\n\n");

    Map filter_medoids = FindMedoid(dataset, vecs, min_f, max_f, filtered_data, t);
    printf("found starting points\n\n");
    *med = filter_medoids;

    // create a random permutation of 1...vecs (really from 0 to vecs-1) 
    // and store it in the array per
    int* per = (int*)malloc(vecs * sizeof(int));
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
    printf("\n\n");

    //int query_size = q_comps-4;
    float* xq = (float*)malloc((comps-2) * sizeof(float));
    float* point_vec = (float*)malloc((comps-2) * sizeof(float));

    for (int i=0; i<vecs; i++) {

        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        printf("query vector %d: ", query_pos);
        int c = 0;
        for (int i=2; i<comps; i++) {
            xq[c] = dataset[i][query_pos];
            c++;
            printf("%f ", xq[i]);
        }
        printf("\n");

        // find the filter of the vector with possition per[i]
        int query_fltr = (int)dataset[0][query_pos];
        // and retrieve the start node of said filter from the filter_medoids map
        int s = vec_get_at(map_find_values(filter_medoids, query_fltr), 0);

        Set V;
        PQueue knn = FilteredGreedySearch(G, R, comps, vecs, dataset, xq, query_fltr, filter_medoids, fmedoid, L, 0, &V);
        FilteredRobustPrune(&G, query_pos, &V, a, R, comps, vecs, dataset, dist_matrix);

        set_destroy(V);
        pqueue_destroy(knn);

        for (int j=0; j<G[query_pos]->size; j++) {
            
            // for each point j that is an outgoing neighbour of the query point
            int point = vec_get_at(G[query_pos], j);
            printf("Neighbour %d has %d neighbours\n", point, G[query_pos]->size);

            // int c_i = 0;
            // for (int i=2; i<comps; i++) {
            //     point_vec[c_i] = dataset[i][point];
            //     c_i++;
            // }

            // // update Nout of j with the query point
            // vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps-2));

            // // now check the size of Nout
            // if (G[point]->size == R) {

            //     // we create a set Nout_j with the outgoing neighbours of j as well as the current query point
            //     Set Nout_j = set_Create();
            //     for (int n=0; n<G[point]->size; n++) {
            //         set_insert(Nout_j, vec_get_at(G[point], n));
            //     }
            //     set_insert(Nout_j, query_pos);
            //     // and we call robust prune
            //     FilteredRobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);

            //     set_destroy(Nout_j);

            // } 

            // OLD WAY
            // check if it has less outgoing neighbours than R 
            // so that we can also add the query point as a neighbour

            // if we cannot
            if (G[point]->size == R) {

                // we create a set Nout_j with the outgoing neighbours of j as well as the current query point
                Set Nout_j = set_Create();
                for (int n=0; n<G[point]->size; n++) {
                    set_insert(Nout_j, vec_get_at(G[point], n));
                }
                set_insert(Nout_j, query_pos);
                // and we call robust prune
                FilteredRobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);
                set_destroy(Nout_j);

            } else {    // if the query point fits and is not already included
                // add it to the neighbours of j
                int flag = 1;
                for (int n=0; n<G[point]->size; n++) {
                    if (vec_get_at(G[point], n) == query_pos) {flag = 0; break;}
                }

                for (int i=2; i<comps; i++) {
                    point_vec[i] = dataset[i][point];
                }

                if (flag == 1) vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps-2));
                
            }


        }

    }

    free(xq);
    free(point_vec);
    free(per);
    map_destroy(filtered_data);
    free_matrix_fvecs(dist_matrix, vecs);


    return G;
}

Vector** StichedVamanaIndexing(float** dataset, float min_f, float max_f, Set filters, int vecs, int comps, int L, int R_stitched, int a, Map* med, int* medoid, int t, Map* filt_data, Vector** permutation) {
    int num_filters = filters->size;
    
    // create an array of graphs, a graph for each filter of the dataset
    Vector** G_f = (Vector**)malloc(num_filters * sizeof(Vector*));
    for (int i = 0; i < num_filters; ++i) {
        G_f[i] = (Vector*)malloc(vecs * sizeof(Vector));
        for (int j=0; j<vecs; j++) {
            G_f[i][j] = vec_Create(0);
        }
    }

    printf("initialized random empty graphs\n\n");

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
        //printf("filter %d: ", node->key);
        values = node->values;
        int c_f = 0;
        for (VecNode s_node = vec_first(values); s_node != VECTOR_EOF; s_node = vec_next(values, s_node)) {
            //printf("%d ", s_node->value);
            c_f++;
        }
        //printf("filter %d: %d\n",node->key, c_f);
        //printf("\n");
    }

    printf("vamana filtered data\n\n");
    

    *filt_data = filtered_data;

    Map filter_medoids = FindMedoid(dataset, vecs, min_f, max_f, filtered_data, t);
    printf("found starting points\n\n");
    *med = filter_medoids;

    // stores the original permutation of each filter's points in filtered_data
    Vector* per = (Vector*)malloc(num_filters * sizeof(Vector));
    for (int i=0; i<num_filters; i++) {
        per[i] = vec_Create(0);
    }

    //for (set_Node node = find_min(filters->root); node != SET_EOF; node = set_next(filters, node)) { 
    for (int f=0; f<num_filters; f++) {
        // keep the current filter, the values associated with it, and how many there are
        // int f = node->value; 
        int f_size = map_find_values(filtered_data, f)->size;
        if (f_size == 1) continue;

        Vector f_values = map_find_values(filtered_data, f);

        // next keep the permutation of the original indices of the points vs the indices in the filtered dataset
        // for (int j=0; j<f_size; j++) {
        //     vec_insert(per[f], vec_get_at(f_values, j), vec_get_dist(f_values, j));
        // }

        for (VecNode s_node = vec_first(f_values); s_node != VECTOR_EOF; s_node = vec_next(f_values, s_node)) {
            vec_insert(per[f], s_node->value, s_node->dist);
        }

        // printf("per: ");
        // for (VecNode s_node = vec_first(per[f]); s_node != VECTOR_EOF; s_node = vec_next(per[f], s_node)) {
        //     printf("%d ", s_node->value);
        // }
        // printf("\n");

        printf("filter: %d\n", f);

        printf("random stitched permutation:\n");
        for (int j=0; j<f_size; j++) {
            printf("%d ", vec_get_at(per[f], j));
        }
        printf("\n");

        // and store them in the new dataset
        int c = comps-2;
        float** data_f = (float**)malloc(c * sizeof(float*));
        for (int i = 0; i < c; i++) {
            data_f[i] = (float*)malloc(f_size * sizeof(float));
        }

        for (int j = 0; j < f_size; j++) {
            int count = 0;
            for (int i = 2; i < comps; i++) {
                //printf("%f \n", dataset[i][vec_get_at(per[f], j)]);
                data_f[count][j] = dataset[i][vec_get_at(per[f], j)];
                count++;
            }
        }

        for (int j = 0; j < f_size; j++) {
            printf("vector %d -> %d: ", j, vec_get_at(per[f], j));
            for (int i = 0; i < c; i++) {
                printf("%f ", data_f[i][j]);
            }
            printf("\n\n");
        }

        int Vmedoid;

        if (f_size == 2) {
            int pos = 1;
            for (int j = 0; j < f_size; j++) {
                float* vec_x = (float*)malloc(c * sizeof(float));
                float* vec_j = (float*)malloc(c * sizeof(float));
                for (int i=0; i<c; i++) {
                    vec_x[i] = data_f[i][j];
                    vec_j[i] = data_f[i][pos];
                }
                
                vec_insert(G_f[f][j], vec_get_at(per[f], j), euclidean_distance(vec_x, vec_j, c));
                pos--;
            }
        } else {
            if (R_stitched >= f_size) {
                printf("R: %d\n", f_size);
                G_f[f] = Vamana_main(data_f, f_size, comps-2, L, f_size-1, a, &Vmedoid);
            } else {
                printf("R: %d\n", R_stitched);
                G_f[f] = Vamana_main(data_f, f_size, comps-2, L, R_stitched, a, &Vmedoid);
            }
        }

        free_matrix_fvecs(data_f, comps-2);

    }

    // for (int j=0; j<vecs; j++) {
    //     FilteredRobustPrune()
    // }

    *medoid = -1;

    *permutation = per;
    return G_f;
}

Vector** StichedVamanaIndexing_randomG(float** dataset, float min_f, float max_f, Set filters, int vecs, int comps, int L, int R_stitched, int a, Map* med, int* medoid, int t, int neigh, Map* filt_data, Vector** permutation) {
    int num_filters = filters->size;
    
    // create an array of graphs, a graph for each filter of the dataset
    Vector** G_f = (Vector**)malloc(num_filters * sizeof(Vector*));
    for (int i = 0; i < num_filters; ++i) {
        G_f[i] = (Vector*)malloc(vecs * sizeof(Vector));
        for (int j=0; j<vecs; j++) {
            G_f[i][j] = vec_Create(0);
        }
    }

    printf("initialized random empty graphs\n\n");

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
        //printf("filter %d: ", node->key);
        values = node->values;
        int c_f = 0;
        for (VecNode s_node = vec_first(values); s_node != VECTOR_EOF; s_node = vec_next(values, s_node)) {
            //printf("%d ", s_node->value);
            c_f++;
        }
        //printf("filter %d: %d\n",node->key, c_f);
        //printf("\n");
    }

    printf("vamana filtered data\n\n");
    

    *filt_data = filtered_data;

    Map filter_medoids = FindMedoid(dataset, vecs, min_f, max_f, filtered_data, t);
    printf("found starting points\n\n");
    *med = filter_medoids;

    // stores the original permutation of each filter's points in filtered_data
    Vector* per = (Vector*)malloc(num_filters * sizeof(Vector));
    for (int i=0; i<num_filters; i++) {
        per[i] = vec_Create(0);
    }

    //for (set_Node node = find_min(filters->root); node != SET_EOF; node = set_next(filters, node)) { 
    for (int f=0; f<num_filters; f++) {
        // keep the current filter, the values associated with it, and how many there are
        // int f = node->value; 
        int f_size = map_find_values(filtered_data, f)->size;
        if (f_size == 1) continue;

        Vector f_values = map_find_values(filtered_data, f);

        for (VecNode s_node = vec_first(f_values); s_node != VECTOR_EOF; s_node = vec_next(f_values, s_node)) {
            vec_insert(per[f], s_node->value, s_node->dist);
        }

        // printf("per: ");
        // for (VecNode s_node = vec_first(per[f]); s_node != VECTOR_EOF; s_node = vec_next(per[f], s_node)) {
        //     printf("%d ", s_node->value);
        // }
        // printf("\n");

        printf("filter: %d\n", f);

        printf("random stitched permutation:\n");
        for (int j=0; j<f_size; j++) {
            printf("%d ", vec_get_at(per[f], j));
        }
        printf("\n");

        // and store them in the new dataset
        int c = comps-2;
        float** data_f = (float**)malloc(c * sizeof(float*));
        for (int i = 0; i < c; i++) {
            data_f[i] = (float*)malloc(f_size * sizeof(float));
        }

        for (int j = 0; j < f_size; j++) {
            int count = 0;
            for (int i = 2; i < comps; i++) {
                //printf("%f \n", dataset[i][vec_get_at(per[f], j)]);
                data_f[count][j] = dataset[i][vec_get_at(per[f], j)];
                count++;
            }
        }

        for (int j = 0; j < f_size; j++) {
            printf("vector %d -> %d: ", j, vec_get_at(per[f], j));
            for (int i = 0; i < c; i++) {
                printf("%f ", data_f[i][j]);
            }
            printf("\n\n");
        }

        // CODE TO CREATE A RANDOM STARTING GRAPH !!!

        printf("creating random starting graph\n\n");

        int x;
        //printf("%d");
        float* vec_x = (float*)malloc((comps-2) * sizeof(float));
        float* vec_j = (float*)malloc((comps-2) * sizeof(float));

        // for every vector in the dataset
        for (int j = 0; j < f_size; j++) {
            for (int i=0; i<c; i++) {
                vec_j[i] = data_f[i][j];
            }
            //printf("vector %d:", j);

            // find its filter
            int filter = data_f[0][j];
            //Vector same_f_values = map_find_values(filtered_data, filter);
        
            // for every one of its neigh neighbours, or until all the same filter points have been checked
            int count_n = 0;
            VecNode v_node = vec_first(f_values);
            while (count_n < neigh && count_n < f_values->size) {
                // insert them in the vector of out going neighbours of vector j
                x = v_node->value;
                if (x != j) {
                    // temporarilly store the vectors j and x to compute their distance
                    for (int i=0; i<c; i++) {
                        vec_x[i] = data_f[i][x];
                    }
                    float dist = euclidean_distance(vec_j, vec_x, c);
                    vec_insert(G_f[f][j], v_node->value, dist);
                    //printf(" %d, %f ", v_node->value, dist);
                    count_n++;
                }
                v_node = vec_next(f_values, v_node);

                if (v_node == VECTOR_EOF) break;
                
            }
            //printf("\n");
        }
        free(vec_x);
        free(vec_j);

        printf("found random starting graph\n\n");

        int Vmedoid;

        if (f_size == 2) {
            int pos = 1;
            for (int j = 0; j < f_size; j++) {
                float* vec_x = (float*)malloc(c * sizeof(float));
                float* vec_j = (float*)malloc(c * sizeof(float));
                for (int i=0; i<c; i++) {
                    vec_x[i] = data_f[i][j];
                    vec_j[i] = data_f[i][pos];
                }
                
                vec_insert(G_f[f][j], vec_get_at(per[f], j), euclidean_distance(vec_x, vec_j, c));
                pos--;
            }
        } else {
            if (R_stitched >= f_size) {
                printf("R: %d\n", f_size);
                G_f[f] = Vamana_main(data_f, f_size, comps-2, L, f_size-1, a, &Vmedoid);
            } else {
                printf("R: %d\n", R_stitched);
                G_f[f] = Vamana_main(data_f, f_size, comps-2, L, R_stitched, a, &Vmedoid);
            }
        }

        free_matrix_fvecs(data_f, comps-2);

    }

    // for (int j=0; j<vecs; j++) {
    //     FilteredRobustPrune()
    // }

    *medoid = -1;

    *permutation = per;
    return G_f;
}

Vector* Groundtruth(float** dataset, int vecs, int comps, float** queries, int vecs_q, int comps_q, int k) {
    printf("GROUDTRUTH\n");

    if ((comps-2) != (comps_q-4)) {
        printf("Error: queries and data vectors have different number of components!\n");
        return NULL;
    }

    // int** groundtruth = (int**)malloc(k * sizeof(int*));
    // for (int i = 0; i < k; i++) {
    //     groundtruth[i] = (int*)malloc(vecs_q * sizeof(int));
    // }
    Vector* groundtruth = (Vector*)malloc(vecs_q * sizeof(Vector));
    for (int i=0; i<vecs_q; i++) {
        groundtruth[i] = vec_Create(0);
    }

    float* vec_q = (float*)malloc((comps-2) * sizeof(float));
    float* vec_p2 = (float*)malloc((comps-2) * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;
    float dist;

    // for each vector in the queries dataset
    for (int j=0; j<vecs_q; j++) {
        //printf("query %d\n", j);

        PQueue knn = pqueue_create(NULL);
 
        int count = 0;
        for (int i=4; i<comps_q; i++) {
            vec_q[count] = queries[i][j];
            count++;
        }

        
        // compute its distance to all other vectors in the dataset
        for (int z=0; z<vecs; z++) {
            count = 0;
            for (int i=2; i<comps; i++) {
                vec_p2[count] = dataset[i][z];
                count++;
            }
            // and add them to a priority queue based on their distance AND filter
            if (queries[0][j] == 0) {
                // if query type is zero, take into account vectors of all filters
                pqueue_insert(knn, z, squared_euclidean_distance(vec_q, vec_p2, comps-2));
            } else if (queries[0][j] == 1) {
                // its one, check the vector's filter so that it matches
                if (queries[1][j] == dataset[0][z]) pqueue_insert(knn, z, squared_euclidean_distance(vec_q, vec_p2, comps-2));
            }
            
        }

        // remove the most distant neighbours of the query from knn until its equal to k
        while (knn->vector->size > k) {
            pqueue_remove(knn);
        }

        // then finally add the k nearest nodes to the grountruth matrix
        // count = 0;
        // for (VecNode node = vec_first(knn->vector); node != VECTOR_EOF; node = vec_next(knn->vector, node)) {
        //     groundtruth[count][j] = node->value;
        //     count++;
        //     if (count >= k) break;
        // }

        for (VecNode node = vec_first(knn->vector); node != VECTOR_EOF; node = vec_next(knn->vector, node)) {
            //printf("value: %d, dist: %f\n", node->value, node->dist);
            vec_insert(groundtruth[j], node->value, node->dist);
        }
        
        pqueue_destroy(knn);

    }

    free(vec_q);
    free(vec_p2);

    // PRINTS FOR TESTING !!!

    // for (int j=0; j<vecs_q && j<20; j++) {
    //     printf("query %d with filter %f:", j, queries[1][j]);
    //     for (VecNode node = vec_first(groundtruth[j]); node != VECTOR_EOF; node = vec_next(groundtruth[j], node)) {
    //         int idx = node->value;
    //         printf("%d - %f, ", idx, dataset[0][idx]);
    //     }
    //     printf("\n");
    // }

    return groundtruth;
}

void free_G_f(Vector** G_f, int num_filters, Map filtered_data) {
    for (int i = 0; i < num_filters; i++) {
        int vecs = map_find_values(filtered_data, i)->size;
        for (int j = 0; j < vecs; j++) {
            vec_destroy(G_f[i][j]);
        }
        
        free(G_f[i]);
    }
    
    free(G_f);
}
