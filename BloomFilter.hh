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
        vector<unsigned char> mask;
        //number of hash functions
        int k;

        int fHash1(int prime, const string& word);

    public:
        BloomFilter();
        BloomFilter(int n2, float p2);
        void insertWord(const string& word);
        int search(const string& auxWord);
};

#endif
