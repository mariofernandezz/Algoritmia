#include "Trie.hh"
#include <iostream>
#include <string>
using namespace std;
 
// Define the character size
#define CHAR_SIZE 128

// PRIVATE FUNCTIONS
int Trie::numberChildren() {
    Trie* curr = this;
    return curr->children.size();
}

// PUBLIC FUNCTIONS 
// Iterative function to insert a key into a Trie
void Trie::insert(string key)
{
    // start from the root node
    // s    o   p   a
    Trie* curr = this;
    string s = "";
    for (int i = 0; i < key.length(); i++)
    {
        s = key[i];
        if(curr->children.find(s) == curr->children.end()) {
            curr->children.insert({s,new Trie()});
        }
        curr = curr->children.find(s)->second;
        curr->word = s; // node has value
    }
    // mark the current node as a leaf
    curr->isCompleteWord = true;
}

bool Trie::search(string key)
{
    // start from the root node
    Trie* curr = this;
    string s = "";
    for(int i=0; i<key.length(); ++i) {
        s.push_back(key[i]);
        if(curr->existChildrenWithKey(s)) {
            curr = curr->nodeWithKey(s);
            s = "";
        }
        else if(!existsChildWithKeyPrefix(s)) {
            return false;
        }
    }
    return curr->isCompleteWord;
}

bool Trie::existChildrenWithKey(string key) {
    Trie* curr = this;
    return (curr->children.find(key) != curr->children.end());
}

Trie* Trie::nodeWithKey(string key) {
    Trie* curr = this;
    return curr->children.find(key)->second;
}

bool Trie::existsChildWithKeyPrefix(string prefix) {
    Trie* curr = this;
    for(auto it = curr->children.begin(); it!=curr->children.end(); ++it) {
        auto res = mismatch(prefix.begin(), prefix.end(), it->first.begin());
        if(res.first==prefix.end()) {
            return true;
        }
    }
    return false;
}


void Trie::patricia() {
    Trie* curr = this;
    int nChildren = curr->numberChildren();
    if (nChildren != 0) {
        if(nChildren==1 and !curr->isCompleteWord) {
            auto it = this->children.begin();
            curr->word = curr->word + it->second->word; // curr value + son value
            curr->isCompleteWord = it->second->isCompleteWord;  //If children was completeWord, this is also
            curr->children = it->second->children;  // current children becomes children's children
            curr->patricia();
        }
        else {
            map<string,Trie*> newChildren;
            for(auto it = curr->children.begin(); it!=curr->children.end();) {
                it->second->patricia();
                string newName = it->second->word;
                Trie* newTrie = it->second;
                curr->children.erase(it++);
                newChildren.insert({newName,newTrie});
            }
            curr->children = newChildren;
        }
    }
}