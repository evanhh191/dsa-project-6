#include <limits>

typedef int Vertex;

const int max_size = 50; 

template <class Weight>
class Digraph {
public:
    Digraph(Weight** W, int n);

    Weight get_shortest_path(const Vertex &u, const Vertex &v);

protected:
    int count;
    Weight adjacency[max_size][max_size];
    const Weight infinity = std::numeric_limits<Weight>::max();
    Weight distances[max_size];
    void set_distances(const Vertex &source, Weight distances[]);
};

template <class Weight>
Digraph<Weight>::Digraph(Weight** W, int n)
{
    count = n;
    for (int i = 0; i < n && i < max_size; i++) {
        for (int j = 0; j < n && j < max_size; j++) {
            adjacency[i][j] = W[i][j];
        }
    }
}

template <class Weight>
Weight Digraph<Weight>::get_shortest_path(const Vertex &u, const Vertex &v)
{
    for (int i = 0; i < count; i++) { // reset distances
        distances[i] = std::numeric_limits<Weight>::max();
    }
    set_distances(u, distances);
    return distances[v];
}

template <class Weight>
void Digraph<Weight>::set_distances(const Vertex &source, Weight distance[])
{
    Vertex v, w;
	bool found[max_size]; // Vertices found in S
	for (v = 0; v < count; v++) {
		found[v] = false;
		distance[v] = adjacency[source][v];
	}
	// Initialize with vertex source alone in the set S
	found[source] = true; 
	distance[source] = 0;
    for (int i = 1; i < count; i++) { 
		// Add one vertex v to S on each pass
		Weight min = infinity;
		for (w = 0; w < count; w++) 
		    if ( ! found[w] )
		        if (distance[w] < min) {
		            v = w;
		            min = distance[w];
		        }
		found[v] = true;
        // update the distance to each vertex that is directly connected to S, but not in S
		for (w = 0; w < count; w++) 
		    if (!found[w])
		        if (min + (long long)adjacency[v][w] < distance[w]) // avoid overflow
		            distance[w] = min + adjacency[v][w];
	}

}

