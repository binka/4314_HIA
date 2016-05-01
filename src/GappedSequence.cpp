#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "GappedSequence.h"

using namespace std;

GappedSequence::GappedSequence(const string &sequence, string name, string description, string gap_string):
    name(name),
    description(description),
    sequence(&sequence),
    gap_string(gap_string){}

// GappedSequence::GappedSequence(const GappedSequence &other):
//     name(other.name.substr()),
//     description(other.description.substr()),
//     sequence(other.sequence),
//     gap_string(other.gap_string.substr()),
//     gaps(other.gaps){}

// GappedSequence::GappedSequence(GappedSequence &&other):
//     name(other.name),
//     description(other.description),
//     sequence(other.sequence),
//     gap_string(other.gap_string),
//     gaps(other.gaps){}

// GappedSequence &GappedSequence::operator=(GappedSequence &&other){
//     name = other.name;
//     description = other.description;
//     sequence = other.sequence;
//     gap_string = other.gap_string;
//     gaps = other.gaps;
//     return *this;
// }

void GappedSequence::add_gaps(int index){
    gaps.push_back(index);
    sort(begin(gaps), end(gaps));
}

void GappedSequence::add_gaps(vector<int> indices){
    gaps.insert(end(gaps), begin(indices), end(indices));
    sort(begin(gaps), end(gaps));
}

/*
 * Builds sequence using the gap characters.
 */
string GappedSequence::get_sequence() const{
    stringstream ss;
    auto it = begin(gaps);

    while(it != end(gaps) && *it <= 0){
        ss << gap_string;
        it++;
    }

    for(unsigned int i = 0; i < sequence->size(); i++){
        while(it != end(gaps) && *it == static_cast<int>(i)){
            ss << gap_string;
            it++;
        }
        ss << (*sequence)[i];
    }

    while(it != end(gaps)){
        ss << gap_string;
        it++;
    }

    return ss.str();
}

/*
 * Merges two Gapped sequences iff name and description match by string comparision, and sequences point to the same location.
 * Throws std::invalid_argument error if this condition is broken.
 * Merging consists of merging the list of gaps, copying name, description, and the reference to the sequence.
 */
GappedSequence GappedSequence::merge(const GappedSequence &a, const GappedSequence &b){
    if(a.name.compare(b.name) != 0){
        throw invalid_argument("Cannot merge GappedSequences with different names.");
    }else if(a.description.compare(b.description) != 0){
        throw invalid_argument("Cannot merge GappedSequences with different descriptions.");
    }else if(a.sequence != b.sequence){

        throw invalid_argument("Cannot merge GappedSequences with different sequences.");
    }
    GappedSequence temp(*a.sequence, a.name, a.description);
    temp.add_gaps(a.gaps);
    temp.add_gaps(b.gaps);
    return temp;
}