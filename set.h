#ifndef SET_H
#define SET_H

#include <vector>

class Vertex;

class Set
{
public:
    Set(unsigned int);
    void insert(Vertex*);
    bool exist(Vertex*, bool = false);
private:
    unsigned int hash(Vertex*);
    unsigned int prime, original_capacity, current_size;
    std::vector<Vertex*> hash_table;
};

#endif // SET_H
