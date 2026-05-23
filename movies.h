#ifndef MOVIES_H
#define MOVIES_H
 
#include <string>
#include <vector>
#include <cstring>
using namespace std;
 

struct Movie {
    string name;
    double rating;
};
 
struct MovieCmp {
    bool operator()(const Movie& a, const Movie& b) const {
        if (a.rating != b.rating) return a.rating > b.rating;
        return a.name < b.name;
    }
};
 
class MovieTrie {
public:
    MovieTrie();
    ~MovieTrie();
 
    void insert(const string& name, double rating);
 
    vector<Movie> search(const string& prefix) const;
 
private:
    struct TrieNode {
        TrieNode* children[128];
        Movie* movie; 
 
        TrieNode() : movie(nullptr) {
            memset(children, 0, sizeof(children));
        }
        ~TrieNode() {
            for (int i = 0; i < 128; i++) delete children[i];
            delete movie;
        }
    };
 
    TrieNode* root;
 
   
    void collect(TrieNode* node, vector<Movie>& result) const;
};
 
#endif
 
