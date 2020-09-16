## STL Vector Container Implementation

[Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library) vector is a container class, that can be used like a dynamic array. The elements of a [vector](https://en.cppreference.com/w/cpp/container/vector) are stored in contiguous memory.

This project is a re - implementation of the standard STL vector library of C++. It's designed to be exception safe, effcient and memory safe.

## Class Vector

This implementation of the `vector` container class relies on [SFINAE](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) based overload & placement new for intialisation of items.

#### `private` members

* `size__` total size of the buffer
* `cur_len__` current number of items in the buffer
* `mem_buff__` points to memory allocated for storing the elements of type `T`. (`T *mem_buff__`)

## Changes

* copy assign template (sfinae overload) funtion argument is made const apart from no const in example.

