#ifndef kmeans
#define kmeans

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>


#define MAX_POINTS 4096
#define MAX_CLUSTERS 32

typedef struct point
{
    float x; // The x-coordinate of the point
    float y; // The y-coordinate of the point
    int cluster; // The cluster that the point belongs to
} point;

struct th
{
    int start;
    int end;
    int size;
    int factor;
};
pthread_t *children;
pthread_barrier_t barrier;

int	N;		// number of entries in the data
int k;      // number of centroids
point data[MAX_POINTS];		// Data coordinates
point cluster[MAX_CLUSTERS]; // The coordinates of each cluster center (also called centroid)

void read_data(int input_k, int input_N, char* path);
int get_closest_centroid(int i, int k);
void* assign_clusters_to_points(void* id);
void update_cluster_centers();
void update_cluster_centers_continue(int count[MAX_CLUSTERS], point temp[MAX_CLUSTERS], int start);
int kmean(int k);
void write_results(int pID);
int start_kmeans(char* k, int N, char* path, int pID);

#endif
