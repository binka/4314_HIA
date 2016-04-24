#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>
#include <list>
#include <cmath>

#include "Aligner.h"

using namespace std;

typedef list<tuple<int, int, int>> mr_list;
typedef vector<pair<mr_list, int>> car_list;

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

    //Sort MR's by diagonal then read position
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

    //Merge MR's if they have identical diagonal values and successive offsets
    //diagonal, read position, length
    mr_list new_mrs;
    int cur_diagonal = matching_regions.front().first;
    int cur_pos = matching_regions.front().second;
    int start_pos = cur_pos;
    int cur_length = kmer_length;
    for(auto elm : matching_regions){
        //Add merged MR's
        if(elm.first != cur_diagonal || elm.second != cur_pos){
            new_mrs.push_back(make_tuple(cur_diagonal, start_pos, cur_length-1));
            cur_diagonal = elm.first;
            cur_pos = elm.second;
            start_pos = cur_pos;
            cur_length = kmer_length;
        }
        cur_pos++;
        cur_length++;
    }

    //Add remaining MR
    new_mrs.push_back(make_tuple(cur_diagonal, start_pos, cur_length-1));

    for(auto elm : new_mrs){
        cout << "(" << get<0>(elm) << " " << get<1>(elm) << " " << get<2>(elm) << ") ";
    }

    cout << endl;


    //max_adjacency allowed for combining MR's into CAR's
    //TODO: better estimate
    int max_adjacency = 2;//0.1 * read.size();

    //Compute CAR's by merging MR's with diagonals with a different <= max_adjacency
    vector<pair<list<tuple<int, int, int>>, int>> candidate_regions;
    cur_diagonal = get<0>(new_mrs.front());
    int cur_match = 0;
    mr_list l;
    for(auto elm : new_mrs){
        //Add merged MR's
        if(abs(get<0>(elm) - cur_diagonal) <= max_adjacency){
            cout << "mr with diag " << get<0>(elm) << " is in range of cur diag " << cur_diagonal << endl;
            l.push_back(elm);
            cur_match += kmer_length - abs(cur_diagonal - get<0>(elm));
        }else{
            cout << "mr with diag " << get<0>(elm) << " is not in range of cur diag " << cur_diagonal << endl;
            candidate_regions.push_back(pair<mr_list, int>(l, cur_match));
            l.clear();
            l.push_back(elm);
            cur_diagonal = get<0>(elm);
            cur_match = get<2>(elm);
        }
    }

    candidate_regions.push_back(pair<mr_list, int>(l, cur_match));

    //Sort CAR's
    sort(begin(candidate_regions), end(candidate_regions),
        [](const pair<mr_list, int> a, const pair<mr_list, int> b){
            return a.second > b.second;
        });

    for(auto car : candidate_regions){
        cout << car.second << " matches:\n";
        for(auto mr : car.first){
            cout << get<0>(mr) << " " << get<1>(mr) << " " << get<2>(mr) << endl;
        }
    }

    return pair<GappedSequence,GappedSequence>(GappedSequence(read), GappedSequence(read));
}