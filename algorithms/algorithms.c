#include "algorithms.h"
#include <float.h>

PQueue greedySearch(Vector* G, int R, int dim, int vecs, float** vectors, int s, float* xq, int L, int k, Set* V) {
    //printf("GREEDY SEARCH\n");

    //printf("G[%d].size: %d\n", s, G[s]->size);

    // initializing the knn set with the starting point s
    PQueue knn = pqueue_create(NULL);
    float* vec_s = (float*)malloc(dim * sizeof(float));
    for (int i=0; i<dim; i++) {
        vec_s[i] = vectors[i][s];
    }
    pqueue_insert(knn, s, euclidean_distance(vec_s, xq, dim));

    // initializing the empty set for visited nodes
    Set visited_nodes = set_Create();

    int flag = 1;

    int node_value, xp = -1;
    float min_dist;

    float* vec_p = (float*)malloc(dim * sizeof(float));

    //create a distance matrix with the euclidean distance of each vector to xq and initiallize it to -1
    float* dist_matrix = (float*)malloc(vecs * sizeof(float*));
    for (int j = 0; j < vecs; j++) {
        dist_matrix[j] = -1;
    }

    float prev_max; 
    int prev_max_pos;

    while (flag == 1) {
        // we check the flag up front in case we do not have any unvisited nodes in the first iteration
        flag = 0;
        // for every iteration we have to reinitiallize the minimum
        xp = -1;
        // check for all the unvisited nodes on the knn_set
        Vector knn_vec = knn->vector;
        for (VecNode node = vec_first(knn_vec); node != VECTOR_EOF; node = vec_next(knn_vec, node)) {
            node_value = node->value;
            //printf("node_value: %d\n", node_value);
            if (S_find_equal(visited_nodes->root, node_value) == SET_EOF) {
                flag = 1;   // unvisited nodes where indeed found              

                // if the next node xp is not initialized yet initialize it with the first unvisited node you encounter
                // otherwise check if its distance to xq is smaller than the current min_dist
                if (dist_matrix[node_value] == -1) {    // if the distance has not been computed before, compute it
                    for (int i=0; i<dim; i++) {
                        vec_p[i] = vectors[i][node_value];
                    }
                    dist_matrix[node_value] = euclidean_distance(vec_p, xq, dim);
                }
                float dist = dist_matrix[node_value];

                if (xp == -1 || dist <= min_dist) {
                    xp = node_value;
                    min_dist = dist;
                }
            }
            
        }
        //printf("xp: %d, flag: %d\n", xp, flag);

        // if there where no more unvisited nodes found there is no point in continuing with the loop
        if (flag == 0) break; 

        // now we add all the outgoing neighbours of the node xp in the knn_set
        // the outgoing neighbours of a node are the one that are included in said node's column
        // the incoming neighbours are the other node's columns that the node is part of
        //printf("G[%d].size: %d\n", xp, G[xp]->size);
        for (int i=0; i<G[xp]->size; i++) {
            int point = vec_get_at(G[xp], i);
            //printf("point: %d\n", point);
            // if the distance has not been computed before, compute it
            if (dist_matrix[point] == -1) {
                for (int i=0; i<dim; i++) {
                    vec_p[i] = vectors[i][point];
                }
                dist_matrix[point] = euclidean_distance(vec_p, xq, dim);
                //printf("dist: %f\n", dist_matrix[point]);
            }
            pqueue_insert(knn, point, dist_matrix[point]);
        }

        // add node xp to te visited_nodes set
        set_insert(visited_nodes, xp);

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

    // printf("knn_set: ");
    // for (VecNode node = vec_first(knn->vector); node != VECTOR_EOF; node = vec_next(knn->vector, node)) { 
    //     printf("%d ", node->value);
    // }
    // printf("\n");

    // printf("visited_set: ");
    // for (set_Node node = find_min(visited_nodes->root); node != SET_EOF; node = set_next(visited_nodes, node)) { 
    //     printf("%d ", node->value);
    // }
    // printf("\n");
    
    *V = visited_nodes;
    
    free(vec_s);
    free(dist_matrix);
    free(vec_p);

    return knn;

}

//Implementation of Robust Prune function
void RobustPrune(Vector** G, int p ,Set * V, int a, int R, int dim , int vecs , float **vectors, float** dist_m){
    // printf("ROBUST PRUNE\n");
    // printf("p: %d\n", p);
    
    // we start out by creating some temporary structs to store the visited nodes set, as well as the graph G
    Set temp = *V;
    Vector* temp_G = *G;

    // then we insert all the outgoing neighbours of p (Nout(p)) to the V set
    //printf("Nout(p)\n");
    for (int i = 0; i < temp_G[p]->size; i++){           
        //printf("%d ", vec_get_at(temp_G[p], i));
        set_insert(temp, vec_get_at(temp_G[p], i));
    }
    //printf("\n");
    // And we check if we have inserted our p so that we remove it
    set_remove(temp, p);   

    //printf("V: ");
    // for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) {
    //     printf("%d ", node->value);
    // }
    // printf("\n");                        

    // Now we have to empty our Nout(p)
    while(temp_G[p]->size != 0) vec_remove(temp_G[p]);

    int i_count = 0;    // to keep count of the Nout vector index

    while(temp->size != 0){
        //Now we have to find which vector of V has minimum distance to our point p
        float min_dist;        
        int p_star = -1;

        for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) {
            int node_value = node->value;
            float dist = dist_m[node_value][p];
            if (p_star == -1 || dist <= min_dist) {
                p_star = node_value;
                min_dist = dist;
            }
        }

        //printf("p*: %d\n", p_star);
        
        // insert p* to the outgoing neighbours of p
        // vec_insert(temp_G[p], p_star, euclidean_distance(vec_of_p_star, vec_of_p, dim));
        vec_insert(temp_G[p], p_star, min_dist);
        i_count ++;

        // if we found R neighbours --> stop
        if(i_count == R)
            break;

        // for every point p' in V perform the necessary pruning by removing certain points
        set_Node next = find_min(temp->root);
        while(next != SET_EOF){
            int node_value = next->value;
            if(a * dist_m[node_value][p_star] <= dist_m[node_value][p]){
                next = set_next(temp, next);
                set_remove(temp,node_value);
            } else {
                next = set_next(temp, next);
            }
        }

    }

    // printf("V: ");
    // for (set_Node node = find_min(temp->root); node != SET_EOF; node = set_next(temp, node)) {
    //     printf("%d ", node->value);
    // }
    // printf("\n");

    // printf("Nout(%d): ", p);
    // for (int i=0; i<temp_G[p]->size; i++) {
    //     printf("%d ", vec_get_at(temp_G[p], i));
    // }
    // printf("\n");
    // printf("size: %d\n", temp_G[p]->size);

    *V = temp;
    *G = temp_G;
    
    return;
}

int medoid(float** dataset, int vecs, int comps, float*** dist_m) {
    float** temp_m =  *dist_m;

    float* vec_p1 = (float*)malloc(comps * sizeof(float));
    float* vec_p2 = (float*)malloc(comps * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;
    float dist;
    float dist_sum;

    // for each vector in the dataset
    for (int j=0; j<vecs; j++) {

        dist_sum = 0.0f;
        for (int i=0; i<comps; i++) {
            vec_p1[i] = dataset[i][j];
        }

        
        // compute its distance to all other vectors
        for (int z=0; z<vecs; z++) {
            
            for (int i=0; i<comps; i++) {
                vec_p2[i] = dataset[i][z];
            }

            // and add it to the total sum
            if (j != z)  {
                // compute the distance only if it has not been computed before
                if (temp_m[j][z] == 0) {
                    dist = squared_euclidean_distance(vec_p1, vec_p2, comps);
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

int medoid_threads(float** dataset, int vecs, int comps, float*** dist_m, int threads_count) {
    printf("medoid_threads:\nBuilding distance matrix and computing the medoiod.\n");
    int num_threads = threads_count;
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int chunk_size = vecs / num_threads;

    float min_sum_total = FLT_MAX;
    int min_p_total = -1;

    for (int t = 0; t < num_threads; t++) {
        thread_data[t].dataset = dataset;
        thread_data[t].dist_m = *dist_m;
        thread_data[t].start = t * chunk_size;
        thread_data[t].end = (t == num_threads - 1) ? vecs : (t + 1) * chunk_size;
        thread_data[t].comps = comps;
        thread_data[t].vecs = vecs;
        thread_data[t].med = -1;
        thread_data[t].med_sum = FLT_MAX;
        pthread_create(&threads[t], NULL, calculate_distances_simple, (void*)&thread_data[t]);
    }

    // Join threads
    for (int t = 0; t < num_threads; ++t) {
        pthread_join(threads[t], NULL);
        if (thread_data[t].med_sum < min_sum_total) {
            min_sum_total = thread_data[t].med_sum;
            min_p_total = thread_data[t].med;
        }
    }

    printf("medoid sum: %f\n", min_sum_total);
    return min_p_total;
}

int random_medoid(float** dataset, int vecs, int comps, float*** dist_m) {
    srand((unsigned int)time(NULL));

    float** temp_m =  *dist_m;

    float* vec_p1 = (float*)malloc(comps * sizeof(float));
    float* vec_p2 = (float*)malloc(comps * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;
    float dist;
    float dist_sum;

    // for each vector in the dataset
    for (int j=0; j<vecs; j++) {

        dist_sum = 0.0f;
        for (int i=0; i<comps; i++) {
            vec_p1[i] = dataset[i][j];
        }

        
        // compute its distance to all other vectors
        for (int z=0; z<vecs; z++) {
            
            for (int i=0; i<comps; i++) {
                vec_p2[i] = dataset[i][z];
            }

            // and add it to the total sum
            if (j != z)  {
                // compute the distance only if it has not been computed before
                if (temp_m[j][z] == 0) {
                    dist = squared_euclidean_distance(vec_p1, vec_p2, comps);
                    temp_m[j][z] = dist;
                    temp_m[z][j] = dist;
                }
                dist_sum = dist_sum + temp_m[j][z];
            }
        }

        int x = rand() % (vecs - 1);
        if (x%2 == 0) {
            // if its an even number let it contribute to the medoid search
            // then find which vector has the lowest total sum
            if (dist_sum <= min_sum) {
                min_sum = dist_sum;
                min_p = j;
            }
        }
    
    }

    free(vec_p1);
    free(vec_p2);

    *dist_m = temp_m;

    printf("medoid sum: %f\n", min_sum);
    return min_p;
}

int random_medoid_threads(float** dataset, int vecs, int comps, float*** dist_m, int threads_count) {
    printf("random_medoid_threads:\nBuilding distance matrix and computing the medoiod.\n");
    int num_threads = threads_count;
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int chunk_size = vecs / num_threads;

    float min_sum_total = FLT_MAX;
    int min_p_total = -1;

    for (int t = 0; t < num_threads; t++) {
        thread_data[t].dataset = dataset;
        thread_data[t].dist_m = *dist_m;
        thread_data[t].start = t * chunk_size;
        thread_data[t].end = (t == num_threads - 1) ? vecs : (t + 1) * chunk_size;
        thread_data[t].comps = comps;
        thread_data[t].vecs = vecs;
        thread_data[t].med = -1;
        thread_data[t].med_sum = FLT_MAX;
        pthread_create(&threads[t], NULL, calculate_distances_simple, (void*)&thread_data[t]);
    }

    // Join threads
    for (int t = 0; t < num_threads; ++t) {
        pthread_join(threads[t], NULL);
        if (thread_data[t].med_sum < min_sum_total) {
            min_sum_total = thread_data[t].med_sum;
            min_p_total = thread_data[t].med;
        }
    }

    printf("medoid sum: %f\n", min_sum_total);
    return min_p_total;
}

Vector* Vamana_main(float** dataset, int vecs, int comps, int L, int R, int a, int* med, int threads) {
    printf("Vamana:\nBuilding the vamana index.\n\n");

    // first we have to initialize G to a random R-regular directed graph

    Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   
    for (int i=0; i<vecs; i++) {
        G[i] = vec_Create(0);
    }

    printf("Vamana finding neighbours.\n\n");
    int x;
    float* vec_x = (float*)malloc(comps * sizeof(float));
    float* vec_j = (float*)malloc(comps * sizeof(float));

    // for every vector in the dataset
    for (int j = 0; j < vecs; j++) {
        for (int i=0; i<comps; i++) {
            vec_j[i] = dataset[i][j];
        }
        //printf("vector %d:", j);

        int limit = vecs-1, min = 0;
        // for every one of its R neighbours
        for (int i = 0; i < R; i++) {

            int stop = 1;
            while (stop == 1) {
                x = rand() % (limit-min+1)+min;    // pick another vector randomly
                //printf("%d\n", x);
                stop = 0;
                // as long as that vector is not a neighbour already and it is not the same as our current vector
                if (vec_find_node(G[j], x) != VECTOR_EOF || x == j) {   
                    stop = 1;
                }
                if (x == limit) limit--;
                if (x == min) min++;
                // for (int z = 0; z < i; z++) {
                //     // as long as that vector is not a neighbour already and it is not the same as our current vector
                //     if (vec_find_node(G[j], x) != VECTOR_EOF || x != j) {   
                //         stop = 1;
                //         break;
                //     }
                // }    
            }
            // temporarilly store the vectors j and x to compute their distance
            for (int i=0; i<comps; i++) {
                vec_x[i] = dataset[i][x];
            }
            vec_insert(G[j], x, euclidean_distance(vec_x, vec_j, comps));

            //printf(" %d, %f ", G[j]->array[i].value, G[j]->array[i].dist);
            
        }
        //printf("\n");
    }
    free(vec_x);
    free(vec_j);

    // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
    float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
        dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
    }

    for(int i=0; i<vecs; i++) {
        for(int j=0; j<vecs; j++) {
            dist_matrix[i][j] = 0;
        }
    }

    // now we find the medoid of the dataset that will be our starting point s
    int s;
    if (threads != 0) s = medoid_threads(dataset, vecs, comps, &dist_matrix, threads);
    else if (threads == 0) s = medoid(dataset, vecs, comps, &dist_matrix);
    printf("Vamana found medoid: %d\n\n", s);
    *med = s;

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

    printf("Found random permutation\n\n");
    // for (int i=0; i<vecs; i++) {
    //     printf("%d ", per[i]);
    // }
    // printf("\n");

    float* xq = (float*)malloc(comps * sizeof(float));
    float* point_vec = (float*)malloc(comps * sizeof(float));

    printf("Going through each point of the dataset and updating the graph.\n\n");
    for (int i=0; i<vecs; i++) {
        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        //printf("query vector %d: ", query_pos);
        for (int i=0; i<comps; i++) {
            xq[i] = dataset[i][query_pos];
            //printf("%f ", xq[i]);
        }
        //printf("\n");
        
        // call greedysearch for xq
        Set V;
        PQueue knn = greedySearch(G, R, comps, vecs, dataset, s, xq, L, 1, &V);
        // then run robust prune
        RobustPrune(&G, query_pos, &V, a, R, comps, vecs, dataset, dist_matrix);

        set_destroy(V);
        pqueue_destroy(knn);

        for (int j=0; j<G[query_pos]->size; j++) {
            
            // for each point j that is an outgoing neighbour of the query point
            int point = vec_get_at(G[query_pos], j);
            //printf("Neighbour %d has %d neighbours\n", point, G[query_pos]->size);

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
                RobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);

                set_destroy(Nout_j);

            } else {    // if the query point fits and is not already included
                // add it to the neighbours of j
                int flag = 1;
                for (int n=0; n<G[point]->size; n++) {
                    if (vec_get_at(G[point], n) == query_pos) {flag = 0; break;}
                }

                for (int i=0; i<comps; i++) {
                    point_vec[i] = dataset[i][point];
                }

                if (flag == 1) vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps));
                
            }
        }
    }

    free(xq);
    free(point_vec);
    free_matrix_fvecs(dist_matrix, vecs);
    free(per);
    return G;
}

Vector* Vamana_random_medoid(float** dataset, int vecs, int comps, int L, int R, int a, int* med, int threads) {
    printf("Vamana:\nBuilding the vamana index.\n\n");

    // first we have to initialize G to a random R-regular directed graph

    Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   
    for (int i=0; i<vecs; i++) {
        G[i] = vec_Create(0);
    }

    printf("Vamana finding neighbours\n\n");
    int x;
    float* vec_x = (float*)malloc(comps * sizeof(float));
    float* vec_j = (float*)malloc(comps * sizeof(float));

    // for every vector in the dataset
    for (int j = 0; j < vecs; j++) {
        for (int i=0; i<comps; i++) {
            vec_j[i] = dataset[i][j];
        }
        //printf("vector %d:", j);

        int limit = vecs-1, min = 0;
        // for every one of its R neighbours
        for (int i = 0; i < R; i++) {

            int stop = 1;
            while (stop == 1) {
                x = rand() % (limit-min+1)+min;    // pick another vector randomly
                //printf("%d\n", x);
                stop = 0;
                // as long as that vector is not a neighbour already and it is not the same as our current vector
                if (vec_find_node(G[j], x) != VECTOR_EOF || x == j) {   
                    stop = 1;
                }
                if (x == limit) limit--;
                if (x == min) min++;
                // for (int z = 0; z < i; z++) {
                //     // as long as that vector is not a neighbour already and it is not the same as our current vector
                //     if (vec_find_node(G[j], x) != VECTOR_EOF || x != j) {   
                //         stop = 1;
                //         break;
                //     }
                // }    
            }
            // temporarilly store the vectors j and x to compute their distance
            for (int i=0; i<comps; i++) {
                vec_x[i] = dataset[i][x];
            }
            vec_insert(G[j], x, euclidean_distance(vec_x, vec_j, comps));

            //printf(" %d, %f ", G[j]->array[i].value, G[j]->array[i].dist);
            
        }
        //printf("\n");
    }
    free(vec_x);
    free(vec_j);

    // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
    float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
        dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
    }

    for(int i=0; i<vecs; i++) {
        for(int j=0; j<vecs; j++) {
            dist_matrix[i][j] = 0;
        }
    }

    // now we find the medoid of the dataset that will be our starting point s
    // and this time we do it by selecting a random point of the dataset
    srand((unsigned int)time(NULL));

    // we still have to call the medoid function so that the dist_matrix iis computed
    int s;
    if (threads != 0) s = medoid_threads(dataset, vecs, comps, &dist_matrix, threads);
    else if (threads == 0) s = medoid(dataset, vecs, comps, &dist_matrix);
    s = rand() % (vecs - 1);
    printf("Vamana found medoid: %d\n\n", s);
    *med = s;

    // create a random permutation of 1...vecs (really from 0 to vecs-1) 
    // and store it in the array per
    int* per = (int*)malloc(vecs * sizeof(int*));
    for (int i=0; i<vecs; i++) {
        per[i] = i;
    }

    //srand((unsigned int)time(NULL));

    // this is done by swaping the index in the ith position 
    // with that in the position given by randIdx
    for(int i=0; i<vecs; ++i){
        int randIdx = rand() % (vecs - 1);
        // swap per[i] with per[randIdx]
        int t = per[i];
        per[i] = per[randIdx];
        per[randIdx] = t;
    }

    printf("Found random permutation\n\n");
    // for (int i=0; i<vecs; i++) {
    //     printf("%d ", per[i]);
    // }
    // printf("\n");

    float* xq = (float*)malloc(comps * sizeof(float));
    float* point_vec = (float*)malloc(comps * sizeof(float));

    printf("Going through each point of the dataset and updating the graph.\n\n");
    for (int i=0; i<vecs; i++) {
        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        //printf("query vector %d: ", query_pos);
        for (int i=0; i<comps; i++) {
            xq[i] = dataset[i][query_pos];
            //printf("%f ", xq[i]);
        }
        //printf("\n");
        
        // call greedysearch for xq
        Set V;
        PQueue knn = greedySearch(G, R, comps, vecs, dataset, s, xq, L, 1, &V);
        // then run robust prune
        RobustPrune(&G, query_pos, &V, a, R, comps, vecs, dataset, dist_matrix);

        set_destroy(V);
        pqueue_destroy(knn);

        for (int j=0; j<G[query_pos]->size; j++) {
            
            // for each point j that is an outgoing neighbour of the query point
            int point = vec_get_at(G[query_pos], j);
            //printf("Neighbour %d has %d neighbours\n", point, G[query_pos]->size);

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
                RobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);

                set_destroy(Nout_j);

            } else {    // if the query point fits and is not already included
                // add it to the neighbours of j
                int flag = 1;
                for (int n=0; n<G[point]->size; n++) {
                    if (vec_get_at(G[point], n) == query_pos) {flag = 0; break;}
                }

                for (int i=0; i<comps; i++) {
                    point_vec[i] = dataset[i][point];
                }

                if (flag == 1) vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps));
                
            }
        }
    }

    free(xq);
    free(point_vec);
    free_matrix_fvecs(dist_matrix, vecs);
    free(per);
    return G;
}

Vector* Vamana_semirandom_medoid(float** dataset, int vecs, int comps, int L, int R, int a, int* med, int threads) {
    printf("Vamana:\nBuilding the vamana index.\n\n");

    // first we have to initialize G to a random R-regular directed graph

    Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   
    for (int i=0; i<vecs; i++) {
        G[i] = vec_Create(0);
    }

    printf("Vamana finding neighbours\n\n");
    int x;
    float* vec_x = (float*)malloc(comps * sizeof(float));
    float* vec_j = (float*)malloc(comps * sizeof(float));

    // for every vector in the dataset
    for (int j = 0; j < vecs; j++) {
        for (int i=0; i<comps; i++) {
            vec_j[i] = dataset[i][j];
        }
        //printf("vector %d:", j);

        int limit = vecs-1, min = 0;
        // for every one of its R neighbours
        for (int i = 0; i < R; i++) {

            int stop = 1;
            while (stop == 1) {
                x = rand() % (limit-min+1)+min;    // pick another vector randomly
                //printf("%d\n", x);
                stop = 0;
                // as long as that vector is not a neighbour already and it is not the same as our current vector
                if (vec_find_node(G[j], x) != VECTOR_EOF || x == j) {   
                    stop = 1;
                }
                if (x == limit) limit--;
                if (x == min) min++;
                // for (int z = 0; z < i; z++) {
                //     // as long as that vector is not a neighbour already and it is not the same as our current vector
                //     if (vec_find_node(G[j], x) != VECTOR_EOF || x != j) {   
                //         stop = 1;
                //         break;
                //     }
                // }    
            }
            // temporarilly store the vectors j and x to compute their distance
            for (int i=0; i<comps; i++) {
                vec_x[i] = dataset[i][x];
            }
            vec_insert(G[j], x, euclidean_distance(vec_x, vec_j, comps));

            //printf(" %d, %f ", G[j]->array[i].value, G[j]->array[i].dist);
            
        }
        //printf("\n");
    }
    free(vec_x);
    free(vec_j);

    // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
    float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
        dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
    }

    for(int i=0; i<vecs; i++) {
        for(int j=0; j<vecs; j++) {
            dist_matrix[i][j] = 0;
        }
    }

    // now we find the medoid of the dataset that will be our starting point s
    // and this time we do it by selecting a random point of the dataset
    srand((unsigned int)time(NULL));

    // we still have to call the medoid function so that the dist_matrix iis computed
    int s;
    if (threads != 0) s = random_medoid_threads(dataset, vecs, comps, &dist_matrix, threads);
    else if (threads == 0) s = random_medoid(dataset, vecs, comps, &dist_matrix);
    printf("Vamana found medoid: %d\n\n", s);
    *med = s;

    // create a random permutation of 1...vecs (really from 0 to vecs-1) 
    // and store it in the array per
    int* per = (int*)malloc(vecs * sizeof(int*));
    for (int i=0; i<vecs; i++) {
        per[i] = i;
    }

    //srand((unsigned int)time(NULL));

    // this is done by swaping the index in the ith position 
    // with that in the position given by randIdx
    for(int i=0; i<vecs; ++i){
        int randIdx = rand() % (vecs - 1);
        // swap per[i] with per[randIdx]
        int t = per[i];
        per[i] = per[randIdx];
        per[randIdx] = t;
    }

    printf("Found random permutation\n\n");
    // for (int i=0; i<vecs; i++) {
    //     printf("%d ", per[i]);
    // }
    // printf("\n");

    float* xq = (float*)malloc(comps * sizeof(float));
    float* point_vec = (float*)malloc(comps * sizeof(float));

    printf("Going through each point of the dataset and updating the graph.\n\n");
    for (int i=0; i<vecs; i++) {
        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        //printf("query vector %d: ", query_pos);
        for (int i=0; i<comps; i++) {
            xq[i] = dataset[i][query_pos];
            //printf("%f ", xq[i]);
        }
        //printf("\n");
        
        // call greedysearch for xq
        Set V;
        PQueue knn = greedySearch(G, R, comps, vecs, dataset, s, xq, L, 1, &V);
        // then run robust prune
        RobustPrune(&G, query_pos, &V, a, R, comps, vecs, dataset, dist_matrix);

        set_destroy(V);
        pqueue_destroy(knn);

        for (int j=0; j<G[query_pos]->size; j++) {
            
            // for each point j that is an outgoing neighbour of the query point
            int point = vec_get_at(G[query_pos], j);
            //printf("Neighbour %d has %d neighbours\n", point, G[query_pos]->size);

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
                RobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);

                set_destroy(Nout_j);

            } else {    // if the query point fits and is not already included
                // add it to the neighbours of j
                int flag = 1;
                for (int n=0; n<G[point]->size; n++) {
                    if (vec_get_at(G[point], n) == query_pos) {flag = 0; break;}
                }

                for (int i=0; i<comps; i++) {
                    point_vec[i] = dataset[i][point];
                }

                if (flag == 1) vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps));
                
            }
        }
    }

    free(xq);
    free(point_vec);
    free_matrix_fvecs(dist_matrix, vecs);
    free(per);
    return G;
}

Vector* Vamana(float** dataset, int vecs, int comps, int L, int R, int a) {

    Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   
    for (int i=0; i<vecs; i++) {
        G[i] = vec_Create(0);
    }

    // we check the vamana for G:
    // vector0: 1 2 3 
    // vector1: 3 2 0
    // vector2: 1 3 4
    // vector3: 2 4 1
    // vector4: 2 3 0

    float* vec1 = (float*)malloc(comps * sizeof(float));
    float* vec2 = (float*)malloc(comps * sizeof(float));

    for (int i = 0; i < comps; i++) {
        vec1[i] = dataset[i][0];
    }
    for(int j=0; j<vecs; j++) {
        for (int i = 0; i < comps; i++) {
            vec2[i] = dataset[i][j];
        }
        if (j == 1 || j==2 || j==3) vec_insert(G[0], j, euclidean_distance(vec1, vec2, comps));
    }
    printf("vec1: ");
    for (int i = 0; i < comps; i++) {
        vec1[i] = dataset[i][1];
    }
    for(int j=0; j<vecs; j++) {
        for (int i = 0; i < comps; i++) {
            vec2[i] = dataset[i][j];
        }
        if (j ==2 || j==3 || j==0) vec_insert(G[1], j, euclidean_distance(vec1, vec2, comps));
    }
    

    for (int i = 0; i < comps; i++) {
        vec1[i] = dataset[i][2];
    }
    for(int j=0; j<vecs; j++) {
        for (int i = 0; i < comps; i++) {
            vec2[i] = dataset[i][j];
        }
        if (j ==3 || j==0 || j==1) vec_insert(G[2], j, euclidean_distance(vec1, vec2, comps));
    }

    for (int i = 0; i < comps; i++) {
        vec1[i] = dataset[i][3];
    }
    for(int j=0; j<vecs; j++) {
        for (int i = 0; i < comps; i++) {
            vec2[i] = dataset[i][j];
        }
        if (j == 2 || j==0 || j==1) vec_insert(G[3], j, euclidean_distance(vec1, vec2, comps));
    }

    for (int i = 0; i < comps; i++) {
        vec1[i] = dataset[i][4];
    }
    for(int j=0; j<vecs; j++) {
        for (int i = 0; i < comps; i++) {
            vec2[i] = dataset[i][j];
        }
        if (j ==2 || j==3 || j==1) vec_insert(G[4], j, euclidean_distance(vec1, vec2, comps));
    }

    free(vec1);
    free(vec2);
    
    printf("neighbours\n");
    for (int j = 0; j < vecs; j++) {
        printf("vector %d:", j);
        for (int i = 0; i < R; i++) {
            printf(" %d, %f ",  vec_get_at(G[j], i), vec_get_dist(G[j], i));
        }
        printf("\n");
    }

    // create a 2D distance matrix that will hold the euclidean distances between all vectors of the dataset
    float** dist_matrix = (float**)malloc(vecs * sizeof(float*));
    for (int i = 0; i < vecs; i++) {
        dist_matrix[i] = (float*)malloc(vecs * sizeof(float));
    }

    for(int i=0; i<vecs; i++) {
        for(int j=0; j<vecs; j++) {
            dist_matrix[i][j] = 0;
        }
    }

    // now we find the medoid of the dataset that will be our starting point s
    // and we also fill the distance matrix
    int s = medoid(dataset, vecs, comps, &dist_matrix);
    printf("medoid: %d\n", s);

    // create a random permutation of 1...vecs (really from 0 to vecs-1) 
    // and store it in the array per
    int* per = (int*)malloc(vecs * sizeof(int*));
    for (int i=0; i<vecs; i++) {
        per[i] = i;
    }

    // the permutation used for testing
    per[0] = 0; per[1] = 3; per[2] = 2; per[3] = 4; per[4] = 1;

    printf("random permutation:\n");
    for (int i=0; i<vecs; i++) {
        printf("%d ", per[i]);
    }
    printf("\n");

    float* xq = (float*)malloc(comps * sizeof(float));
    float* point_vec = (float*)malloc(comps * sizeof(float));

    for (int i=0; i<vecs; i++) {
        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        printf("query vector %d: ", query_pos);
        for (int i=0; i<comps; i++) {
            xq[i] = dataset[i][query_pos];
            printf("%f ", xq[i]);
        }
        printf("\n");
        
        Set V;
        PQueue knn = greedySearch(G, R, comps, vecs, dataset, s, xq, L, 1, &V);
        
        RobustPrune(&G, query_pos, &V, a, R, comps, vecs, dataset, dist_matrix);

        set_destroy(V);
        pqueue_destroy(knn);

        for (int j=0; j<G[query_pos]->size; j++) {
            
            // for each point j that is an outgoing neighbour of the query point
            int point = vec_get_at(G[query_pos], j);
            printf("Neighbour %d has %d neighbours\n", point, G[query_pos]->size);

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
                RobustPrune(&G, point, &Nout_j, a, R, comps, vecs, dataset, dist_matrix);

                set_destroy(Nout_j);

            } else {    // if the query point fits and is not already included
                // add it to the neighbours of j
                int flag = 1;
                for (int n=0; n<G[point]->size; n++) {
                    if (vec_get_at(G[point], n) == query_pos) {flag = 0; break;}
                }

                for (int i=0; i<comps; i++) {
                    point_vec[i] = dataset[i][point];
                }

                if (flag == 1) vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps));
        
            }
        }
    }

    free_matrix_fvecs(dist_matrix, vecs);
    free(per);
    free(xq);
    free(point_vec);
    return G;
}

void free_G(Vector* G, int v) {
    // Free the allocated memory for the matrix
    if (G == NULL) return;

    for (int i = 0; i < v; i++) {
        vec_destroy(G[i]);
    }
    free(G);
}