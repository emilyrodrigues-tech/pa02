
#include "movies.h"
#include <algorithm>

MovieTrie::MovieTrie() : root(new TrieNode()) {}

MovieTrie::~MovieTrie() {
    delete root;
}

void MovieTrie::insert(const string& name, double rating) {
    TrieNode* cur = root;
    for (unsigned char c : name) {
        if (!cur->children[c])
            cur->children[c] = new TrieNode();
        cur = cur->children[c];
    }
    if (!cur->movie)
        cur->movie = new Movie{name, rating};
    else
        cur->movie->rating = rating;
}

void MovieTrie::collect(TrieNode* node, vector<Movie>& result) const {
    if (!node) return;
    if (node->movie)
        result.push_back(*node->movie);
    for (int i = 0; i < 128; i++)
        if (node->children[i])
            collect(node->children[i], result);
}

vector<Movie> MovieTrie::search(const string& prefix) const {
    TrieNode* cur = root;
    for (unsigned char c : prefix) {
        if (!cur->children[c])
            return {}; 
        cur = cur->children[c];
    }

    vector<Movie> result;
    collect(cur, result);
    sort(result.begin(), result.end(), MovieCmp());
    return result;
}
