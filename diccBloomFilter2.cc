#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include "wordSearch.hh"
#include "BloomFilter.hh"

using namespace std;

//                  NO N NE E SE S SO O
vector<int> dirI = {-1,0,1,1,1,0,-1,-1};
vector<int> dirJ = {-1,-1,-1,0,1,1,1,0};
int maxSize = 0;

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

//recursive call
void searchingWordsRec(BloomFilter& bloomFilter, WordSearch& wordSearch, int i, int j, list<Result>& result, BoolMatrix& visited, string& currentWord, PosChars& auxPos, int cont) {

    //cout << "entra" << endl;
    if(not visited[i][j] and cont <= 4/*maxSize*/) {

        visited[i][j] = true;
        //creamos string auxiliar para ver si el nuevo prefijo existe
        string auxWord = currentWord;
        auxWord.push_back(wordSearch.toChar(i,j));
        int auxI, auxJ;
        if (cont < 3) {

            auxPos.push_back({i,j});
            for(int k = 0; k < 8; k++) {

                auxI = i + dirI[k];
                auxJ = j + dirJ[k];
                if(wordSearch.posOk(auxI,auxJ)) searchingWordsRec(bloomFilter, wordSearch, auxI, auxJ, result, visited, auxWord, auxPos, cont+1);
            }
            auxPos.pop_back();
        }
        else {

            int exists = bloomFilter.search(auxWord);
            // existe la palabra entera
            if (exists == 1) {

                auxPos.push_back({i,j});
                result.push_back({auxWord,auxPos});
                //llamada recursiva
                for(int k = 0; k < 8; k++) {

                    auxI = i + dirI[k];
                    auxJ = j + dirJ[k];
                    if(wordSearch.posOk(auxI,auxJ)) searchingWordsRec(bloomFilter, wordSearch, auxI, auxJ, result, visited, auxWord, auxPos, cont+1);
                }
                auxPos.pop_back();
            }
            //no existe la palabra
            else {

                auxPos.push_back({i,j});
                for(int k = 0; k < 8; k++) {

                    auxI = i + dirI[k];
                    auxJ = j + dirJ[k];
                    if(wordSearch.posOk(auxI,auxJ)) searchingWordsRec(bloomFilter, wordSearch, auxI, auxJ, result, visited, auxWord, auxPos, cont+1);
                }
                auxPos.pop_back();
            }
        }
        visited[i][j] = false;
    }
}

//first call to recursive searching words
void searchingWords(BloomFilter& bloomFilter, WordSearch& wordSearch, int n, int i, int j, list<Result>& result) {

    BoolMatrix visited(n, BoolRow(n, false));
    string w = "";
    PosChars auxPos;
    searchingWordsRec(bloomFilter, wordSearch, i, j, result, visited, w, auxPos, 1);
}

int main() {

    srand(time(NULL));

    //vector<string> dictionary = {"ABUELO", "ARBOL", "BALON", "BICICLETA", "COLILLA", "CHORRA", "DICCIONARIO", "DORMIR", "ELECTIRCO", "ESPANYA", "FAROLA", "FUENTE", "GATO", "GORILA", "HELICOPTERO", "HORMIGA", "IGUALAR", "ISLAM", "JUEGO", "JORDAN", "KIWI", "KILO", "LIBRA", "LIMON", "MONEDA", "MESA", "NORIA", "NUBE", "ORIFICIO", "OLER", "PALOMA", "PUEBLO", "QUESO", "QUERER", "RUIDO", "RUEGO", "SORIA", "SUERTE", "TIRAR", "TITAN", "UVA", "UMBRAL", "VACACIONES", "VOLVER", "WATERPOLO", "WIKI", "XAVI", "XINO", "YOGUR", "YAYO", "ZEBRA", "ZAPATO"};

		vector<string> dictionary;
		string s;
		while(cin >> s and s!=".") dictionary.push_back(s);

    vector<string> words(20);

    chooseWordsFromDict(dictionary,words);

    //int n = rand() % 16 + 10;
		int n = 10;
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

    int dicSize = dictionary.size();
		cout << dicSize << ' ' << 1/(float)dicSize<< endl;
    BloomFilter bloomFilter(dicSize,(1/(float)dicSize)); // min p = 0.0001 (por palabras de max tamaño 13)p = 0.000039 (more p could cause inefficiency , but it could be something more) en el cas de 25500 palabras no habria fallos
    for (int i = 0; i < dicSize; i++) {
			if(dictionary[i].size()>maxSize) maxSize = dictionary[i].size();
			bloomFilter.insertWord(dictionary[i]);
		}
    cout << endl;

		//Print de la mascara
    //for (int i = 0; i < bloomFilter.size(); i++) {
		//	if(i == 10000) cout << endl;
	//		bloomFilter.print(i);
	//	}
		cout << maxSize << endl;
    list<Result> result;
    for(int i = 0; i < n; i++) {

        for(int j = 0; j < n; j++) {

            searchingWords(bloomFilter, wordSearch, n, i, j, result);
        }
    }

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
}
