//Spring'26
// Instructor: Diba Mirza
// Student name: Emily Rodrigues
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <map>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies
    map<string, double> movieMap; // keeps movies sorted alphabetically
    MovieTrie trie;               // used for prefix search in part 2

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
            movieMap[movieName] = movieRating;
            trie.insert(movieName, movieRating);
    }

    movieFile.close();

    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names
            for (auto& [name, rating] : movieMap)
                cout << name << ", " << fixed << setprecision(1) << rating << "\n";
            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    prefixFile.close();

    // store the best movie for each prefix (only when matches exist)
    vector<pair<string, Movie>> bestMovies;

    for (auto& prefix : prefixes) {
        //  For each prefix,
        //  Find all movies that have that prefix and store them in an appropriate data structure
        vector<Movie> matches = trie.search(prefix);

        //  If no movie with that prefix exists print the following message
        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << "\n";
        } else {
            // print all matches sorted by rating desc, then alphabetically
            for (auto& m : matches)
                cout << m.name << ", " << fixed << setprecision(1) << m.rating << "\n";
            cout << "\n";
            bestMovies.push_back({prefix, matches[0]});
        }
    }

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.
    for (auto& [prefix, m] : bestMovies)
        cout << "Best movie with prefix " << prefix << " is: "
             << m.name << " with rating "
             << fixed << setprecision(1) << m.rating << "\n";

    return 0;
}

/*
 * runtime analysis
 *
 * variables:
 *   n = number of movies in the dataset
 *   m = number of prefixes
 *   k = max number of movies that match a single prefix
 *   l = max number of characters in a movie name
 *
 * time complexity:
 * I used a trie for the prefix search. for each of the m prefixes:
 *   - walk down the trie to the prefix node: O(l)
 *   - collect all k matching movies with a DFS: O(k)
 *   - sort those k movies by rating then name: O(k log k)
 * so each prefix takes O(l + k log k), and for all m prefixes
 * the total is O(m * (l + k log k))
 *
 * space complexity:
 * in the worst case the trie has O(n * l) nodes (if no two movie names
 * share any prefix). each node has a fixed 128-pointer array plus one
 * Movie pointer. the result vector for each query is O(k) temporary space.
 * overall: O(n * l)
 *
 * tradeoffs:
 * I focused on keeping the time complexity low. the trie lets me go straight
 * to the right prefix node in O(l) instead of scanning all n movies each time.
 * the downside is memory -- every node allocates 128 pointers even if most
 * of them are null, so the constant factor on O(n*l) is pretty large. a radix
 * trie would fix that but it's a lot more code. since the priority here was
 * runtime I just went with the simpler version and accepted the extra memory.
 */

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
