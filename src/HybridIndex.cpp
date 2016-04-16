#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <exception>


#include "HybridIndex.h"

using namespace std;



HybridIndex :: HybridIndex(unsigned int kmer_length) : kmer_length(kmer_length){}

/*
 * Uses the string reference without copying. Starts the index creation process.
 */
HybridIndex :: HybridIndex(unsigned int kmer_length, string &sequence) : kmer_length(kmer_length), sequence(sequence){
    create_index();
}

HybridIndex :: ~HybridIndex(){};

/*
 * Implements the HIA algorithm specified in the paper. 
 */
void HybridIndex :: create_index(){
    if(sequence.size() == 0 || kmer_length == 0){
        throw logic_error("Cannot create index of a zero sequence and/or kmer length.");
    }


    map<string,int> counts;

    for(unsigned int i = 0; i < sequence.size()-kmer_length+1; i++){
        counts[sequence.substr(i, kmer_length)]++;
    }

    cout << "created counts map\n";

    //Modify the map so it now maps chars to current index into suffix array
    int index = 0;
    for(auto elm : counts){
        int temp = elm.second;
        counts[elm.first] = index;
        index += temp;
    }
    cout << "updated counts map\n";

    //Setup suffix array
    suffix_array.resize(sequence.size()-kmer_length+1);

    for(unsigned int i = 0; i < suffix_array.size(); i++){
        suffix_array[i] = i;
    }

    for(auto s : suffix_array){
        cout << s << " ";
    }

    cout << endl;

    cout << "suffix array initialized\n";

    //Sort suffix array using the reference sequence
    sort(suffix_array.begin(), suffix_array.end(),
        [this](const int a, const int b){
            //compare using the ints as indices into the sequence
            cout << a << " < " << b <<  " is " << sequence.compare(a, string::npos, sequence, b, string::npos) << endl;
            // return a > b;
            return sequence.compare(a, string::npos, sequence, b, string::npos);
        });

    cout << "sorted suffix array\n";

    //Find size of alphabet (if a character does not appear in the reference sequence, then it will not be counted)
    set<char> alphabet;
    for(char c : sequence){
        alphabet.insert(c);
    }

    //Setup hashtable
    int max = alphabet.size();
    vector<int> counter(kmer_length, 0);
    auto counts_it = counts.begin();
    auto alphabet_it = alphabet.begin();
    char start = *alphabet.begin();
    string number = string(kmer_length, start);
    
    hash_table.reserve(pow(max, kmer_length));

    for(unsigned int i = 0; i < hash_table.size(); i++){
        counter[0]++;
        number[0] = *(alphabet_it++);
        //Carry
        int j = 0;
        while(counter[j] >= max){
            alphabet_it = alphabet.begin();
            counter[j] = 0;
            number[j] = start;
        }
        //Add entry (might be incorrect)
        hash_table[number] = counts_it->second;
        if((counts_it->first).compare(number) == 0){
            counts_it++;
        }
    }
}