#include "../include/kmeans.h"

bool something_changed =false;
int count[MAX_CLUSTERS] = { 0 }; // Array to keep track of the number of points in each cluster
point temp[MAX_CLUSTERS] = { 0.0 };

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
        xdist = data[i].x - cluster[c].x;
        ydist = data[i].y - cluster[c].y;
        dist = xdist * xdist + ydist * ydist; // The square of Euclidean distance

        if (dist <= min_dist)
        {
            min_dist = dist;
            nearest_cluster = c;
        }
    }
    return nearest_cluster;
}

bool assign_clusters_to_points(int start, int end)
{
    int old_cluster = -1, new_cluster = -1;

    for (int i = start; i < end; i++)
    {
        old_cluster = data[i].cluster;
        new_cluster = get_closest_centroid(i, k);

        data[i].cluster = new_cluster; // Assign a cluster to the point i
        if (old_cluster != new_cluster)
        {
            something_changed = true;
        }
    }
    return something_changed;
}
void update_cluster_centers()
{
    /* Update the cluster centers */
    int c;

    memset(count, 0, sizeof(count));
    for(int i=0; i<MAX_CLUSTERS; i++){
        temp[i].cluster = 0;
        temp[i].x = 0.0;
        temp[i].y = 0.0;
    }

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


void* kmean_thread(void* id)
{    
    int iter;
    struct th *t_info = (struct th*)id;
    int start, end, size_n, thread_div;

    size_n= (int)t_info->size;

    if(size_n == -1){         //if size = -1, this indicates its last thread with extra work
        start = t_info->start*t_info->factor;
        end = t_info->end;
    }
    else if(size_n < 4){      //If size < 8, indicates we have less size than threads
        thread_div=1;
        start = t_info->start+1*thread_div;
        end = (t_info->end)*thread_div;
    } else{                   //Else standard case where we divide workload based on 8 threads
        thread_div = size_n/4;
        start = t_info->start*thread_div;
        end = (t_info->end)*thread_div;
    }
    /*
    struct th:
        start:  where iteration start for thread
        end:    end iteration
        size:   size of file (if size=-1, used as flag to indicate last iteration)
        factor: what to multiply with to get the correct iteration for all threads
    */
    do {
        pthread_barrier_wait(&barrier);
        if(start == 0){ // First thread is only thread that can set something_changed
            something_changed = false;
            iter++; // Keep track of number of iterations
        }
        pthread_barrier_wait(&barrier);

        something_changed = assign_clusters_to_points(start,end);

        pthread_barrier_wait(&barrier);
        if(start == 0){
            update_cluster_centers();
        }
        pthread_barrier_wait(&barrier);


    } while (something_changed);

}

int kmean(int k)
{    
    children = malloc(N * sizeof(pthread_t));
    int n_threads = 4;           // Number of threads to be created
    int rest=0; // Modulus rest
    int factor = 0; //how much to multiply with if n%threads!= 0
    int flag = 0; //flag to indicate n%threads!=0
    pthread_barrier_init(&barrier, NULL, n_threads); // Initialize barrier to wait for n_threads amount
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
        pthread_join(children[id], NULL); // Collect/join result from the threads
    }

    free(children);
    free(t_info);
    pthread_barrier_destroy(&barrier);
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
}

int start_kmeans(char* k, int N, char* path, int pID)
{
    int input_k = atoi(k); // Number of clusters
    read_data(input_k, N, path); // Read input file
    kmean(input_k); // Start kmeans
    write_results(pID); // Write to file
    return 0;
}
// int main()
// {
//     read_data(9, 1797 , "../objects/kmeans-data.txt"); 
//     kmean(k);
//     write_results(0);

// }