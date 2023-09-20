#include <iostream>
#include <string>
#include <map>

using namespace std;

class Trie {
    private:
        int numberChildren();
    
    public:
        string word;
        bool isCompleteWord;
        map<string,Trie*> children;
    
        // Constructor
        Trie()
        {
            word = "";
            this->isCompleteWord = false;
        }

        void insert(string);
        bool search(string);
        bool existChildrenWithKey(string);
        bool existsChildWithKeyPrefix(string);
        Trie* nodeWithKey(string);
        void patricia();
};
