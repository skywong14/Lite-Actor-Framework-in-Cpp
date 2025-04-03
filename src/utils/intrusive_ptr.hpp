#ifndef LITEACTORFRAMEWORK_INTRUSIVE_PTR_HPP
#define LITEACTORFRAMEWORK_INTRUSIVE_PTR_HPP

#pragma once

namespace laf {
    /**
     * why Intursive_ptr?
     * 1. Reduces memory allocations
     * 2. Fewer atomic operations in multi-threaded environments
     * 3. Allows actors to be aware of their reference state
     * 4. Optimize message passing efficiency
     * 5. Aligns with CAF's lightweight, high-performance design philosophy
     */
    class ref_counted {
    public:
        virtual ~ref_counted() {}

        // add ref_cnt
        virtual void ref() = 0;

        // def ref_cnt, returns true if ref_cnt > 0
        virtual bool deref() = 0;
    };

    // minimal implementation for intrusive_ptr
    template<typename T>
    class intrusive_ptr {
    private:
        T* ptr_; // raw pointer

    public:
        intrusive_ptr() : ptr_(nullptr) {}
        // default constructor
        explicit intrusive_ptr(T* ptr) : ptr_(ptr) {
            if (ptr_) ptr_->ref();
        }

        // copy constructor
        intrusive_ptr(const intrusive_ptr& other) : ptr_(other.ptr_) {
            if (ptr_) ptr_->ref();
        }

        // copy assignment
        intrusive_ptr& operator=(const intrusive_ptr& other) {
            if (ptr_ == other.ptr_) return;
            if (ptr_ && !ptr_->deref()) delete ptr_;
            ptr_ = other.ptr_;
            if (ptr_) ptr_->ref();
            return *this;
        }

        // no move constructor or move assignment
        intrusive_ptr(intrusive_ptr&& other) = delete;
        intrusive_ptr& operator=(intrusive_ptr&& other) = delete;

        // destructor
        ~intrusive_ptr() {
            if (ptr_ && !ptr_->deref()) delete ptr_;
        }

        // get raw pointer
        T* get() const { return ptr_; }

        // dereference operator
        T* operator->() const { return ptr_; }
        T& operator*() const { return *ptr_; }

        // to boolean
        explicit operator bool() const { return ptr_ != nullptr; }

        // compare
        bool operator==(const intrusive_ptr& other) const {
            return ptr_ == other.ptr_;
        }
        bool operator!=(const intrusive_ptr& other) const {
            return ptr_ != other.ptr_;
        }
    };
}

#endif //LITEACTORFRAMEWORK_INTRUSIVE_PTR_HPP
