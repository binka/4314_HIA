#ifndef FASTA_FILE_H
#define FASTA_FILE_H

#include <vector>
#include <string>

#include "GappedSequence.h"

class FastaFile{
    public:
        FastaFile(string file_name);
        void buildSequences(string file_name);
    private:
        string file_name;
        vector<GappedSequence> sequences;
};
 
#endif