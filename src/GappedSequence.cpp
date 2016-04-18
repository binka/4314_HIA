#include <iostream>

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>


#include "GappedSequence.h"

using namespace std;

GappedSequence::GappedSequence(string name, string description, const string &sequence, string gap_string):
    name(name),
    description(description),
    sequence(sequence),
    gap_string(gap_string){}

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
string GappedSequence::get_sequence(){
    stringstream ss;
    auto it = begin(gaps);

    while(it != end(gaps) && *it <= 0){
        ss << gap_string;
        it++;
    }

    for(unsigned int i = 0; i < sequence.size(); i++){
        while(it != end(gaps) && *it == static_cast<int>(i)){
            ss << gap_string;
            it++;
        }
        ss << sequence[i];
    }

    while(it != end(gaps)){
        ss << gap_string;
        it++;
    }

    return ss.str();
}