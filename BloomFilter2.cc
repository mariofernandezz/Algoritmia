#include "BloomFilter.hh"
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

vector<int> primeElevatedNumbers2 = {51, 27, 10, 23, 43, 39, 23, 29, 51, 37, 14, 2};
vector<int> primeElevatedNumbers = {22, 34, 4, 17, 49, 21, 15, 36, 59, 33, 7, 12};
vector<unsigned char> bitMask = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00};

//private functions

int BloomFilter::stringToInt(const string& word) {

    int num = 0, wordSize = word.size();

    for (int i = 0; i < wordSize; i++) {

        num = 43*num + int(word[i]);
        num = num % m;
    }
    return num;
}

//exponenciació ràpida (O(logn))
int BloomFilter::fHash0(int base, int exponent) {

    if (exponent == 0) {

        return 1;
    }

    unsigned int pow = fHash0(base, exponent/2);
    if (exponent%2 == 0) pow *= pow;
    else pow *= pow * base;
    pow = pow % m;
    return pow;
}

int BloomFilter::fHash1(int prime, int sNum) {

    int num = sNum;

    for (int i = 0; i < 5; i++) {

        num += m / (i+prime);

    }
		num = num % m;
    return num;
}

int BloomFilter::fHash2(int prime, int sNum) {

    unsigned int num = 0;

    for (int i = 0; i < 10; i++) {

        num += num^(sNum*(prime+prime));
				num = num % m;
    }
    return num;
}

int BloomFilter::fHash3(int prime, int sNum) {

    unsigned int num = 0;

    for (int i = 0; i < 10; i++) {
        num += num | (sNum/prime);
				num = num % m;
    }
    return num;
}

int BloomFilter::fHash4(int prime, int sNum) {

     unsigned int num = 0;

    for (int i = 0; i < 10; i++) {

          num += 88* (sNum*prime);
					num = num % m;
    }
    return num;
}

//public functions

BloomFilter::BloomFilter(int n2, float p2) {

    n = n2;
    p = p2;
    float aux = -((n * log(p))/(pow(log(2), 2)));
    m = ceil(aux);
		//m = ceil((5*n)/log(2));
		m = 3500000;
		//cout << m << ' ' << aux << endl;
    mask = vector<int>(m/*ceil((float)m/8)*/,0);
    k = ceil((m / n) * log(2));
		cout << m << ' ' << k << endl;
    if (k == 0) k=1;
}

void BloomFilter::insertWord(const string& word) {

    int numWord = stringToInt(word);
    int numHash/*, posMask, posBit*/;
    int j = 0;
    cout << word<< ' ' << numWord << ' ';
    for(int i = 0; i < 5; i++, j++) {

        if (j == primeElevatedNumbers.size()) j = 0;
        if (i % 5 == 0) numHash = fHash0(primeElevatedNumbers[j], numWord);
				else if (i % 5 == 1) numHash = fHash1(primeElevatedNumbers2[j], numWord);
        else if (i % 5 == 2) numHash = fHash2(primeElevatedNumbers[j], numWord);
        else if (i % 5 == 3) numHash = fHash3(primeElevatedNumbers2[j], numWord);
        else numHash = fHash4(primeElevatedNumbers[j], numWord);
        //posMask = numHash / 8;
        //posBit = numHash % 8;
				//cout << "PM:"<< posMask << ' ' <<  "PB:" << posBit << ' ' << '(' << i << ") ";
				cout << "POS:" << numHash <<' ' << '(' << i << ") ";
				if(mask[numHash]==0) mask[numHash]=1;
        //mask[posMask] = mask[posMask] | bitMask[posBit];
				//cout << i << ' ' << j << endl;
    }
		cout << endl;
}

int BloomFilter::search(const string& auxWord) {

    int numWord = stringToInt(auxWord);
    int numHash/*, posMask, posBit*/;
    int j = 0;
		//cout << endl << auxWord << "   ";
    for(int i = 0; i < 5; i++, j++) {

        if (j == primeElevatedNumbers.size()) j = 0;
        if (i % 5 == 0) numHash = fHash0(primeElevatedNumbers[j], numWord);
        else if (i % 5 == 1) numHash = fHash1(primeElevatedNumbers2[j], numWord);
        else if (i % 5 == 2) numHash = fHash2(primeElevatedNumbers[j], numWord);
        else if (i % 5 == 3) numHash = fHash3(primeElevatedNumbers2[j], numWord);
        else numHash = fHash4(primeElevatedNumbers[j], numWord);
      //  posMask = numHash / 8;
      //  posBit = numHash % 8;
				//bool f = mask[posMask] & bitMask[posBit];
				//cout<<  "PM:"<< posMask << ' ' <<  "PB:" << posBit << ' ' << f << '(' << i << ')';
        //if((mask[posMask] & bitMask[posBit])==bitMask[8]) return 0;
//				cout << "POS:" << numHash <<' ' << '(' << i << ") ";
				if(mask[numHash] == 0) return 0;
    }
    return 1;
}

void BloomFilter::print (int i) {

    cout << int(mask[i]);
}

int BloomFilter::size() {

    return mask.size();
}
