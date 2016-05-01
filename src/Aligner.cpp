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
    long read_pos;
    long seq_pos;
    long length;
};

struct candidate_mr{
    list<match_region> mr_list;
    unsigned int matched_bases;

};

typedef pair<GappedSequence, GappedSequence> GS_pair;

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
        for(long elm : index.query(read.substr(i, kmer_length))){
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
    cout << "last: " << cur_diagonal  << endl;
    new_mrs.push_back({cur_diagonal, start_pos, seq_pos, cur_length-1});

    cout << "New matching regions.\n";
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
        int search_limit = kmer_length;

        //Align LMUR (Left-Most Unmatched Region)
        match_region mr = cr.mr_list.front();
        GS_pair gs_pair = global_align(reference, read, pair<int, int>(mr.seq_pos - search_limit - mr.read_pos, mr.seq_pos), pair<int, int>(0, mr.read_pos));

        //Align RMUR (Right-Most Unmatched Region)
        mr = cr.mr_list.back();
        GS_pair gs_pair2 = global_align(reference, read, pair<int, int>(mr.seq_pos, mr.seq_pos + search_limit + mr.read_pos), pair<int, int>(mr.read_pos, string::npos));

        //Align all MRUR
        GS_pair gs_pair_acc = GS_pair(GappedSequence::merge(gs_pair.first, gs_pair2.first), GappedSequence::merge(gs_pair.second, gs_pair2.second));
        auto it1 = begin(cr.mr_list);
        auto it2 = begin(cr.mr_list);
        it2++;
        for(; it2 != end(cr.mr_list); it1++, it2++){
            auto seq_range = pair<int, int>(it1->seq_pos + it1->length, it2->seq_pos);
            auto read_range = pair<int, int>(it1->read_pos + it1->length, it2->read_pos);
            GS_pair gs_temp = global_align(reference, read, seq_range, read_range);
            gs_pair_acc = make_pair(GappedSequence::merge(gs_temp.first, gs_pair_acc.first), GappedSequence::merge(gs_temp.second, gs_pair_acc.second));
        }
    }


    return GS_pair(GappedSequence(read), GappedSequence(read));
}

GS_pair Aligner::global_align(const string &seq1, const string &seq2, pair<int, int> seq1_range, pair<int, int> seq2_range){

    int seq1Length = seq1.length();
    int seq2Length = seq2.length();

    int numCols = seq1Length + 1;
    int numRows = seq2Length + 1;
    int substitution = 0;

    int left, top, diagonal, num = 0;

    vector < vector<int> > matrix;
    matrix.resize(numRows, vector<int>(numCols, 0));

    // ##################################################
    // ########## Building DP Matrix ####################
    // ##################################################

    for (int row = 0; row < numRows; row++)
    {
        for (int col = 0; col < numCols; col++)
        {
            if (row == 0 and col == 0)
            {
                matrix[row][col] = 0;
            }
            else if (row == 0) {
                matrix[row][col] = matrix[row][col - 1] + 1;
            }
            else if (col == 0) {
                matrix[row][col] = matrix[row - 1][col] + 1;
            }
            else {
                if (seq1[col - 1] == seq2[row - 1])
                {
                    substitution = 0; //match score = 0
                }
                else
                {
                    substitution = 1; //mismatch score = 1
                }

                left = matrix[row][col - 1] + 1; //gap penalty: +1
                top = matrix[row - 1][col] + 1; //gap penalty: +1
                diagonal = matrix[row - 1][col - 1] + substitution; //match/mismatch

                num = min(min(left, top), diagonal);
                matrix[row][col] = num;
            }
        }
    }

    // for (int i = 0; i < numRows; i++)
    // {
    //     for (int j = 0; j < numCols; j++)
    //     {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // ##################################################
    // ########## Aligning Sequences ####################
    // ##################################################

    GappedSequence s1(seq1, "", "", "");
    GappedSequence s2(seq2, "", "", "");

    int match = 0;

    int currentCol = seq1Length;
    int currentRow = seq2Length;

    left = 0; 
    top = 0;
    diagonal = 0;
    num = 0;

    int string1Index = seq1Length;
    int string2Index = seq2Length;

    while(true)
    {

        if (currentCol == 0 and currentRow == 0)
        {
            break; //breaks when traceback gets to top left cell
        }

        if (currentRow != 0 and currentCol != 0)
        {
            left = matrix[currentRow][currentCol - 1];
            top = matrix[currentRow - 1][currentCol];
            diagonal = matrix[currentRow - 1][currentCol - 1];
        }

        num = min(min(left, top), diagonal);

        if (seq1[currentCol - 1] == seq2[currentRow - 1])
        {
            match = 1;
        }

        if ((num == left and num != diagonal and match != 1) or (currentRow == 0))
        {
            // left

            // seq2New += '-'; //add a gap
            s2.add_gaps(string2Index);
            // seq1New += seq1[currentCol - 1]; //take character
            currentCol -= 1;
            string2Index -= 1;
        }

        else if ((num == top and num != diagonal and match != 1) or (currentCol == 0))
        {
            // top
            
            // seq1New += '-'; //add a gap
            s1.add_gaps(string1Index);
            // seq2New += seq2[currentRow - 1]; //take character
            currentRow -= 1;
            string1Index -= 1;
        }
        else
        {
            // diagonal

            // seq1New += seq1[currentCol - 1]; //take character
            // seq2New += seq2[currentRow - 1]; //take character

            currentCol -= 1;
            currentRow -= 1;

            string1Index -= 1;
            string2Index -= 1;
        }
        match = 0;
    }
    return GS_pair(s1, s2);
}