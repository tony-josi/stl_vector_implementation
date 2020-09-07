## Class Vector

This implementation of the `vector` container class will rely [SFINAE](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) & placement new for intialisation of items.

#### `public` members

* All `std::vector` public members

#### `private` members

* `v_buff` points to memory allocated for storing the elements of type `T`. (`T *v_buff`)
* `length` current number of items in the buffer
* `size` total size of the buffer

