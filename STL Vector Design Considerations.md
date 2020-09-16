#### Memory Allocation is done using `operator new`

`operator new` is used instead of new operator because,
operator new wont call the constructor for T during allocation
and destructor for T during deallocation. 

This infact will fix a major error caused due to
multiple call to destructors on the same object, 
which can occur in different scenarios as the delete[] will also
call the destructor on all elements regardless of whether 
it was destroyed previously, say, for example via pop_back().

A possible scenario that causes above issue:

1. Consider a type T, that has a pointer to heap memory and the destructor ( ~T() ) of T will delete the heap memory as usual. 
2. If vector<T> calls pop_back it will call the destructor of T on the last element of the vector.
3. Also during the destruction of the vector, the delete[] will again call destructor of T for the same object, which was deleted in step 2. This happens for example when vector goes out of scope or destroyed, soon after a pop_back.
4. Thus delete is called twice on the same memory location causing undefined behaviour.


[more](https://stackoverflow.com/q/17344727/6792356)
