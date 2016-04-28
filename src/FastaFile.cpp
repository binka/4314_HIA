#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

#include "FastaFile.h"
#include "GappedSequence.h"

using namespace std;

FastaFile::FastaFile(string file_name): file_name(file_name){
    build_sequences(file_name);
}

bool FastaFile::build_sequences(string file_name){
    string line;
    string name;
    string description;
    string seq;

    sequences.clear();

    ifstream myfile(file_name); 
    if(!myfile.is_open()){
        return false;
    }

    cout << "file is open\n";

    while(getline (myfile,line)){
        //cout << line << '\n';
        
        if(line[0] != '>'){
            //construct seq, ignore special characters
            for(unsigned int i = 0; i < line.length(); i++){
                if(line[i] != '\r' && line[i] != '\t'){
                    seq += line[i];
                }
            }
        }else{
            // we've hit the > character, start parsing new sequence
            cout << "Starting new sequence\n";
            if(seq.length() != 0){
                GappedSequence temp(seq, name, description);
                sequences.push_back(temp); 
                cout << "Just appended name: " << name << "\nIts sequence is: " << seq << "\n"; 
            }

            seq = string();

            int stop = min(line.find(' '), line.size() - 1);
            name = line.substr(1, stop - 1);
            description = line.substr(stop + 1);
            cout << "Heres the new name: " << name << "\nDescription: " << description << "\n";
        }
    }

    GappedSequence temp(seq, name, description);
    sequences.push_back(temp);
    cout << "Just appended name: " << name << "\nIts sequence is: " << seq << "\n"; 
    myfile.close();
    return true;
}

const vector<GappedSequence>& FastaFile::get_sequences(){
    return sequences;
}