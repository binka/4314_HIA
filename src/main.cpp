#include <iostream>
#include <string>

#include "HybridIndex.h"
#include "GappedSequence.h"

int main(){
    HybridIndex hy_ind(1, "TATAGGCATGAGCCAC");
    hy_ind.query("A");
    hy_ind.query("C");
    hy_ind.query("G");
    hy_ind.query("T");
    hy_ind.query("GC");

    string seq("Hello World.");
    GappedSequence gs("seq1", "descript", seq, "+");
    gs.add_gaps(0);
    gs.add_gaps(vector<int> {0, 3, 4, 6, 2, 12, 15, 16});
    cout << gs.get_name() << "; " << gs.get_description() << endl;
    cout << gs.get_sequence() << endl;
}

/*
HybridIndex class:
  public:
    constructor()
    constructor(ref_str)
    destructor()
    void set_reference(str)
    <char *>vector query(kmer)

Aligner class:
  public:
    constructor()
    destructor()
    void set_reference(str)
    vector<GappedSequence> find_alignment(read_str)
    operator<<
    wrapped_print(line_len)
    to_file(name)

GappedSequence class:
  public:
    constructor()
    constructor(name, description, sequence)
    destructor()
    get_name()
    get_description()
    void add_gap(str_ind)
    void add_gaps(str_indices)
    void set_string(str)
    string get_sequence()
    bool is_gapped()
    operator[]
    operator<<
    wrapped_print(line_len)
  private:
    int lookup_index(index)

FastaFile class:
  public:
    constructor(stream)
    destructor()
    <GappedSequence>iterator iterate_sequences()

alignment functions (in a util file):
  print_alignment(vector<GappedSequence>)
  to_file(vector<GappedSequence>)
  pad_to_match(vecctor<GappedSequence>, strip = true)
*/