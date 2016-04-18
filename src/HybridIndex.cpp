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



/*
 * Uses the string reference without copying. Starts the index creation process.
 */
HybridIndex :: HybridIndex(unsigned int kmer_length, const string &sequence) : kmer_length(kmer_length), sequence(sequence){
    create_index();
}

HybridIndex :: ~HybridIndex(){};

/*
 * Implements the HIA algorithm specified in the paper. 
 */
void HybridIndex :: create_index(){
    if(sequence.size() == 0 || kmer_length == 0){
        throw logic_error("Cannot create index of a zero length sequence and/or kmer.");
    }


    hash_table.clear();
    suffix_array.clear();

    //Find size of alphabet (if a character does not appear in the reference sequence, then it will not be counted)
    set<char> alphabet;
    for(char c : sequence){
        alphabet.insert(c);
    } 

    //Create a map from kmer to counts of the kmer
    map<string,int> counts;

    int num_alphabet = alphabet.size();
    auto alphabet_it = begin(alphabet);
    char start = *alphabet_it;
    
    string number = string(kmer_length, start);
    int capacity = pow(num_alphabet, kmer_length);
    
    vector<int> counter(num_alphabet);

    for(int i = 0; i < capacity; i++){
        counter[0]++;
        number[0] = *(alphabet_it++);
        //Carry
        int j = 0;
        while(counter[j] >= num_alphabet){
            alphabet_it = begin(alphabet);
            counter[j] = 0;
            number[j] = start;
        }
        //Add entry
        counts[number] = 0;
    }

    int max = sequence.size()-kmer_length+1;
    for(int i = 0; i < max; i++){
        counts[sequence.substr(i, kmer_length)]++;
    }

    //Setup suffix array
    suffix_array.resize(sequence.size()-kmer_length+1);

    for(unsigned int i = 0; i < suffix_array.size(); i++){
        suffix_array[i] = i;
    }

    //Sort suffix array using the reference sequence
    sort(begin(suffix_array), end(suffix_array),
        [this](const int a, const int b){
            //compare using the ints as indices into the sequence
            return sequence.compare(a, string::npos, sequence, b, string::npos) < 0;
        });

    //Setup hash table
    hash_table.reserve(capacity);

    int index = 0;
    for(auto elm : counts){
        hash_table[elm.first] = pair<int, int>(index, elm.second);
        index += elm.second;
    }
}

/*
 * Lookup kmer in index, returns a list of indices into index.
 * kmer must be at least kmer_length characters long.
 * Uses a seed and extend algorithm for longer kmer lengths.
 */
vector<int> HybridIndex::query(const string &kmer){
    unsigned int size = kmer.size();
    if(size < kmer_length){
        vector<int> v;
        return v;
    }

    //Find sub-suffix array
    string kmer_substr = kmer.substr(0, kmer_length);
    pair<int, int> entry = hash_table[kmer_substr];

    //Find specific part of suffix array
    auto start_it = begin(suffix_array);
    advance(start_it, entry.first);
    auto end_it = begin(suffix_array);
    advance(end_it, entry.first + entry.second);

    //Extend (could be done more effeciently with a double binary search)
    vector<int> v;
    for(; start_it != end_it; start_it++){
        if(sequence.compare(*start_it, size, kmer) == 0){
            v.push_back(*start_it);
        }
    }

    return v;
}