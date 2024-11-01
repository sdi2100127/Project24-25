#include "Vamana.h"

int medoid(float** dataset, int vecs, int comps) {

    float* vec_p1 = (float*)malloc(comps * sizeof(float));
    float* vec_p2 = (float*)malloc(comps * sizeof(float));

    float min_sum = 1000000.0f;
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
                dist_sum = dist_sum + euclidean_distance(vec_p1, vec_p2, comps);
            }
        }
        
        // then find which vector has the lowest total sum
        if (dist_sum <= min_sum) {
            min_sum = dist_sum;
            min_p = j;
        }
    }

    printf("medoid sum: %f\n", min_sum);
    return min_p;
}

int** Vamana(float** dataset, int vecs, int comps, int L, int R, int a) {
    // first we have to initialize G to a random R-regular directed graph

    int** G = (int**)malloc(R * sizeof(int*));
    for (int i = 0; i < R; i++) {
        G[i] = (int*)malloc(vecs * sizeof(int));
    }

    printf("neighbours\n");
    int x;
    // for every vector in the dataset
    for (int j = 0; j < vecs; j++) {
        printf("vector %d:", j);
        // for every one of its R neighbours
        for (int i = 0; i < R; i++) {

            int stop = 1;
            while (stop == 1) {
                x = rand() % (vecs - 1);    // pick another vector randomly
                stop = 0;
                for (int z = 0; z < i; z++) {
                    // as long as that vector is not a neighbour already and it is not the same as our current vector
                    if (x == G[z][j] || x == j) {   
                        stop = 1;
                        break;
                    }
                }    
            }
            G[i][j] = x;
            printf(" %d", G[i][j]);
            
        }
        printf("\n");
    }

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

    printf("random permutation:\n");
    for (int i=0; i<vecs; i++) {
        printf("%d ", per[i]);
    }
    printf("\n");

    // we create an array that holds how many outgoing neighbours each point/vector has
    // and initiallize it with R
    int* out_n_count = (int*)malloc(vecs * sizeof(int));
    for (int i=0; i<vecs; i++) out_n_count[i] = R;

    for (int i=0; i<vecs; i++) {
        // find and store the query vector xq based on the point in the dataset indexed by per[i]
        int query_pos = per[i];
        float* xq = (float*)malloc(comps * sizeof(float));
        printf("query vector %d: ", query_pos);
        for (int i=0; i<comps; i++) {
            xq[i] = dataset[i][query_pos];
            printf("%f ", xq[i]);
        }
        printf("\n");
        
        Set V;
        Set knn = greedySearch(G, R, comps, vecs, dataset, s, xq, L, 1, &V);

        // TESTED SO FAR!!!!!!!!!
        //break;
        
        int new_n_out;
        RobustPrune(&G, query_pos, &V, a, R, &new_n_out, comps, vecs, dataset);
        // after each point's neighbours get pruned we update the count in the outgoing neighbours array
        out_n_count[query_pos] = new_n_out;

        for (int j=0; j<new_n_out; j++) {
            // for each point j that is an outgoing neighbour of the query point
            int point = G[j][query_pos];
            // check if its size is smaller than R 
            // so that we can also add the query point as a neighbour

            // if we cannot
            if (out_n_count[point] == R) {

                // we create a set Nout_j with the outgoing neighbours of j as well as the current query point
                Set Nout_j = set_Create();
                for (int n=0; n<R; n++) {
                    if (G[n][point] == -1) {
                        break;
                    }
                    set_insert(Nout_j, G[n][point]);
                }
                set_insert(Nout_j, query_pos);
                RobustPrune(&G, point, &Nout_j, a, R, &new_n_out, comps, vecs, dataset);
                //set_destroy(Nout_j)

            } else {    // if the query point fits
                // add it to the neighbours of j
                G[out_n_count[point] + 1][point] = query_pos;
            }
        }
    }
    return G;
}