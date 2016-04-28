#ifndef GAPPED_SEQUENCE_H
#define GAPPED_SEQUENCE_H

#include <string>
#include <vector>

using namespace std;

class GappedSequence{
    public:
        static GappedSequence merge(const GappedSequence &a, const GappedSequence &b);

        GappedSequence(const string &sequence, string name = "", string description = "", string gap_string = "_");
        // GappedSequence(GappedSequence &&other);
        // GappedSequence(const GappedSequence &other);
        // GappedSequence &operator=(GappedSequence &&other);
        string get_name() const{return name;}
        void set_name(string name){this->name = name;}
        string get_description() const{return description;}
        void set_description(string description){this->description = description;}
        void add_gaps(int index);
        void add_gaps(vector<int> indices);
        string get_sequence() const;
    private:
        string name;
        string description;
        const string *sequence;
        string gap_string;
        vector<int> gaps;
};
#endif