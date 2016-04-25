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
        //cout << line << '\n';
        
        if(line[0] != '>')
        {
            cout << "I'm in the matrix\n";
            for(unsigned int i=0;i<line.length();i++)
            {
                //construct seq, ignore newlines and tabs
                if(line[i] != '\n' && line[i] != '\t')
                    seq += line[i];
            }
        }
        else    // we've hit the > character, start parsing seqname
        {
            cout << "No but actually \n";
            if(sequences.size() !=0)
            {
                GappedSequence temp(seq,name);
                sequences.push_back(temp); 
                cout << "Just appended name: " << name << " \n" << " its sequence is: \n" << seq; 
            }   
            
            seq = "";
            for(unsigned int i=1;i<line.length();i++)
            {
                if(line[i] != '\n' && line[i] != '\t')
                    name += line[i];
            }
            cout << "Heres the new name: " << name << "\n";
            
        }
    }
    myfile.close();
}
