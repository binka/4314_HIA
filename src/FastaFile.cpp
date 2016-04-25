#include <iostream>
#include <string>

#include "FastaFile.h"
#include "GappedSequence.h"
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

FastaFile::FastaFile(string file)
{
	fileName = file;
	genome = buildGenome(fileName);
}

vector<GappedSequence> FastaFile::buildGenome(string fileName)
{
	cout << "I'm building some shiz\n";

	vector<GappedSequence> genome;

	string line;
	string name;
	string seq;
	string desc;


	ifstream myfile(fileName); 
	if( myfile.is_open())
	{
		cout << "file is open\n";
		while( getline (myfile,line) )
    	{
      		cout << line << '\n';
      		// regex e (">(\\S+)\\s+(.*)");
      		regex e (">(.*)");
      		smatch sm;
      		//find where a sequence begins
      		if(regex_search (line,sm,e))
      		{
      			cout << "Regex: " << sm[1] << "\n";
   
      		

      			if(genome.size() ==0)
      			{
      				name = sm[0];
      				desc = sm[1];
      			}
      			else
      			{   
      				GappedSequence temp(seq,name,desc);
    				genome.push_back(temp);
      				name = sm[0];
      				desc = sm[1];
      				seq = "";
      			}	
      				
      		}
      		// we're in the middle of a sequence so just append
      		else
      		{
      			for(unsigned int i =0;i<line.length();i++)
      			{
      				if(line[i] !=('\n') && line[i]!='\t')
      				{
      					seq += line[i];
      				}
      			}
      		}

    	}
    	myfile.close();

    	GappedSequence temp(seq,name);
    	genome.push_back(temp);
	}

	return genome;
}
