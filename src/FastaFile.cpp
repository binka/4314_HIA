#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include "FastaFile.h"
#include "GappedSequence.h"

using namespace std;

FastaFile::FastaFile(string file_name): file_name(file_name){
    buildSequences(file_name);
}

void FastaFile::buildSequences(string file_name){
    string line;
    string name;
    string seq;
    string desc;

    sequences.clear();

    ifstream myfile(file_name); 
    if(!myfile.is_open()){
        return;
    }

    cout << "file is open\n";
    while(getline (myfile,line)){
        cout << line << '\n';
        // regex e (">(\\S+)\\s+(.*)");
        regex e (">(.*)");
        smatch sm;
        //find where a sequence begins
        if(regex_match(line, sm, e)){
            if(sequences.size() ==0){
                name = sm[0];
                desc = sm[1];
            }else{
                GappedSequence temp(seq,name,desc);
                sequences.push_back(temp);
                name = sm[0];
                desc = sm[1];
                seq = "";
            }
        }else{
            // we're in the middle of a sequence so just append
            for(unsigned int i =0;i<line.length();i++){
                if(line[i] !=('\n') && line[i]!='\t'){
                    seq += line[i];
                }
            }
        }

        myfile.close();

        GappedSequence temp(seq,name);
        sequences.push_back(temp);
    }
}
