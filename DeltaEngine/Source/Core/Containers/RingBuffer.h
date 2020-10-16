#pragma once
#include <memory>
#include <cassert>
#include <iostream>

template <typename T>
class RingBuffer
{

public:
  using Size_u32 = uint32_t;
  using Index = uint32_t;

  /***********************************************************************/
  /*!
  \fn RingBuffer(Size_u32 size)

  \brief Constructor for RIng Buffer

  */
  /***********************************************************************/
  explicit RingBuffer( Size_u32 size ) :
    head { 0 },
    tail { 0 },
    max_size { UpperPowerOfTwo( size ) },
    mask_val { max_size - 1 },
    array { std::make_unique<T[]>( max_size ) }
  {
    assert( size > 1 && "size must be greater than 1" );
  }

  /***********************************************************************/
  /*!
  \fn ~RingBuffer()

  \brief Destructor for Ring Buffer

  */
  /***********************************************************************/
  ~RingBuffer()
  {
    array.reset();
  }

  /***********************************************************************/
  /*!
  \fn Size_u32 size()

  \brief Returns the current size of the Ring Buffer
  */
  /***********************************************************************/
  Size_u32 Size()
  {
    return mask( tail - head );
  }

  /***********************************************************************/
  /*!
  \fn bool full()

  \brief Check if the Ring Buffer is full
  */
  /***********************************************************************/
  bool Full()
  {
    return inc( tail ) == head;
  }

  /***********************************************************************/
  /*!
  \fn bool empty()

  \brief Check if the Ring Buffer is empty
  */
  /***********************************************************************/
  bool Empty() const
  {
    return head == tail;
  }

  /***********************************************************************/
  /*!
  \fn bool write(T& data)

  \brief Writes data to the ring buffer. Data is written to the tail.
  */
  /***********************************************************************/
  bool Write( T data )
  {
    if ( !Full() )
    {
      array[tail] = data;
      tail = inc( tail );
      return true;
    }
    else
    {
      return false;
    }
  }

  /***********************************************************************/
  /*!
  \fn bool write(T&& data)

  \brief Writes data to the ring buffer. Data is written to the tail.
  */
  /***********************************************************************/
  bool Write( T &&data )
  {
    if ( !Full() )
    {
      array[tail] = data;
      tail = inc( tail );
      return true;
    }
    else
    {
      return false;
    }
  }

  /***********************************************************************/
  /*!
  \fn T& read()

  \brief Reads data from the ring buffer. Data is read from the head.
  */
  /***********************************************************************/
  T &Read()
  {
    if ( !Empty() )
    {
      T &temp = array[head];
      head = inc( head );
      return temp;
    }
    else
    {
      assert( "array is empty!" );
      return array[0];
    }
  }

  /***********************************************************************/
  /*!
  \fn void printdetails()

  \brief Gives detail about the Ring Buffer
  */
  /***********************************************************************/
  void Printdetails()
  {
    std::cout << std::endl;
    std::cout << "Current size is " << Size() << std::endl;
    std::cout << "Max size is " << max_size << std::endl;
    std::cout << "Current head index is " << head << std::endl;
    std::cout << "Current tail index is " << tail << std::endl;
    std::cout << "Mask value is " << mask_val << std::endl;
    for ( Size_u32 i = 0; i < max_size; i++ )
    {
      std::cout << "ring[" << i << "] is: " << array[i] << std::endl;
    }
  }

  /***********************************************************************/
  /*!
  \fn void clear()

  \brief Clears the Ring Buffer, resetting its head and tail.
  */
  /***********************************************************************/
  void Clear()
  {
    head = 0;
    tail = 0;
  }

  /***********************************************************************/
  /*!
  \fn Size getMaxSize() const

  \brief Gets the maximum size the Ring Buffer can hold
  */
  /***********************************************************************/
  inline Size_u32 GetMaxSize() const
  {
    return max_size;
  }

/***********************************************************************/
/*!
\fn T operator[](Index index) const

\brief Gets a specific index of the ring buffer
*/
/***********************************************************************/
  T operator[]( Index index ) const
  {
    if ( index <= max_size )
    {
      return array[head + index - 1];
    }
    else
    {
      assert( index <= max_size && "Index out of bound" );
    }

    return array[0];
  }

  /***********************************************************************/
  /*!
  \fn T& operator[](Index index) const

  \brief Gets a specific index of the ring buffer
  */
  /***********************************************************************/
  T &operator[]( Index index )
  {
    if ( index <= max_size )
    {
      return array[head + index - 1];
    }
    else
    {
      assert( index <= max_size && "Index out of bound" );
    }

    return array[0];
  }

private:
  Index head;
  Index tail;
  Size_u32 max_size;
  Index mask_val;
  std::unique_ptr<T[]> array;

  /***********************************************************************/
  /*!
  \fn Index upper_power_of_two(Index num)

  \brief Index would round up the value to a power of 2.
  */
  /***********************************************************************/
  Index UpperPowerOfTwo( Index num )
  {
    num--;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num++;
    return num;
  }

  /***********************************************************************/
  /*!
  \fn inline Index mask(Index val)  { return val & mask_val; }

  \brief
  */
  /***********************************************************************/
  inline Index mask( Index val )
  {
    return val & mask_val;
  }

/***********************************************************************/
/*!
\fn inline Index inc(Index index) { return mask(index + 1); }

\brief
*/
/***********************************************************************/
  inline Index inc( Index index )
  {
    return mask( index + 1 );
  }

};