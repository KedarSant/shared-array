# shared-array

This is my own small implementation of the C++ standard class shared_ptr.  
In C++ since garbage collection collection is manual, bad errors can occur (such as memory leaks).  
This class takes a dynamically allocated array (using new or malloc()) and automatically calls delete[] in the destructor. Thus the programmer doesn't have to worry about cleanup. Many pointers can have shared ownership of the object, and the last pointer which goes out of of scope will delete the pointer array and free its memory. However the only way to have a shared owbership of the object is by using the copy constructor of the ***shared_array*** class. Don't initialize multiple class objects with the same pointer, else it could result in undefined behavior.
