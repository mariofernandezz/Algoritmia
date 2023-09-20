#ifndef WORD_SEARCH
#define WORD_SEARCH

#include <vector>
#include <string>

using namespace std;

typedef vector<char> Row;
typedef vector<Row> Matrix;
typedef vector<pair<int,int>> PosChars;
typedef vector<bool> BoolRow;
typedef vector<BoolRow> BoolMatrix;

class WordSearch {
    
    private:  
        Matrix wordSearch;
        
        bool addWordRec(int posi, int posj, const string& word, int n, PosChars& posChars);
        bool posOk(int posi, int posj, char c);     
        bool posRepeated(int posi, int posj, const PosChars& posChars);  
        
        
    public:
        WordSearch(int n);

        void addWord(const string& word); 
        bool posOk(int i, int j);
        int getSize();
        char toChar(int i, int j);
        void print();
};

#endif
