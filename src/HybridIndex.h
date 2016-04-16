#ifndef HYBRID_INDEX_H
#define HYBRID_INDEX_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class HybridIndex{
     public:
          HybridIndex(unsigned int kmer_length);
          HybridIndex(unsigned int kmer_length, string &sequence);
          ~HybridIndex();
          void set_reference(string &sequence);
     private:
          unsigned int kmer_length;
          string sequence;
          unordered_map<string, int> hash_table;
          vector<int> suffix_array;
          void create_index();
};
 
#endif