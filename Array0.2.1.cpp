#include <iostream>
#include <cstdio>
#include <exception>
#include <new>
#include <algorithm>


enum CONSTANTS                                             //! enum with constants(thx, Cap)
  {
  POISON_DATA = -666,
  };

template <typename T, size_t size>
class Array
  {
  public:
    Array();                                           
    explicit Array(const Array & source);                  //! copying constr.
    ~Array();
    
    const Array & operator = (const Array & source);
    T & operator [](size_t index);
    bool operator == (const Array & that) const;
    bool operator != (const Array & that) const;
    bool operator < (const Array & that) const;
    bool operator > (const Array & that) const;
    bool operator <= (const Array & that) const;
    bool operator >= (const Array & that) const;
    
    void fill(const T value);

    bool verify() const;
    void dump() const;

  private:
    static const size_t Size_ = size;
    T Data_[Size_];
  };


class ArrayBadIndex: public std::exception                 //! that exception will be thrown
  {                                                        //! if we refer in wrong index
  virtual const char* what() const throw()
    {
    return "Index is out of range";
    }
  };
class ArrayNoData: public std::exception                   //! will be thrown in case of
  {                                                        //! Data_ == NULL
  virtual const char* what() const throw()
    {
    return "Data lost";
    }
  };


#define VERIFY \
  if(!verify()) \
    { \
    ArrayNoData nodata; \
    throw nodata; \
    }

template <typename T, size_t size>
Array<T, size>::Array()
  {
  for(size_t i = 0; i < Size_; i++)
    {
    Data_[i] = POISON_DATA;
    }
  VERIFY;
  }

template <typename T, size_t size>
Array<T, size>::~Array() {}

/*
template <typename T, size_t size>
Array<T, size>::Array(const Array & source)                //! copying constr. 
  {
  for(size_t i = 0; i < Size_; i++)
    {
    Data_[i] = source.Data_[i];
    }
  VERIFY;
  }
*/

template <typename T, size_t size>
Array<T, size>::Array(const Array & source)                //! copying constr. with std::copy
  {
  std::copy(source.Data_, source.Data_ + Size_, Data_);
  VERIFY;
  }



/*
template <typename T, size_t size>
const Array<T, size> & Array<T, size>::operator=(const Array & source) //! operator= with placement new
  {
  VERIFY;
  if(this == &source)
    {
    return *this;
    }
  this->~Array();
  new(this)Array(source);
  VERIFY;
  return *this;
  }
*/

template <typename T, size_t size>
const Array<T, size> & Array<T, size>::operator=(const Array & source) //! operator= with std::swap
  {
  VERIFY;
  if(this == &source)
    {
    return *this;
    }
  Array<T, size> victim(source);
  std::swap(Data_, victim.Data_);
  VERIFY;
  return *this;
  }


template <typename T, size_t size>
bool Array<T, size>::verify() const
  {
  if(!Data_)return false;
  return true;
  }

template <typename T, size_t size>
void Array<T, size>::dump() const
  {
  printf("\n====\nArray:dump has been called\n");
  VERIFY;
  printf("Size_ = %zu\n", Size_);
  printf("Data_ = %p\n", Data_);
  for(size_t i = 0; i < Size_; i++)
    {
    std::cout << '[' << i << ']' << Data_ + i << " : " << Data_[i];
    if(Data_[i] == POISON_DATA) std::cout << "  //poison data. Cell is empty or broken";
    std::cout << '\n';
    }
  printf("====\n");
  }

template <typename T, size_t size>
T & Array<T, size>::operator[](size_t index)
  {
  VERIFY;
  if(index >= Size_)
    {
    ArrayBadIndex badindex;
    throw badindex;
    return Data_[0];
    }
  return Data_[index];
  }

template <typename T, size_t size>
bool Array<T, size>::operator == (const Array & that) const
  {
  VERIFY;
  for(size_t i = 0; i < Size_; ++i)
    {
    if(Data_[i] != that.Data_[i])return false;
    }
  return true;
  }

template <typename T, size_t size>
bool Array<T, size>::operator != (const Array & that) const
  {
  VERIFY;
  if(!(*this == that))return true;
  else return false;
  }

template <typename T, size_t size>
void Array<T, size>::fill(const T val)
  {
  for(size_t i = 0; i < Size_; i++)
    {
    Data_[i] = val;
    }
  }
// Array<bool, size> ....


#undef VERIFY

int main()
  {
  Array<int, 10> array;
  array.fill(42);
  Array<int, 10> array2;
  array2 = array;
  array2.dump();
  }


