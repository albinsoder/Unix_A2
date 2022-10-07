#include "../include/kmeans.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool something_changed=false;
void read_data()
{
    N = 1797;
    k = 9;
    FILE* fp = fopen("kmeans-data.txt", "r");
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

    int old_cluster = -1, new_cluster = -1;
    for (int i = start; i < end; i++)
    { // For each data point
        old_cluster = data[i].cluster;
        // pthread_barrier_wait(&barrier);
        new_cluster = get_closest_centroid(i, k);
        data[i].cluster = new_cluster; // Assign a cluster to the point i
        if (old_cluster != new_cluster)
        {
            something_changed = true;
        }
    }
    // printf("BOOL: %d", something_changed);
}

void update_cluster_centers(int start, int end)
{
    /* Update the cluster centers */
    int c;
    int count[MAX_CLUSTERS] = { 0 }; // Array to keep track of the number of points in each cluster
    point temp[MAX_CLUSTERS] = { 0.0 };

    for (int i = 0; i < N; i++)
    {
        c = data[i].cluster;
        count[c]++;
        temp[c].x += data[i].x;
        temp[c].y += data[i].y;
    }
    for (int i = 0; i < k; i++)
    {
        cluster[i].x = temp[i].x / count[i];
        cluster[i].y = temp[i].y / count[i];
    }
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
    pthread_barrier_init(&barrier, NULL, n_threads);                          // Initialize barrier
    struct th* t_info;
    
    do {
        iter++; // Keep track of number of iterations
        // printf("START BOOL: %d \n", something_changed);
        // sleep(1);
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
        pthread_barrier_destroy(&barrier);
        int start,end=0;
        update_cluster_centers(start, end);
    } while (something_changed);
    free(children);
    free(t_info);
    printf("Number of iterations taken = %d\n", iter);
    printf("Computed cluster numbers successfully!\n");

}

void write_results()
{
    FILE* fp = fopen("kmeans-results.txt", "w");
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
    printf("Wrote the results to a file!\n");
}

int start_kmeans()
{
    read_data(); 
    kmean(k);
    write_results();
}
int main()
{
    read_data(); 
    kmean(k);
    write_results();
}