#ifndef ITERATORS_HPP_
#define ITERATORS_HPP_

template<typename type>
class Random_Access_Iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;


  Random_Access_Iterator(type* ptr = nullptr) : my_pointer(ptr) {}
  
  Random_Access_Iterator(const Random_Access_Iterator<type>& rawIterator) = default;


  type* getPtr() const {return this->my_pointer;}
  
  const type* getConstPtr() const {return this->my_pointer;}

  
  Random_Access_Iterator<type>& operator=(const Random_Access_Iterator<type>& rawIterator) = default;
  
  Random_Access_Iterator<type>& operator=(type* ptr) {
    this->my_pointer = ptr;
    return (*this);
  }


  operator bool() const {
    if (this->my_pointer) {return true;}
    else {return false;}
  }


  bool operator==(const Random_Access_Iterator<type>& rawIterator) const {return (this->my_pointer == rawIterator.getConstPtr());}
  
  bool operator!=(const Random_Access_Iterator<type>& rawIterator) const {return (this->my_pointer != rawIterator.getConstPtr());}
  
  Random_Access_Iterator<type>& operator++() {
    ++this->my_pointer;
    return (*this);
  }

  Random_Access_Iterator<type>& operator--() {
    --this->my_pointer;
    return (*this);
  }

  
  Random_Access_Iterator<type> operator++(int) {
    auto temp(*this);
    ++this->my_pointer;
    return temp;
  }
  
  Random_Access_Iterator<type> operator--(int) {
    auto temp(*this);
    --this->my_pointer;
    return temp;
  }

  Random_Access_Iterator<type> operator+(const difference_type& movement) {
    auto oldPtr = this->my_pointer;
    this->my_pointer += movement;
    auto temp(*this);
    this->my_pointer = oldPtr;
    return temp;
  }
  
  Random_Access_Iterator<type> operator-(const difference_type& movement) {
    auto oldPtr = this->my_pointer;
    my_pointer -= movement;
    auto temp(*this);
    this->my_pointer = oldPtr;
    return temp;
  }

  difference_type operator-(const Random_Access_Iterator<type>& rawIterator) {return std::distance(rawIterator.getPtr(),this->getPtr());}

  type& operator*() {return *this->my_pointer;}

  const type& operator*() const {return *this->my_pointer;}

  type* operator->() {return this->my_pointer;}

  ~Random_Access_Iterator() {} 
  
protected:

  type* my_pointer;
};

#endif // ITERATORS_HPP_