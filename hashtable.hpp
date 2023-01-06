template <typename K, typename V>
HashTable<K,V>::HashTable(size_t size){
    theList.resize(prime_below(size));
    currentSize = 0;
}

template <typename K, typename V>
HashTable<K,V>::~HashTable(){
  clear();  
}

template <typename K, typename V>
bool HashTable<K,V>::contains(const K & k) const{
  auto & whichList = theList[myhash(k)];

  if(currentSize == 0){
    return false;
  }
    
  for(auto & x : whichList){
    if(x.first == k) return true;  
  }
  
  return false;
}

template <typename K, typename V>
bool HashTable<K,V>::match(const std::pair<K, V> &kv) const{
  auto & whichList = theList[myhash(kv.first)];

  if(whichList.size() == 0) return false;
  
  for(auto & x : whichList){
    if((x.first == kv.first) && (x.second == kv.second)) return true;
  }
  return false;
  
}

template <typename K, typename V>
bool HashTable<K,V>::insert(const std::pair<K,V> &kv){
  auto & whichList = theList[myhash(kv.first)];

  if(whichList.size() == 0){
    whichList.push_back(kv);
  } else{
    for(auto & x : whichList){
      if(x.first == kv.first){
        if(x.second == kv.second){
          return false;
        } else{
          x.second = kv.second; 
          break;
        }
      } else{
        whichList.push_back(kv);
        break;
      }
    }
  }

  if(++currentSize > theList.size()) rehash();
  return true;
}

template <typename K, typename V>
bool HashTable<K,V>::insert(std::pair<K,V> &&kv){
  auto & whichList = theList[myhash(kv.first)];

  if(whichList.size() == 0){
    whichList.push_back(kv);
  } else{
    for(auto & x : whichList){
      if(x.first == kv.first){
        if(x.second == kv.second){
          return false;
        }else{ 
          x.second = kv.second; 
          break;
        }
      }else{ 
        whichList.push_back(move(kv));
        break;
      }
    }
  }

  if(++currentSize > theList.size()) rehash();
  return true;
}

template <typename K, typename V>
bool HashTable<K,V>::remove(const K &k){
  auto & whichList = theList[myhash(k)];
  int count = 0, count2 = 0;
  
  for(auto & x : whichList){
    if(x.first == k){
      for(auto itr = whichList.begin(); itr != whichList.end(); itr++){
        if(count2 == count){ 
          whichList.erase(itr);
          --currentSize;  
          return true;
        }
        count2++;
      }
    }
    count++;
  }
  return false;
}

template <typename K, typename V>
void HashTable<K,V>::clear(){
  makeEmpty();
}

template <typename K, typename V>
bool HashTable<K,V>::load(const char *filename){
  ifstream inFile;
  inFile.open(filename);

  if(!inFile) return false;
  
  while(inFile){
    pair<K,V> p1;
    string line, user, pwd;
    getline(inFile, line);
    stringstream stringStream(line);

    stringStream >> user;
    stringStream >> pwd;
    
    p1.first = user;
    p1.second = pwd;

    insert(p1);
  }

  inFile.close();
  return true;
}

template <typename K, typename V>
void HashTable<K,V>::dump() const{
  if(currentSize > 0){
    int count = 0;
    
    for(auto & whichList : theList){
      cout << "v[" << count++ << "]: ";
      int count2 = 0;
      for(auto itr = whichList.begin(); itr != whichList.end(); ++itr){
        if(++count2 > 1) cout << ":";
        cout << itr->first << ' ' << itr->second;
      }
      cout << endl;
    }
  }
  
}

template <typename K, typename V>
bool HashTable<K,V>::write_to_file(const char *filename) const{
  ofstream outFile;
  outFile.open(filename);

  if(!outFile) return false;

  for(auto & whichList : theList){
    for(auto & i : whichList){
      outFile << i.first << " " << i.second << "\n";
    }
  }

  outFile.close();
  return true;
}

template <typename K, typename V>
void HashTable<K,V>::makeEmpty(){
  if(currentSize > 0){
    for(auto & whichList : theList){
      whichList.clear();
    }
    currentSize = 0;
  }
}

template <typename K, typename V>
void HashTable<K,V>::rehash(){
  vector<list<pair<K,V>>> oldLists = theList;
  oldLists.resize(prime_below(2 * theList.size()));
  
  for(auto & whichList : theList){
    whichList.clear();
  }

  currentSize = 0;
  for(auto & whichList : oldLists){
    for(auto & x : whichList){
      insert(move(x));
    }
  }

}

template <typename K, typename V>
size_t HashTable<K,V>::myhash(const K &k) const{
  static hash<K> hf;
  return hf(k) % theList.size();
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

