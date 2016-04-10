#include <iostream>

int main(){
    std::cout << "Hello World!\n";
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