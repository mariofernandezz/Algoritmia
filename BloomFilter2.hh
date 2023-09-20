#ifndef BLOOM_FILTER
#define BLOOM_FILTER

#include <vector>
#include <string>

using namespace std;


class BloomFilter {

    private:
        //dictionary size
        int n;
        //false positive probability
        float p;
        //number of bits
        int m;
        vector<int> mask;
        //number of hash functions
        int k;

        int stringToInt(const string& word);
        int fHash0(int base, int exponent);
        int fHash1(int prime, int sNum);
        int fHash2(int prime, int sNum);
        int fHash3(int prime, int sNum);
        int fHash4(int prime, int sNum);

    public:
        BloomFilter(int n2, float p2);
        void insertWord(const string& word);
        int search(const string& auxWord);
        void print(int i);
        int size();
};

#endif
