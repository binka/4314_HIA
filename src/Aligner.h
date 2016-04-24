#ifndef ALIGNER_H
#define ALIGNER_H

#include "GappedSequence.h"
#include "HybridIndex.h"

using namespace std;

class Aligner{
    public:
        Aligner(int kmer_length, const string &reference);
        pair<GappedSequence, GappedSequence> find_alignment(const string &read);
        pair<GappedSequence, GappedSequence> find_alignment(const string &read, int kmer_length);
    private:
        int kmer_length;
        string reference;
        HybridIndex index;
};
 
#endif