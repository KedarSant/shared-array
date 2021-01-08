
#pragma once

#include <type_traits>

namespace std {

    auto del_arr = [](auto var) -> void {
        delete[] var;
    };

    struct reference {
        int count;
        reference() : count(0) {}
        void addref() { ++count; }
        void release() { --count; }
    };

    template <class type,class deleter = decltype(del_arr)>
    class shared_array {

        type *ptr;
        deleter *del;
        reference *ref;

        void SafeRelease(type* ptr) {
            if(ptr != nullptr) 
                (*del)(ptr);
        }

        void SafeRelease(reference* ref) {
            if(ref != nullptr)
                delete ref;
        }

        void init_deleter(deleter dl, std::false_type) { deleter = dl; }

        void init_deleter(deleter dl, std::true_type) { del = &dl; }

        public :

        explicit shared_array(type *p) : ptr{p}, ref{new reference} { 
            ref->addref();  
        }

        shared_array(type* p, deleter dl) : ptr{p} , ref{ new reference } {
            ref->addref();
            init_deleter(dl, std::is_class<deleter>());
        }

        int use_count() {
            return ref->count;
        }

        shared_array(const shared_array<type>& arr) : ptr{arr.ptr}, ref{arr.ref} {
            ref->addref();
        }

        shared_array<type>& operator = (const shared_array<type>& arr) {
            SafeRelease(ptr);
            SafeRelease(ref);
            ptr = arr.ptr;
            ref = arr.ref;
            ref->addref();
            return *this;
        }

        shared_array)shared_array<type> && arr) : ptr{arr.ptr} , ref{arr.ref} {
            arr.ptr = nullptr;
            arr.ref = nullptr;
        }

        shared_array<type>& operator = (shared_array<type>&& arr) {
            SafeRelease(ptr);
            SafeRelease(ref);
            ptr = arr.ptr;
            ref = arr.ref;
            arr.ptr = nullptr;
            arr.ref = nullptr;
            return *this;
        }

        type& operator[] (size_t idx) {
            return ptr[idx];
        }

        ~shared_array() {
            if(ref->count == 1)
                (*del)(ptr);
            else
                ref->release();
        }
    };
}