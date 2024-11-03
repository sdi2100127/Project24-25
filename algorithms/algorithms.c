#include "algorithms.h"
#include <float.h>

float euclidean_distance(float* vec1, float* vec2, int comps) {
    float res = 0.0f;
    for (int i = 0; i < comps; i++) {
        float diff = vec1[i] - vec2[i];
        res += diff*diff;
    }
    res = sqrt(res);
    return res;
}

float squared_euclidean_distance(float* vec1, float* vec2, int comps) {
    float res = 0.0f;
    for (int i = 0; i < comps; i++) {
        float diff = vec1[i] - vec2[i];
        res += diff*diff;
    }
    
    return res;
}

PQueue greedySearch(Vector* G, int R, int dim, int vecs, float** vectors, int s, float* xq, int L, int k, Set* V) {
    //printf("GREEDY SEARCH\n");

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

    set_Node node_v;
    int node_value, xp = -1;
    float min_dist;

    // float* vec_p = (float*)malloc(dim * sizeof(float));

    // create a distance matrix with the euclidean distance of each vector to xq
    // float* dist_matrix = (float*)malloc(vecs * sizeof(float*));
    // for (int j = 0; j < vecs; j++) {
    //     for (int i=0; i<dim; i++) {
    //         vec_p[i] = vectors[i][j];
    //     }
    //     dist_matrix[j] = euclidean_distance(vec_p, xq, dim);
    // }

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
            //if (node_value == -1) continue;
            if (S_find_equal(visited_nodes->root, node_value) == SET_EOF) {
                flag = 1;   // unvisited nodes where indeed found              

                // if the next node xp is not initialized yet initialize it with the first unvisited node you encounter
                // otherwise check if its position is smaller than the current min_pos
                //float dist = dist_matrix[node_value];
                float dist = node->dist;
                if (xp == -1 || dist <= min_dist) {
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
        for (int i=0; i<G[xp]->size; i++) {
            pqueue_insert(knn, vec_get_at(G[xp], i), vec_get_dist(G[xp], i));
        }

        // add node xp to te visited_nodes set
        set_insert(visited_nodes, xp);

        // if the knn_set is bigger than L
        if (knn->vector->size > L) {
            // remove the most distant neighbours of xq from knn_set until its equal to L
            // prev_max = 0.0;
            // prev_max_pos = -1;
            while(knn->vector->size > L) {
                pqueue_remove(knn);
                // float max_dist = 0.0;
                // int max_pos = -1;
                // for (int i=0; i<vecs; i++) {
                //     if (dist_matrix[i] >= max_dist && (prev_max == 0.0 || prev_max >= dist_matrix[i])) {
                //         if (i != prev_max_pos) {
                //             max_dist = dist_matrix[i];
                //             max_pos = i;
                //         }
                //     } 
                // }
                // set_Node node = S_find_equal(knn_set->root, max_pos);
                // if (node != SET_EOF) {
                //     set_remove(knn_set, max_pos);
                // }
                // prev_max = max_dist;
                // prev_max_pos = max_pos;
            }
        }
    }
    
    // now we have to prune the set again to only keep xq's k nearest neighbours
    if (knn->vector->size > k) {
        // remove the most distant neighbours of xq from knn_set until its equal to k
        while(knn->vector->size > k) {
            pqueue_remove(knn);
        //     float max_dist = 0.0;
        //     int max_pos = -1;
        //     for (int i=0; i<vecs; i++) {
        //         if (dist_matrix[i] >= max_dist && (prev_max == 0.0 || prev_max >= dist_matrix[i])) {
        //             if (i != prev_max_pos) {
        //                 max_dist = dist_matrix[i];
        //                 max_pos = i;
        //             }
        //         } 
        //     }
        //     set_Node node = S_find_equal(knn_set->root, max_pos);
        //     if (node != SET_EOF) {
        //         set_remove(knn_set, max_pos);
        //     }
        //     prev_max = max_dist;
        //     prev_max_pos = max_pos;
        }
    }

    // printf("knn_set: ");
    // for (set_Node node = find_min(knn_set->root); node != SET_EOF; node = set_next(knn_set, node)) { 
    //     printf("%d ", node->value);
    // }
    // printf("\n");

    // printf("visited_set: ");
    // for (set_Node node = find_min(visited_nodes->root); node != SET_EOF; node = set_next(visited_nodes, node)) { 
    //     printf("%d ", node->value);
    // }
    // printf("\n");

    
    *V = visited_nodes;
    
    // free(vec_p);
    // free(dist_matrix);

    //set_destroy()
    return knn;

}

//Implementation of Robust Prune function
void RobustPrune(Vector** G, int p ,Set * V, int a, int R , int * neigh_count ,int dim , int vecs , float **vectors){
    //printf("ROBUST PRUNE\n");
    
    Set temp = *V;
    
    Vector* temp_G = *G;
    for (int i = 0; i <= R - 1 ;i++){           //Inserting our Nout(p) to our V set
        set_insert(temp, vec_get_at(temp_G[p], i));
    }

    set_remove(temp, p);                           //And checking if we have inserted our p so that we remove it

    //Now we have to empty our Nout(p)
    while(temp_G[p]->size != 0) vec_remove(temp_G[p]);

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
        
        for (int i=0; i<dim; i++) {
            vec_of_p_star[i] = vectors[i][p_star];
        }

        vec_set_at(temp_G[p], i_count, p_star, euclidean_distance(vec_of_p_star, vec_of_p, dim));
        i_count ++;

        if(i_count == R)
            break;

        for (int j = 0; j < vecs; j++) {
            for (int i=0; i<dim; i++) {
                vec_p[i] = vectors[i][j];
            }
            dist_matrix_2[j] = euclidean_distance(vec_p, vec_of_p_star, dim);
        }

        set_Node next = find_min(temp->root);
        while(next != SET_EOF){
            int node_value = next->value;
            if(a * dist_matrix_2[node_value] <= dist_matrix[node_value]){
                next = set_next(temp, next);
                set_remove(temp,node_value);
            } else {
                next = set_next(temp, next);
            }
        }

    }

    // printf("Nout(%d): ", p);
    // for (int i=0; i<dim; i++) {
    //     printf("%d ", temp_G[i][p]);
    // }
    // printf("\n");

    *V = temp;
    *G = temp_G;
    *neigh_count = i_count;

    free(dist_matrix);
    free(dist_matrix_2);
    
    free(vec_p);
    free(vec_of_p);
    free(vec_of_p_star);

    //printf("\n");

    return;
}

int medoid(float** dataset, int vecs, int comps) {

    float* vec_p1 = (float*)malloc(comps * sizeof(float));
    float* vec_p2 = (float*)malloc(comps * sizeof(float));

    float min_sum = FLT_MAX;
    int min_p = -1;
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
            if (j != z) {
                dist_sum = dist_sum + squared_euclidean_distance(vec_p1, vec_p2, comps);
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

    printf("medoid sum: %f\n", min_sum);
    return min_p;
}

Vector* Vamana(float** dataset, int vecs, int comps, int L, int R, int a) {
    // first we have to initialize G to a random R-regular directed graph

    // int** G = (int**)malloc(R * sizeof(int*));
    // for (int i = 0; i < R; i++) {
    //     G[i] = (int*)malloc(vecs * sizeof(int));
    // }

    Vector* G = (Vector*)malloc(vecs * sizeof(Vector));
   
   for (int i=0; i<vecs; i++) {
      G[i] = vec_Create(0);
   }

   printf("neighbours\n");
   int x;
   float* vec_x = (float*)malloc(comps * sizeof(float));
   float* vec_j = (float*)malloc(comps * sizeof(float));
   // for every vector in the dataset
   for (int j = 0; j < vecs; j++) {
      for (int i=0; i<comps; i++) {
         vec_j[i] = dataset[i][j];
      }
      printf("vector %d:", j);
      // for every one of its R neighbours
      for (int i = 0; i < R; i++) {

         int stop = 1;
         while (stop == 1) {
            x = rand() % (vecs - 1);    // pick another vector randomly
            stop = 0;
            for (int z = 0; z < i; z++) {
               // as long as that vector is not a neighbour already and it is not the same as our current vector
               if (vec_find_node(G[j], x) != VECTOR_EOF || x == j) {   
                  stop = 1;
                  break;
               }
            }    
         }
         // temporarilly store the vectors j and x to compute their distance
         for (int i=0; i<comps; i++) {
            vec_x[i] = dataset[i][x];
         }
         vec_insert(G[j], x, euclidean_distance(vec_x, vec_j, comps));

         printf(" %d, %f ", G[j]->array[i].value, G[j]->array[i].dist);
         
      }
      printf("\n");
   }

    // G[0][0] = 3; G[1][0] = 2;  G[2][0] = 1;
    // G[0][1] = 3; G[1][1] = 2;  G[2][1] = 0;
    // G[0][2] = 1; G[1][2] = 3;  G[2][2] = 0;
    // G[0][3] = 2; G[1][3] = 0;  G[2][3] = 1;
    // G[0][4] = 2; G[1][4] = 3;  G[2][4] = 1;

    // now we find the medoid of the dataset that will be our starting point s
    int s = medoid(dataset, vecs, comps);
    printf("medoid: %d\n", s);

    // create a random permutation of 1...vecs (really from 0 to vecs-1) 
    // and store it in the array per
    int* per = (int*)malloc(vecs * sizeof(int*));
    for (int i=0; i<vecs; i++) {
        per[i] = i;
    }

    srand((unsigned int)time(NULL));

    for(int i=0; i<vecs; ++i){
        int randIdx = rand() % (vecs - 1);
        // swap per[i] with per[randIdx]
        int t = per[i];
        per[i] = per[randIdx];
        per[randIdx] = t;
    }

    //per[0] = 0; per[1] = 3; per[2] = 2; per[3] = 4; per[4] = 1;

    printf("random permutation:\n");
    for (int i=0; i<vecs; i++) {
        printf("%d ", per[i]);
    }
    printf("\n");

    // we create an array that holds how many outgoing neighbours each point/vector has
    // and initiallize it with R
    // int* out_n_count = (int*)malloc(vecs * sizeof(int));
    // for (int i=0; i<vecs; i++) out_n_count[i] = R;

    for (int i=0; i<vecs; i++) {
        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        float* xq = (float*)malloc(comps * sizeof(float));
        //printf("query vector %d: ", query_pos);
        for (int i=0; i<comps; i++) {
            xq[i] = dataset[i][query_pos];
            //printf("%f ", xq[i]);
        }
        //printf("\n");
        
        Set V;
        PQueue knn = greedySearch(G, R, comps, vecs, dataset, s, xq, L, 1, &V);

        // TESTED SO FAR!!!!!!!!!
        //break;
        
        int new_n_out;
        RobustPrune(&G, query_pos, &V, a, R, &new_n_out, comps, vecs, dataset);
        // after each point's neighbours get pruned we update the count in the outgoing neighbours array
        //out_n_count[query_pos] = new_n_out;
        //printf("vec %d has now %d neighbours\n", query_pos, new_n_out);

        for (int j=0; j<G[query_pos]->size; j++) {
            
            // for each point j that is an outgoing neighbour of the query point
            int point = vec_get_at(G[query_pos], j);
            //if (point == -1) continue;
            //printf("Neighbour %d has %d neighbours\n", point, out_n_count[point]);

            // check if it has less outgoing neighbours than R 
            // so that we can also add the query point as a neighbour

            // if we cannot
            if (G[point]->size == R) {

                // we create a set Nout_j with the outgoing neighbours of j as well as the current query point
                Set Nout_j = set_Create();
                for (int n=0; n<G[point]->size; n++) {
                    // if (G[n][point] == -1) {
                    //     break;
                    // }
                    set_insert(Nout_j, vec_get_at(G[point], n));
                }
                set_insert(Nout_j, query_pos);
                RobustPrune(&G, point, &Nout_j, a, R, &new_n_out, comps, vecs, dataset);
                //printf("vec %d has now %d neighbours\n", point, new_n_out);

                //set_destroy(Nout_j)

            } else {    // if the query point fits and is not already included
                // add it to the neighbours of j
                int flag = 1;
                for (int n=0; n<G[point]->size; n++) {
                    // if (G[n][point] == -1) {
                    //     break;
                    // } else if (G[n][point] == query_pos) {
                    //     flag = 0; break;
                    // }
                    if (vec_get_at(G[point], n) == query_pos) {flag = 0; break;}
                }

                float* point_vec = (float*)malloc(comps * sizeof(float));
                for (int i=0; i<comps; i++) {
                    point_vec[i] = dataset[i][point];
                }

                if (flag == 1) vec_insert(G[point], query_pos, euclidean_distance(xq, point_vec, comps));

                // printf("Nout(%d): ", point);
                // for (int i=0; i<R; i++) {
                //     printf("%d ", G[i][point]);
                // }
                // printf("\n");
            }
        }
    }
    return G;
}