#ifndef HYBRID_INDEX_H
#define HYBRID_INDEX_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class HybridIndex{
     public:
          HybridIndex(unsigned int kmer_length, const string &sequence);
          ~HybridIndex();
          vector<int> query(const string &kmer);
     private:
          unsigned int kmer_length;
          const string sequence;
          //First int is the index, second is the number of sub-sequences
          unordered_map<string, pair<int,int>> hash_table;
          vector<int> suffix_array;
          void create_index();
};
 
#endif