#include <iostream>
#include <utility>

template<typename T>
class SharedPtr {
private:
    T* ptr_;
    
    struct ControlBlock {
        size_t ref_count;
        
        ControlBlock() : ref_count(1) {}
    };
    
    ControlBlock* control_block_;
    
    void cleanup() {
        if (control_block_) {
            --control_block_->ref_count;
            if (control_block_->ref_count == 0) {
                delete ptr_;
                delete control_block_;
            }
        }
    }
    
public:
    SharedPtr() : ptr_(nullptr), control_block_(nullptr) {}
    
    explicit SharedPtr(T* ptr) : ptr_(ptr), control_block_(new ControlBlock()) {}

    SharedPtr(const SharedPtr& other) 
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        if (control_block_) {
            ++control_block_->ref_count;
        }
    }
    
    SharedPtr(SharedPtr&& other) noexcept 
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        other.ptr_ = nullptr;
        other.control_block_ = nullptr;
    }
    
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            control_block_ = other.control_block_;
            if (control_block_) {
                ++control_block_->ref_count;
            }
        }
        return *this;
    }
    
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            control_block_ = other.control_block_;
            other.ptr_ = nullptr;
            other.control_block_ = nullptr;
        }
        return *this;
    }
    
    ~SharedPtr() {
        cleanup();
    }
    
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    
    size_t use_count() const {
        return control_block_ ? control_block_->ref_count : 0;
    }
    
    bool unique() const {
        return use_count() == 1;
    }
    
    void reset() {
        cleanup();
        ptr_ = nullptr;
        control_block_ = nullptr;
    }
    
    void reset(T* ptr) {
        cleanup();
        ptr_ = ptr;
        control_block_ = ptr ? new ControlBlock() : nullptr;
    }
    
    template<typename U, typename... Args>
    friend SharedPtr<U> make_shared(Args&&... args);
};


template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}