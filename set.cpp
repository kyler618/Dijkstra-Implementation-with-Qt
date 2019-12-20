#include "set.h"

#include "vertex.h"

Set::Set(unsigned int size)
{
    prime = (size<11)? 7:
            (size<23)? 11:
            (size<37)? 17:23;

    for(unsigned int x{1}; size%prime!=0; ++x)
        size += x;
    current_size = original_capacity = size;
    hash_table.resize(original_capacity);
}

unsigned int
Set::hash(Vertex* vertex){
    return reinterpret_cast<unsigned long>(vertex) % prime;
}

void
Set::insert(Vertex* vertex){
    unsigned int key{hash(vertex)};
    while(hash_table[key]!=nullptr)
    {
        key += prime;
        if(key>=current_size)
        {
            current_size += prime;
            hash_table.resize(current_size);
        }
    }
    hash_table[key] = vertex;

}

bool
Set::exist(Vertex* vertex, bool remove){
    unsigned int key{hash(vertex)};

    while(hash_table[key]!=vertex){
        key += prime;
        if(key>=current_size) return false;
    }

    if(remove)
    {
        hash_table[key] = nullptr;

        while( key+prime < current_size && hash_table[key+prime]!=nullptr )
        {
            hash_table[key] = hash_table[key+prime];
            key += prime;
            hash_table[key] = nullptr;
        }

        if( current_size-prime<=key && current_size>original_capacity )
        {
            // possible to reduce hashtable size
            for(unsigned int x{current_size-prime};x<current_size;++x)
                if(hash_table[x]!=nullptr) return true;
            current_size -= prime;
            hash_table.resize(current_size);
        }
    }

    return true;
}
