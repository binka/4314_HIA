#ifndef FASTA_FILE_H
#define FASTA_FILE_H

#include <vector>
#include <string>

#include "GappedSequence.h"

class FastaFile{
    public:
        FastaFile(string file_name);
        bool build_sequences(string file_name);
        const vector<GappedSequence>& get_sequences();
    private:
        string file_name;
        vector<GappedSequence> sequences;
};
 
#endif