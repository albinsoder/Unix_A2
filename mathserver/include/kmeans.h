#ifndef kmeans
#define kmeans

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_POINTS 4096
#define MAX_CLUSTERS 32

typedef struct point
{
    float x; // The x-coordinate of the point
    float y; // The y-coordinate of the point
    int cluster; // The cluster that the point belongs to
} point;

int	N;		// number of entries in the data
int k;      // number of centroids
point data[MAX_POINTS];		// Data coordinates
point cluster[MAX_CLUSTERS]; // The coordinates of each cluster center (also called centroid)

void read_data();
int get_closest_centroid(int i, int k);
bool assign_clusters_to_points();
void update_cluster_centers();
int kmean(int k);
void write_results();
int start_kmeans();

#endif
