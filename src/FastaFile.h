#ifndef FASTA_FILE_H
#define FASTA_FILE_H

#include "GappedSequence.h"
#include <vector>
#include <string>

class FastaFile{
    
    public:
    	string fileName;
    	vector<GappedSequence> genome;
        FastaFile(string file);
    private:
    	vector<GappedSequence> buildGenome(string fileName);
};
 
#endif