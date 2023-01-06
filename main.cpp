#include <iostream>
#include <string>
#include "passserver.h"
using namespace std;
using namespace cop4530;

void Menu()
{
  cout << "\n\n";
  cout << "l - Load From File" << endl;
  cout << "a - Add User" << endl;
  cout << "r - Remove User" << endl;
  cout << "c - Change User Password" << endl;
  cout << "f - Find User" << endl;
  cout << "d - Dump HashTable" << endl;
  cout << "s - HashTable Size" << endl;
  cout << "w - Write to Password File" << endl;
  cout << "x - Exit program" << endl;
  cout << "\nEnter choice : ";
}

int main() {
  char choice = ' ';
  int capacity = 0;

  cout << "Enter preferred hash table capacity (integer): ";
  cin >> capacity; 
  
  if(capacity < 2){
    if(cin.fail() || cin.peek() == '\n'){
      cin.clear();
      cin.ignore(100,'\n'); 
    }
    capacity = 101;
    cout << "** input too small\nset to default capacity\n"
         << capacity << endl;
  } else{
    cout << capacity << endl;
  }

  PassServer pServer1(capacity);
  
  do{
    Menu();
  
    cin >> choice; 
    cout << "choice: " << choice << endl;
    
    switch(choice){
      case 'l':{
        string fName = "";
        cout << "Enter password file name to load from: ";
        cin >> fName;
        if(pServer1.load(fName.c_str()) == false){
          cout << "Error: Cannot open file " << fName << endl;
        }
        break;
      }
      case 'a':{
        string user = "", pwd = "";
        cout << "Enter username: ";
        cin >> user;
        cout << endl;
        cout << "Enter password: ";
        cin >> pwd;
        cout << endl;
        pair<string,string> newUser;
        newUser.first = user;
        newUser.second = pwd;
        if(pServer1.addUser(newUser)){
          cout << "\nUser " << newUser.first << " added." << endl;
        } else{
          cout << "Error: Failed to add " << newUser.first << ".";
        }
        break;
      }
      case 'r':{
        string user = "";
        cout << "Enter username: ";
        cin >> user;
        cout << endl;
        if(pServer1.removeUser(user)){
          cout << "User: " << user << " deleted." << endl;
        }else{
          cout << "Error: Failed to remove " << user << ".";
        }
        break;
      }
      case 'c':{
        string user = "", pwd = "", newPwd = "";
        pair<string,string> newUser;
        cout << "Enter username: ";
        cin >> user;
        cout << endl;
        cout << "Enter password: ";
        cin >> pwd;
        cout << endl;
        cout << "Enter new password: ";
        cin >> newPwd;
        cout << endl;
        newUser.first = user;
        newUser.second = pwd;
        if(pServer1.changePassword(newUser,newPwd)){
          cout << "Password changed for user " << user << endl;
        }else{
          cout << "*****Error: Could not change user password" << endl;
        }
        break;
      }
      case 'f':{
        string user = "";
        cout << "Enter username: ";
        cin >> user;
        cout << endl;
        if(pServer1.find(user)){
          cout << "User '" << user << "' found." << endl;
        } else{
          cout << "User '" << user << "' not found." << endl;
        }
        break;
      }
      case 'd':{
        pServer1.dump();
        break;
      }
      case 's':{
        cout << "Size of hashtable: " << pServer1.size() << endl;
        break;
      }
      case 'w':{
        string fName = "";
        cout << "Enter password file name to write to: ";
        cin >> fName;
        cout << endl;
        if(pServer1.write_to_file(fName.c_str()) == false){
          cout << "Error: Cannot write file " << fName << std::endl;
        }
        break;
      }
      case 'x':
        break;
      default: 
        if(cin.fail() || cin.peek() != '\n'){
          cin.clear();
          cin.ignore(100,'\n'); 
        }
        cout << "\n*****Error: Invalid entry.  Try again." << endl;
    }
  }while(choice != 'x');
}
