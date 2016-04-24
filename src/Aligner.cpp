#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>

#include "Aligner.h"

using namespace std;

Aligner::Aligner(int kmer_length, const string &reference):
    kmer_length(kmer_length),
    reference(reference),
    index(kmer_length, reference){}

pair<GappedSequence, GappedSequence> Aligner::find_alignment(const string &read){
    return find_alignment(read, this->kmer_length);
}

pair<GappedSequence, GappedSequence> Aligner::find_alignment(const string &read, int kmer_length){
    if(kmer_length < this->kmer_length){
        cerr << "Warning: 0 results will be returned when finding an alignment using kmer sizes less than what was specfied when building the hybrid index.\n";
    }

    //Contains a list of diagonal, kmer read position pairs
    vector<pair<int, int>> matching_regions;
    for(unsigned int i = 0; i < read.length() - kmer_length + 1; i++){
        cout << read.substr(i, kmer_length) << endl;
        cout << index.query(read.substr(i, kmer_length)).size() << " results\n";
        for(int elm : index.query(read.substr(i, kmer_length))){
            matching_regions.push_back(pair<int, int>(elm-i, i));
        }
    }

    sort(begin(matching_regions), end(matching_regions),
        [](const pair<int, int> a, const pair<int, int> b){
            //compare using the ints as indices into the sequence
            if(a.first == b.first){
                return a.second < b.second;
            }else{
                return a.first < b.first;
            }
        });

    for(auto elm : matching_regions){
        cout << elm.first << " " << elm.second << endl;
    }

    return pair<GappedSequence,GappedSequence>(GappedSequence(read), GappedSequence(read));
}