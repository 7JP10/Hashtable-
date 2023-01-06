#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <algorithm>
using namespace std;

namespace cop4530{
  template <typename K,typename V>
  class HashTable{
    public:
      HashTable(size_t size = 101);
      ~HashTable();

      bool contains(const K & k) const;
      bool match(const std::pair<K, V> &kv) const;
      bool insert(const std::pair<K, V> & kv);
      bool insert(std::pair<K,  V> && kv);
      bool remove(const K & k);
      void clear();
      bool load(const char *filename);
      void dump() const;
      size_t size() const{return currentSize;}
      bool write_to_file(const char *filename) const;

      static const unsigned int max_prime = 1301081;
      static const unsigned int default_capacity = 101;

    private:
      int currentSize;
      vector<list<pair<K,V>>> theList;

      void makeEmpty();
      void rehash();
      size_t myhash(const K &k) const;
      unsigned long prime_below (unsigned long);
      void setPrimes(vector<unsigned long>&);
  };
  #include "hashtable.hpp"
}

#endif

