#include "filtered_algorithms.h"
#include <float.h>

PQueue FilteredGreedySearch(Vector* G, int R, int dim, int vecs, float** vectors, int s, float* xq, float fq, int L, int k, Set* V) {
    
    // initialise knn and visited set as empty
    PQueue knn = pqueue_create(NULL);
    Set visited_set = set_Create();


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

Map FindMedoid(float** dataset, int vecs, int comps, float*** dist_m, float min_f, float max_f, Map filtered_data, int t) {
    // initialize M as an empty map
    // M mapps filters to start nodes
    Map M = map_create(min_f, max_f);

    // initialize T as an empty map
    // T mapps point ids to how often they have been used as medoids
    Map T = map_create(0, vecs);

    Set R_f = set_Create();
    // for each different filter f
    for (MapNode node = map_first(filtered_data); node != MAP_EOF; node = map_next(filtered_data, node)) {
        // let P_f denote the ids of all points matching filter f
        // (which are the contents of the corresponding bucket in the hash map)
        Vector P_f = node->values;

        // R_f holds t randomly sampled data point ids from P_f
        while (R_f->size < t || R_f->size < P_f->size) {
            int x = rand() % (P_f->size-1);
            set_insert(R_f, vec_get_at(P_f, x));
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
        map_insert(M, node, p_star);

        // and increase the count in map T
        // obtain current count
        int cur_count = vec_get_at(map_find_values(T, p_star), 0);
        // remove it to ensure that the map always contains only the latest count
        map_remove(T, p_star);
        // insert the key p_star again with the updated count
        map_insert(T, p_star, cur_count++);

    }

    return M;
}

Vector* FilteredVamanaIndexing(float** dataset, float min_f, float max_f, int vecs, int comps, int L, int R, int a, int* med) {
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

    // Set values;
    // for (MapNode node = map_first(filtered_data); node != MAP_EOF; node = map_next(filtered_data, node)) {
    //     printf("filter %d: ", node->key);
    //     values = node->values;
    //     for (set_Node s_node = find_min(values->root); s_node != SET_EOF; s_node = set_next(values, s_node)) {
    //         printf("%d ", s_node->value);
    //     }
    //     printf("\n");
    // }



    map_destroy(filtered_data);
    free_matrix_fvecs(dist_matrix, vecs);

    return G;
}


