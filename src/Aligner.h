#ifndef ALIGNER_H
#define ALIGNER_H

#include "GappedSequence.h"
#include "HybridIndex.h"

using namespace std;

class Aligner{
    public:
        Aligner(unsigned int kmer_length, const string &reference);
        pair<GappedSequence, GappedSequence> find_alignment(const string &read);
        pair<GappedSequence, GappedSequence> find_alignment(const string &read, unsigned int kmer_length);
    private:
        unsigned int kmer_length;
        string reference;
        HybridIndex index;
        pair<GappedSequence,GappedSequence> global_align(const string &seq1, const string &seq2, pair<int, int> seq1_range, pair<int, int> seq2_range);
};
 
#endif