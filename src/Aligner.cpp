#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>
#include <list>
#include <cmath>

#include "Aligner.h"

using namespace std;

struct match_region{
    long diagonal;
    unsigned int read_pos;
    unsigned int seq_pos;
    unsigned int length;
};

struct candidate_mr{
    list<match_region> mr_list;
    unsigned int matched_bases;

};

Aligner::Aligner(unsigned int kmer_length, const string &reference):
    kmer_length(kmer_length),
    reference(reference),
    index(kmer_length, reference){}

pair<GappedSequence, GappedSequence> Aligner::find_alignment(const string &read){
    return find_alignment(read, this->kmer_length);
}

pair<GappedSequence, GappedSequence> Aligner::find_alignment(const string &read, unsigned int kmer_length){
    if(kmer_length < this->kmer_length){
        cerr << "Warning: 0 results will be returned when finding an alignment using kmer sizes less than what was specfied when building the hybrid index.\n";
    }

    //Contains a list of diagonal, kmer read position pairs
    vector<match_region> matching_regions;
    for(unsigned int i = 0; i < read.length() - kmer_length + 1; i++){
        cout << read.substr(i, kmer_length) << endl;
        cout << index.query(read.substr(i, kmer_length)).size() << " results\n";
        for(unsigned int elm : index.query(read.substr(i, kmer_length))){
            matching_regions.push_back({elm-i, i, elm, kmer_length});
        }
    }

    //Sort MR's by diagonal then read position
    sort(begin(matching_regions), end(matching_regions),
        [](const match_region &a, const match_region &b){
            //compare using the ints as indices into the sequence
            if(a.diagonal == b.diagonal){
                return a.read_pos < b.read_pos;
            }else{
                return a.diagonal < b.diagonal;
            }
        });

    for(auto elm : matching_regions){
        cout << elm.diagonal << " " << elm.read_pos << " " << elm.seq_pos << " " << elm.length << endl;
    }

    //Merge MR's if they have identical diagonal values and successive offsets
    list<match_region> new_mrs;
    int cur_diagonal = matching_regions.front().diagonal;
    unsigned int cur_pos = matching_regions.front().read_pos;
    unsigned int seq_pos = matching_regions.front().seq_pos;
    unsigned int start_pos = cur_pos;
    unsigned int cur_length = kmer_length;
    for(auto elm : matching_regions){
        //Add merged MR's
        if(elm.diagonal != cur_diagonal || elm.read_pos != cur_pos){
            new_mrs.push_back({cur_diagonal, start_pos, seq_pos, cur_length-1});
            cur_diagonal = elm.diagonal;
            cur_pos = elm.read_pos;
            start_pos = cur_pos;
            cur_length = elm.length;
            seq_pos = elm.seq_pos;
        }
        cur_pos++;
        cur_length++;
    }

    //Add remaining MR
    new_mrs.push_back({cur_diagonal, start_pos, seq_pos, cur_length-1});

    cout << "New matching regions.";
    for(auto elm : matching_regions){
        cout << elm.diagonal << " " << elm.read_pos << " " << elm.seq_pos << " " << elm.length << endl;
    }


    //max_adjacency allowed for combining MR's into CAR's
    //TODO: better estimate
    int max_adjacency = 2;//0.1 * read.size();

    //Compute CAR's by merging MR's with diagonals with a different <= max_adjacency
    vector<candidate_mr> candidate_regions;
    cur_diagonal = new_mrs.front().diagonal;
    unsigned int cur_match = 0;
    list<match_region> l;
    for(auto elm : new_mrs){
        //Add merged MR's
        if(abs(elm.diagonal - cur_diagonal) <= max_adjacency){
            cout << "mr with diag " << elm.diagonal << " is in range of cur diag " << cur_diagonal << endl;
            l.push_back(elm);
            cur_match += kmer_length - abs(cur_diagonal - elm.diagonal);
        }else{
            cout << "mr with diag " << elm.diagonal << " is not in range of cur diag " << cur_diagonal << endl;
            candidate_regions.push_back({l, cur_match});
            l.clear();
            l.push_back(elm);
            cur_diagonal = elm.diagonal;
            cur_match = elm.length;
        }
    }

    candidate_regions.push_back({l, cur_match});

    //Sort CAR's
    sort(begin(candidate_regions), end(candidate_regions),
        [](const candidate_mr &a, const candidate_mr &b){
            return a.matched_bases > b.matched_bases;
        });

    for(auto car : candidate_regions){
        cout << car.matched_bases << " matches:\n";
        for(auto elm : car.mr_list){
            cout << elm.diagonal << " " << elm.read_pos << " " << elm.seq_pos << " " << elm.length << endl;
        }
    }

    //Align each candidate region
    for(auto cr : candidate_regions){
        //Align LMUR (Left-Most Unmatched Region)
        match_region mr = cr.mr_list.front();

        //Only do mismatch, insertion, and deletion search up to this many characters
        int search_limit = kmer_length;
        //Detect Mismatch case
        // int start_seq = cr.second[0].get<0>()
        // for(int i = 0; i < search_limit; i++){
            
        // }

        //Mixed case
        global_align(reference, read, pair<int, int>(mr.seq_pos - search_limit, mr.seq_pos), pair<int, int>(0, mr.read_pos));
    }


    return pair<GappedSequence,GappedSequence>(GappedSequence(read), GappedSequence(read));
}

pair<GappedSequence,GappedSequence> Aligner::global_align(const string &seq1, const string &seq2, pair<int, int> seq1_range, pair<int, int> seq2_range){
    return pair<GappedSequence,GappedSequence>(seq1, seq2);
}