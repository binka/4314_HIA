#include <iostream>
#include <string>

#include "HybridIndex.h"
#include "GappedSequence.h"
#include "Aligner.h"

int main(){
    HybridIndex hy_ind(1, "TATAGGCATGAGCCAC");
    hy_ind.query("A");
    hy_ind.query("C");
    hy_ind.query("G");
    hy_ind.query("T");
    hy_ind.query("GC");

    string seq("Hello World.");
    GappedSequence gs(seq, "name", "descript", "+");
    gs.add_gaps(0);
    gs.add_gaps(vector<int> {0, 3, 2, 4, 6, 2, 12, 15, 16});
    cout << gs.get_name() << "; " << gs.get_description() << endl;
    cout << gs.get_sequence() << endl;

    Aligner aligner(2, "TATAGGCATGAGCCAC");
    aligner.find_alignment("GCCATG", 2);
    /*
    read sequence file -> FastaFile object
    read reads file -> FastaFile/FastaQ object

    //Start the timer for hybrid index creation
    start_timer()

    Aligner.set_reference(sequence)

    print_elapsed_time

    for each read:
        Aligner.find_alignment(read)
        score alignment
            Should scoring using a combination of a scoring matrix and whether or not
            the right location on the reference sequence was selected
            (to caluculate TP,FP,TN,FN) 
        optionally write alignment to file

    //Measure time to run reads through the aligner
    print_elapsed_time
    
    */
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