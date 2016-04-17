#ifndef GAPPED_SEQUENCE_H
#define GAPPED_SEQUENCE_H

#include <string>
#include <vector>

using namespace std;

class GappedSequence{
    public:
        GappedSequence(string name, string description, string &sequence, string gap_string = "_");
        string get_name(){return name;}
        string get_description(){return description;}
        void add_gaps(int index);
        void add_gaps(vector<int> indices);
        string get_sequence();
    private:
        string name;
        string description;
        string sequence;
        string gap_string;
        vector<int> gaps;
};
 
#endif