#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "Utility.h"
//#include "List.h"
#include "Digraph.h"

using namespace std;

/*
 * Handle command line error
 */
struct CommandLineException
{
   CommandLineException()
   {
      cout << endl;
      cout << "Incorrect command line arguments." << endl;
   }
};

/*
 * Handle exception when accessing a file/folder
 */
struct FileException {
  FileException(const string file) {
    cout << endl << "'" << file << "'" << " could not be accessed or contains corrupted content." << endl;
  }
};

/*
 * Handle exception when the input data is invalid
 */
struct FileContentException {
  FileContentException() {
    cout << "Input data could not be correctly parsed." << endl;
  }
};

int string_to_int(const string& s){
   istringstream ss(s);
   int n;
   ss >> n;
   return n;
}

bool isNumber(const string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return !str.empty();
}

void query(ifstream &in1, ifstream &in2, ofstream &o) {
    // get input graph
    // get number of cities
    string first;
    in1 >> first;
    if (!isNumber(first))
        throw FileContentException();
    int size = string_to_int(first);
    
    // get the names of the cities
    string name;
    // because of the limitation of the adjacency matrix, we have to do the mapping here
    // adjacncy list or linked list based implementation can integrate city information
    // to make things easier, we use int type for Vertex in Digraph 
    map<string, int> cities;    // map each city to a vertex (with an index)
    for (int i = 0; i < size && in1 >> name; i++) {
        cities[name] = i;
    }   

    // read the weights
    int count = 0;
    string weight;
    int** adj = new int *[size];
    for(int i = 0; i < size; i++)
        adj[i] = new int[size];

    while (in1 >> weight && count < size * size) {
        if (weight == "INF") {  // no connection between two vertices
            adj[count / size][count % size] = numeric_limits<int>::max();
        }
        else if (!isNumber(weight)) {
            throw FileContentException();
        }
        else {
            adj[count / size][count % size] = string_to_int(weight);
        }
        count++;
    }

    if (count != size * size)
        throw FileContentException();
    
    Digraph<int> graph(adj, size);

    // get queries
    // one query per line
    for (string line; getline(in2, line);) {
        istringstream ss(line);
        string src, dst;
        ss >> src;
        ss >> dst;
        if (cities.find(src) == cities.end() || cities.find(dst) == cities.end()) {
            cout << "Invalid query: the queried city is not in the map." << endl;
        }
        else {
            int result = graph.get_shortest_path(cities[src], cities[dst]);
            if (result == numeric_limits<int>::max()) {
                o << "No available path." << endl;
            }
            else {
                o << result << endl;
            }
        }
    }

    // clean up
    for (int i = 0; i < size; i++) {
        delete[] adj[i];
    }
    delete[] adj;
}

int main(int argc, char* argv[])
{
  try {
    // handle command line input
    if (argc != 4) {
        throw CommandLineException();
    }

    ifstream input_file1, input_file2;
    ofstream output_file;
    input_file1.open(argv[1]);  // adjacency matrix for the graph
    input_file2.open(argv[2]);  // queries
    output_file.open(argv[3]);  // query results
    if (!input_file1) {
        throw FileException(argv[1]);
    }
    if (!input_file2) {
        throw FileException(argv[2]);
    }
    if (!output_file) {
        throw FileException(argv[3]);
    }

    query(input_file1, input_file2, output_file);

    // clean up
    input_file1.close();
    input_file2.close();
    output_file.close();
  }
  catch(...)
  {
      cout << "Program ended." << endl;
      exit(EXIT_FAILURE);
  }
  return 0;
}
