#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <chrono>
#include "wordSearch.hh"

using namespace std;
using namespace std::chrono;

//                  NO N NE E SE S SO O
vector<int> dirI = {-1,0,1,1,1,0,-1,-1};
vector<int> dirJ = {-1,-1,-1,0,1,1,1,0};

typedef pair<string,PosChars> Result;

// words must have 20 positions
void chooseWordsFromDict(vector<string> dictionary, vector<string>& words) {

    int dicSize = dictionary.size();
    vector<int> positionsToChoose(dicSize);
    for(int i=0; i<dicSize; ++i) positionsToChoose[i] = i;

    for(int i=0; i<20; ++i) {
        int r = rand() % (dicSize-i);
        words[i] = dictionary[r];

        swap(dictionary[r], dictionary[dicSize - (i+1)]);
    }
}

int dichotomousSearch(const string& auxWord, const vector<string>& dictionary, int left, int right) {

    if (left > right) return 0;
    int m = (left + right) / 2;
    if (auxWord < dictionary[m]) {

        //prefix the same
        if(dictionary[m].find(auxWord) == 0) return 1;
        return dichotomousSearch(auxWord, dictionary, left, m - 1);
    }
    else if (auxWord > dictionary[m]) return dichotomousSearch(auxWord, dictionary, m + 1, right);
    //word found
    return 2;
}

//recursive call
void searchingWordsRec(const vector<string>& dictionary, WordSearch& wordSearch, int i, int j, list<Result>& result, BoolMatrix& visited, string& currentWord, PosChars& auxPos) {

    if(not visited[i][j]) {

        visited[i][j] = true;
        //creamos string auxiliar para ver si el nuevo prefijo existe
        string auxWord = currentWord;
        auxWord.push_back(wordSearch.toChar(i,j));
        int exists = dichotomousSearch(auxWord, dictionary, 0, dictionary.size()-1);
        int auxI, auxJ;
        // existe la palabra entera
        if (exists == 2) {

            auxPos.push_back({i,j});
            result.push_back({auxWord,auxPos});
            //llamada recursiva
            for(int k = 0; k < 8; k++) {

                auxI = i + dirI[k];
                auxJ = j + dirJ[k];
                if(wordSearch.posOk(auxI,auxJ)) searchingWordsRec(dictionary, wordSearch, auxI, auxJ, result, visited, auxWord, auxPos);
            }
            auxPos.pop_back();
        }
        //existe el prefijo
        else if (exists == 1){

            auxPos.push_back({i,j});
            for(int k = 0; k < 8; k++) {

                auxI = i + dirI[k];
                auxJ = j + dirJ[k];
                if(wordSearch.posOk(auxI,auxJ)) searchingWordsRec(dictionary, wordSearch, auxI, auxJ, result, visited, auxWord, auxPos);
            }
            auxPos.pop_back();
        }
        visited[i][j] = false;
    }
}

//first call to recursive searching words
void searchingWords(const vector<string>& dictionary, WordSearch& wordSearch, int n, int i, int j, list<Result>& result) {

    BoolMatrix visited(n, BoolRow(n, false));
    string w = "";
    PosChars auxPos;
    searchingWordsRec(dictionary, wordSearch, i, j, result, visited, w, auxPos);
}

int main() {
    srand(time(NULL));

    //vector<string> dictionary = {"ABUELO", "ARBOL", "BALON", "BICICLETA", "COLILLA", "CHORRA", "DICCIONARIO", "DORMIR", "ELECTIRCO", "ESPANYA", "FAROLA", "FUENTE", "GATO", "GORILA", "HELICOPTERO", "HORMIGA", "IGUALAR", "ISLAM", "JUEGO", "JORDAN", "KIWI", "KILO", "LIBRA", "LIMON", "MONEDA", "MESA", "NORIA", "NUBE", "ORIFICIO", "OLER", "PALOMA", "PUEBLO", "QUESO", "QUERER", "RUIDO", "RUEGO", "SORIA", "SUERTE", "TIRAR", "TITAN", "UVA", "UMBRAL", "VACACIONES", "VOLVER", "WATERPOLO", "WIKI", "XAVI", "XINO", "YOGUR", "YAYO", "ZEBRA", "ZAPATO"};
		vector<string> dictionary;
    string w;
		while(cin>>w) {
        dictionary.push_back(w);
    }

    vector<string> words(20);

		auto start = high_resolution_clock::now();

    sort(dictionary.begin(), dictionary.end());

		auto stop = high_resolution_clock::now();
		auto duration_sort = duration_cast<microseconds>(stop-start);

    chooseWordsFromDict(dictionary,words);

    int n = rand() % 16 + 10;
		n = 20;
    WordSearch wordSearch(n);

    // always 20 words to search
    for (int i = 0; i < 20; i++) {

        wordSearch.addWord(words[i]);
    }

    sort(words.begin(), words.end());

    int wordsSize = words.size();
    for (int i = 0; i < wordsSize; i++) cout << words[i] << "  ";
    cout << endl << endl;
    //aqui la sopa ya estaria creada y solo tocaria buscar las palabras
    wordSearch.print();

    list<Result> result;

		start = high_resolution_clock::now();
    for(int i = 0; i < n; i++) {

        for(int j = 0; j < n; j++) {

            searchingWords(dictionary, wordSearch, n, i, j, result);
        }
    }
		stop = high_resolution_clock::now();
		auto duration_search = duration_cast<microseconds>(stop-start);

    cout << endl << "WORDS FOUND" << endl << "------------------------------------------------------------" << endl;
    list<Result>::const_iterator it;
    for(it = result.begin(); it!= result.end(); ++it) {

        Result r = *it;
        cout << r.first << " -> ";
        int vsize = r.second.size();
        for (int i = 0; i < vsize; i++) {

            cout << '{' << r.second[i].first << ',' << r.second[i].second << "}";
            if(i != vsize -1) cout << " , ";
        }
        cout << endl;
    }


		cout << "sort: " << duration_sort.count() << endl;
		cout << "search: " << duration_search.count() << endl;
}
