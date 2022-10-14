#include "../include/kmeans.h"

// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool something_changed=false;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void read_data(int input_k, int input_N, char* path)
{
    k = input_k;
    N = input_N;
    // printf("N: %d \n", N);
    // printf("k: %d \n", k);
    // printf("N: %s \n", path);
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Cannot open the file");
        exit(EXIT_FAILURE);
    }
   
    // Initialize points from the data file
    float temp;

    for (int i = 0; i < N; i++)
    {
        fscanf(fp, "%f %f", &data[i].x, &data[i].y);
        data[i].cluster = -1; // Initialize the cluster number to -1
    }
    printf("Read the problem data!\n");
    // Initialize centroids randomly
    srand(0); // Setting 0 as the random number generation seed
    for (int i = 0; i < k; i++)
    {
        int r = rand() % N;
        printf("Cluster: %f \n", data[r].x);
        cluster[i].x = data[r].x;
        cluster[i].y = data[r].y;
    }
    fclose(fp);
}

int get_closest_centroid(int i, int k)
{
    /* find the nearest centroid */
    int nearest_cluster = -1;
    double xdist, ydist, dist, min_dist;
    min_dist = dist = INT_MAX;
    for (int c = 0; c < k; c++)
    { // For each centroid
        // Calculate the square of the Euclidean distance between that centroid and the point
        xdist = data[i].x - cluster[c].x;
        ydist = data[i].y - cluster[c].y;
        dist = xdist * xdist + ydist * ydist; // The square of Euclidean distance
        //printf("%.2lf \n", dist);
        if (dist <= min_dist)
        {
            min_dist = dist;
            nearest_cluster = c;
        }
    }
    //printf("-----------\n");
    return nearest_cluster;
}

void* assign_clusters_to_points(void* id)
{
    something_changed = false;
    struct th *t_info = (struct th*)id;
    int start, end, size_n, thread_div;
    size_n= (int)t_info->size;
    if(size_n == -1){                                        //if size = -1, this indicates its last thread with extra work
        start = t_info->start*t_info->factor;
        end = t_info->end;
    }
    else if(size_n < 8){
        thread_div=1;
        start = t_info->start*thread_div;
        end = (t_info->end)*thread_div;
        // printf("SIZE: %d", sizeN);
    } else{
        thread_div = size_n/8;
        start = t_info->start*thread_div;
        end = (t_info->end)*thread_div;
    }
    // printf("START: %d \n", start);
    // printf("END: %d \n", end);
    // int old_cluster = -1, new_cluster = -1;

    for (int i = start; i < end; i++)
    { // For each data point
        t[i].old_cluster = data[i].cluster;
        pthread_mutex_lock(&lock);
        t[i].new_cluster = get_closest_centroid(i, k);
        pthread_mutex_unlock(&lock);
        data[i].cluster = t[i].new_cluster; // Assign a cluster to the point i
        if (t[i].old_cluster != t[i].new_cluster)
        {
            something_changed = true;
        }
    }
    // update_cluster_centers(start, end);

    // printf("BOOL: %d", something_changed);
}

void update_cluster_centers(int start, int end)
{
    /* Update the cluster centers */
    int count[MAX_CLUSTERS] = { 0 }; // Array to keep track of the number of points in each cluster
    point temp[MAX_CLUSTERS] = { 0.0 };
    int c;
    // printf("START: %d \n", start);
    // printf("END: %d \n", end);
    // pthread_barrier_wait(&barrier);

    for (int i = 0; i < N; i++)
    {
        c = data[i].cluster;
        count[c]++;
        temp[c].x += data[i].x;
        temp[c].y += data[i].y;
    }
    // pthread_barrier_wait(&barrier);
    // start=0;
    for (int i = 0; i < k; i++)
    {
        cluster[i].x = temp[i].x / count[i];
        cluster[i].y = temp[i].y / count[i];
    }
    // if(start ==0){
    //     update_cluster_centers_continue(count, temp, start);
    // }

}
void update_cluster_centers_continue(int count[MAX_CLUSTERS], point temp[MAX_CLUSTERS], int start)
{

}


int kmean(int k)
{    
    children = malloc(N * sizeof(pthread_t));
    int n_threads = 8; // Number of threads to be created
    int rest=0;
    int factor = 0;
    int flag = 0;
    int iter = 0;
    if(N < n_threads){
        n_threads=N;
        rest = 0;
    }
    else if(N % n_threads != 0){
        rest = N%n_threads;
        flag=1;
        factor = (N-rest)/n_threads;
    }
    // pthread_barrier_init(&barrier, NULL, n_threads);                          // Initialize barrier
    struct th* t_info;
    // pthread_barrier_init(&barrier, NULL, n_threads);                          // Initialize barrier

    do {
        iter++; // Keep track of number of iterations
        for(int id=0; id<n_threads; id++){
            if(flag && id==n_threads-1){
                t_info = (struct th*)malloc(sizeof(struct th));
                t_info->start = id;
                t_info->end = N;
                t_info->size = -1; 
                t_info->factor = factor;               
                pthread_create(&(children[id]), NULL, assign_clusters_to_points, t_info);
            }
            else{
                t_info = (struct th*)malloc(sizeof(struct th));
                t_info->start = id;
                t_info->end = id+1;
                t_info->size = N;                
                pthread_create(&(children[id]), NULL, assign_clusters_to_points, t_info);
            }
        }
        for (int id = 0; id < n_threads; id++) {
            pthread_join(children[id], NULL);                                 // Collect/join result from the threads
        }
        update_cluster_centers(0,0);
        // pthread_barrier_destroy(&barrier);
        // update_cluster_centers_continue();
    } while (something_changed);
    free(children);
    free(t_info);
    printf("Number of iterations taken = %d\n", iter);
    printf("Computed cluster numbers successfully!\n");

}

void write_results(int pID)
{
    // char path[70];
    // sprintf(path,"../../computed_results/server_results/kmeans%d_sol.txt",pID);

    FILE* fp = fopen("result.txt", "w");
    if (fp == NULL) {
        perror("Cannot open the file");
        exit(EXIT_FAILURE);
    }
    else
    {
        for (int i = 0; i < N; i++)
        {
            fprintf(fp, "%.2f %.2f %d\n", data[i].x, data[i].y, data[i].cluster);
        }
    }
    fclose(fp);
    printf("Wrote the results to a file!\n");
}

int start_kmeans(char* k, int N, char* path, int pID)
{
    int input_k = atoi(k);
    read_data(input_k, N, path); 
    kmean(input_k);
    write_results(pID);
    return 0;
}
int main()
{
    read_data(9, 1797, "kmeans-data.txt"); 
    kmean(k);
    write_results(1);

}