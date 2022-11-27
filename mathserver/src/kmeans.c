#include "../include/kmeans.h"

// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void read_data(int input_k, int input_N, char* path)
{
    k = input_k;
    N = input_N;

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
    // printf("Read the problem data!\n");
    // Initialize centroids randomly
    srand(0); // Setting 0 as the random number generation seed
    for (int i = 0; i < k; i++)
    {
        int r = rand() % N;
        // printf("Cluster: %f \n", data[r].x);
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
        // pthread_mutex_lock(&lock);

        xdist = data[i].x - cluster[c].x;
        ydist = data[i].y - cluster[c].y;
        dist = xdist * xdist + ydist * ydist; // The square of Euclidean distance
        // pthread_mutex_unlock(&lock);

        if (dist <= min_dist)
        {
            min_dist = dist;
            nearest_cluster = c;
        }
    }
    //printf("-----------\n");
    return nearest_cluster;
}

bool assign_clusters_to_points(int start, int end)
{
    int old_cluster = -1, new_cluster = -1;
    bool something_changed=false;
    for (int i = 0; i < N; i++)
    {
        // pthread_mutex_lock(&lock);
        old_cluster = data[i].cluster;
        new_cluster = get_closest_centroid(i, k);

        // pthread_mutex_lock(&lock);
        data[i].cluster = new_cluster; // Assign a cluster to the point i
        if (old_cluster != new_cluster)
        {
            something_changed = true;
        }
        // pthread_mutex_unlock(&lock);
    }
    // pthread_barrier_wait(&barrier);
    // printf("%d \n", end);
    // if(end == N){
    //     update_cluster_centers(start,end);
    // }
    // bzero(count, 32);
    // bzero(temp, 32);
    return something_changed;
}

void update_cluster_centers(int start, int end)
{
    /* Update the cluster centers */
    int c;
    int counter =0;

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
    
    // if(end == N){
    //     update_cluster_centers_continue();
    // }

    

}

void update_cluster_centers_continue(){
    // printf("HELKLO");
    // bzero(count, sizeof(count));
    // bzero(temp, sizeof(temp));
    // printf("HEJ\n");
}

void* kmean_thread(void* id)
{    
    struct th *t_info = (struct th*)id;
    int start, end, size_n, thread_div;

    size_n= (int)t_info->size;
    bool something_changed=false;

    if(size_n == -1){         //if size = -1, this indicates its last thread with extra work
        start = t_info->start*t_info->factor;
        end = t_info->end;
    }
    else if(size_n < 8){      //If size < 8, indicates we have less size than threads
        thread_div=1;
        start = t_info->start*thread_div;
        end = (t_info->end)*thread_div;
    } else{                   //Else standard case where we divide workload based on 8 threads
        thread_div = size_n/8;
        start = t_info->start*thread_div;
        end = (t_info->end)*thread_div;
    }
    int iter=0;
    /*
    struct th:
        start:  where iteration start for thread
        end:    end iteration
        size:   size of file (if size=-1, used as flag to indicate last iteration)
        factor: what to multiply with to get the correct iteration for all threads
    */
    do {
        iter++; // Keep track of number of iterations
        something_changed = assign_clusters_to_points(start, end);
        update_cluster_centers(start, end);

    } while (something_changed);

    // printf("Number of iterations taken = %d\n", iter);
    // printf("Computed cluster numbers successfully!\n");

}

int kmean(int k)
{    
    children = malloc(N * sizeof(pthread_t));
    int n_threads = 8;           // Number of threads to be created
    int rest=0;
    int factor = 0;
    int flag = 0;
    int iter = 0;
    // pthread_barrier_init(&barrier, NULL, 1); // Initialize barrier to wait for n_threads amount
    if(N < n_threads){ //If number of threads is less than size, do N threads
        n_threads=N;
        rest = 0;
    }
    else if(N % n_threads != 0){  //If mod n_threads give rest, save it to variable rest to assign more work to last thread
        rest = N%n_threads;
        flag=1;
        factor = (N-rest)/n_threads;
    }

    /*
    struct th:
        start:  where iteration start for thread
        end:    end iteration
        size:   size of file (if size=-1, used as flag to indicate last iteration)
        factor: what to multiply with to get the correct iteration for all threads
    */
    struct th* t_info;
//    Keep track of number of iterations
    for(int id=0; id<n_threads; id++){
        if(flag && id==n_threads-1){
            t_info = (struct th*)malloc(sizeof(struct th));
            t_info->start = id;
            t_info->end = N;
            t_info->size = -1; 
            t_info->factor = factor;               
            pthread_create(&(children[id]), NULL, kmean_thread, t_info);
        }
        else{
            t_info = (struct th*)malloc(sizeof(struct th));
            t_info->start = id;
            t_info->end = id+1;
            t_info->size = N;                
            pthread_create(&(children[id]), NULL, kmean_thread, t_info);
        }
    }
    for (int id = 0; id < n_threads; id++) {
        pthread_join(children[id], NULL);                                 // Collect/join result from the threads
    }
        // update_cluster_centers(0, 0);
        // update_cluster_centers_continue();

    free(children);
    free(t_info);
    // printf("Number of iterations taken = %d\n", iter);
    // printf("Computed cluster numbers successfully!\n");

}

void write_results(int pID)
{
    char path[70];
    sprintf(path,"../../computed_results/server_results/kmeans%d_sol.txt",pID);

    FILE* fp = fopen(path, "w");
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
    // printf("Wrote the results to a file!\n");
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
    read_data(9, 1797, "../objects/kmeans-data.txt"); 
    kmean(k);
    write_results(0);

}