#ifndef OS_CARRAY_H_
#define OS_CARRAY_H_

#include <vector>

namespace sarab {

namespace os {

template <typename T>
class carray {
public:
  carray(int size) {

    m_size = size;
    m_arrPtr = new T[m_size];

  };
  ~carray()        { delete [] m_arrPtr;     };

  carray(const carray&)            = delete;
  carray(carray&&)                 = delete;
  carray& operator=(const carray&) = delete;
  carray& operator=(carray&&)      = delete;



  T* getPtr()      { return m_arrPtr;        };

  void copyToVector(std::vector<T>& vect) {

    for (int i = 0; i < m_size; i++)
      vect.push_back(m_arrPtr[i]);

  }

private:
  T*  m_arrPtr;
  int m_size;

};

}

}


#endif
