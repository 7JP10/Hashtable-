#include <iostream>
#include "hashtable.h"
#include "passserver.h"
#include <unistd.h>
#include <crypt.h>
using namespace std;
using namespace cop4530;

PassServer::PassServer(size_t size) : HashTable<string,string>(size){
}

PassServer::~PassServer(){
  HashTable<string,string>::clear();
}

bool PassServer::load(const char *filename){
  return HashTable<string,string>::load(filename);
}

bool PassServer::addUser(std::pair<string, string> & kv){
  pair<string, string> encryptPair(kv.first, encrypt(kv.second));
  return HashTable<string,string>::insert(encryptPair);
}

bool PassServer::addUser(std::pair<string, string> && kv){
  pair<string, string> encryptPair(move(kv));
  return addUser(encryptPair);
}

bool PassServer::removeUser(const string & k){
  return HashTable<string,string>::remove(k);
}

bool PassServer::changePassword(const pair<string, string> &p, const string & newpassword){
  if(!(HashTable<string,string>::contains(p.first))){
    return false;
  } else if(p.second == newpassword){
    return false;
  } else{
    string oldPword = encrypt(p.second);
		string newPword = encrypt(newpassword);
    pair<string, string> oldKV(p.first, oldPword);

		if(!HashTable<string,string>::match(oldKV)){
      return false;
    } else{
			pair<string, string> newKV(p.first, newPword);
			HashTable<string,string>::insert(newKV);
			return true;
		}
  }
}

bool PassServer::find(const string & user) const{
  return HashTable<string,string>::contains(user);
}

void PassServer::dump(){
  HashTable<string,string>::dump();
}

size_t PassServer::size() const{
  return HashTable<string,string>::size();
}

bool PassServer::write_to_file(const char *filename) const{
  return HashTable<string,string>::write_to_file(filename);
}

string PassServer::encrypt(const string & str){
  char salt[] = "$1$########";
	string ePwd = crypt(str.c_str(), salt);

	ePwd.replace(0, 12, "");
	return ePwd;
}


