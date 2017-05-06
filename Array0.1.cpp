#include <iostream>
#include <cstdio>
#include <exception>
#include <new>
#include <algorithm>

typedef int dtype;

enum CONSTANTS                                             //! enum with constants(thx, Cap)
  {
  ARRAY_CAPACITY = 16,
  POISON_DATA = -666,
  };


class Array
  {
  public:
    Array();                                           
    explicit Array(const Array & source);                  //! copying constr.
    ~Array();
    
    const Array & operator = (const Array & source);
    dtype & operator [](size_t index);
    bool operator == (const Array & that) const;
    bool operator != (const Array & that) const;
    bool operator < (const Array & that) const;
    bool operator > (const Array & that) const;
    bool operator <= (const Array & that) const;
    bool operator >= (const Array & that) const;
    
    void fill(const dtype value);

    bool verify() const;
    void dump() const;

  private:
    static const size_t Size_ = ARRAY_CAPACITY;
    dtype Data_[Size_];
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


Array::Array()
  {
  for(size_t i = 0; i < Size_; i++)
    {
    Data_[i] = POISON_DATA;
    }
  VERIFY;
  }

Array::~Array() {}

/*
Array::Array(const Array & source)                         //! copying constr. 
  {
  for(size_t i = 0; i < Size_; i++)
    {
    Data_[i] = source.Data_[i];
    }
  VERIFY;
  }
*/

Array::Array(const Array & source)                         //! copying constr. with std::copy
  {
  std::copy(source.Data_, source.Data_ + Size_, Data_);
  VERIFY;
  }


/*
const Array & Array::operator=(const Array & source)       //! operator= with placement new
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

const Array & Array::operator=(const Array & source)       //! operator= with std::swap
  {
  VERIFY;
  if(this == &source)
    {
    return *this;
    }
  Array victim(source);
  std::swap(Data_, victim.Data_);
  VERIFY;
  return *this;
  }


bool Array::verify() const
  {
  if(!Data_)return false;
  return true;
  }

void Array::dump() const
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


dtype & Array::operator[](size_t index)
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

bool Array::operator == (const Array & that) const
  {
  VERIFY;
  for(size_t i = 0; i < Size_; ++i)
    {
    if(Data_[i] != that.Data_[i])return false;
    }
  return true;
  }

bool Array::operator != (const Array & that) const
  {
  VERIFY;
  if(!(*this == that))return true;
  else return false;
  }

void Array::fill(const dtype val)
  {
  for(size_t i = 0; i < Size_; i++)
    {
    Data_[i] = val;
    }
  }

#undef VERIFY

int main()
  {
  Array array;
  try{
    array[0] = 0;
    array[1] = 1;
    array[2] = 2;
    array[3] = 4;
    array[98] = 8;
    }
  catch(std::exception & ex)
    {
    printf("Array:ERROR:%s", ex.what());
    }
  array[12] = -2;
  Array array2;
  array.dump();
  array2.dump();
  array2 = array;
  array.dump();
  array2.dump();
  array2.fill(3);
  if(array != array2)printf("NO!\n");
  }


