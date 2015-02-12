#pragma once
#include "Utilities.h"
#include <String>
#include <hash_map>

class Base_storage {
protected:
  static std::vector<Base_storage*> warehouses;
  virtual void _Shutdown(){};

public:
  static void Shutdown();
};




template <class T> class Storage : public Base_storage {
private:
  static std::hash_map<std::string, T*> _container;
  static bool constructed;

public:
  Storage() {}
  //! Manually Store an item, return false if key already stored.
  static bool Store(const std::string& name,T* item) {
    if (!constructed) {
      warehouses.push_back(new Storage<T>);
      constructed = true;
    }
    std::pair<std::hash_map<std::string, T*>::iterator, bool > pr;
    pr = _container.insert(std::make_pair(name, item));
    return pr.second;
  }

  //! Checks to see if key is already stored
  static bool Contains(const std::string& name){
    if (!constructed) {
      warehouses.push_back(new Storage<T>);
      constructed = true;
    }
    std::hash_map<std::string, T*>::iterator got = _container.find(name);
    if (got == _container.end()) {
      return false;
    }else {
      return true;
    }
  }

  static T* Get(const std::string& name) {
    if (!constructed) {
      warehouses.push_back(new Storage<T>);
      constructed = true;
    }
    //printf("Lookign for: %s ... ", name.c_str());
    std::hash_map<std::string, T*>::iterator got = _container.find(name);
    if (got == _container.end()) {
      // Not loaded
     // printf("Not found, Loading \n");
      T* item = T::Load(name);
      _container.insert(std::make_pair(name, item));
      return item;
    } else {
      //printf("In warehouse\n");
      return got->second;
    }

    return NULL;
  }

  void _Shutdown() {
    std::hash_map<std::string, T*>::iterator itt = _container.begin();
    while (itt != _container.end()) {
      delete itt->second;
      ++itt;
    }
    _container.clear();
  };
};

template <class T> std::hash_map<std::string, T*> Storage<T>::_container;
template <class T> bool Storage<T>::constructed = false;
