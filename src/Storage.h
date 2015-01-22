#pragma once
#include <String>
#include <hash_map>

class Base_storage {
protected:
  static std::vector<Base_storage*> warehouses;
  virtual void _Shutdown(){};

public:
  static void Shutdown();
};

std::vector<Base_storage*> Base_storage::warehouses;

void Base_storage::Shutdown() {
  for (auto b : warehouses) {
    b->_Shutdown();
  }
}

template <class T> class Storage : public Base_storage {
private:
  static std::hash_map<std::string, T*> _container;
  static bool constructed;

public:
  Storage() {}

  static T* Get(const std::string& name) {
    if (!constructed) {
      warehouses.push_back(new Storage<T>);
      constructed = true;
    }
    printf("Lookign for: %s ... ",name.c_str());
    std::hash_map<std::string, T*>::iterator got = _container.find(name);
    if (got == _container.end()) {
      // Not loaded
      printf("Not found, Loading \n");
      T* item = T::Load(name);
      _container.insert(std::make_pair(name, item));
      return item;
    }
    else {
      printf("In warehouse\n");
      return got->second;
    }

    return NULL;
  }

  void _Shutdown() { _container.clear(); };
};

template <class T> std::hash_map<std::string, T*> Storage<T>::_container;
template <class T> bool Storage<T>::constructed = false;