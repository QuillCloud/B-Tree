#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
#include <assert.h>
#include "btree.h"

template <typename T> class btree;
template <typename T> class btree_Iterator;
template <typename T> class btree_Reverse_Iterator;
template <typename T> class btree_Const_Iterator;
template <typename T> class btree_Const_Reverse_Iterator;
// iterator
template <typename T> class btree_Iterator {
public:
    typedef std::ptrdiff_t                     difference_type;
    typedef std::forward_iterator_tag          iterator_category;
    typedef T                                  value_type;
    typedef T*                                 pointer;
    typedef T&                                 reference;

    // constructor
    // param including 'tail', if do decrement operator with end(), then iterator point to 'tail'
    btree_Iterator(typename btree<T>::Elem *pointee = nullptr, typename btree<T>::Elem *tail = nullptr)
            : pointee_(pointee), tail_(tail) {}
    reference operator*() const;
    pointer operator->() const;
    btree_Iterator<T>& operator++();
    btree_Iterator<T>& operator--();
    btree_Iterator<T> operator++(int);
    btree_Iterator<T> operator--(int);
    bool operator==(const btree_Iterator<T>& other) const;
    bool operator==(const btree_Const_Iterator<T>& other) const;
    bool operator!=(const btree_Iterator<T>& other) const { return !operator==(other); }
    bool operator!=(const btree_Const_Iterator<T>& other) const { return !operator==(other); }
    operator btree_Const_Iterator<T>() {
        return btree_Const_Iterator<T>{pointee_};
    }
private:
    mutable typename btree<T>::Elem *pointee_;
    typename btree<T>::Elem *tail_;
};

// reverse_iterator
template <typename T> class btree_Reverse_Iterator {
public:
    typedef std::ptrdiff_t                     difference_type;
    typedef std::forward_iterator_tag          iterator_category;
    typedef T                                  value_type;
    typedef T*                                 pointer;
    typedef T&                                 reference;

    // constructor
    // param including 'head', if do decrement operator with rend(), then iterator point to 'head'
    btree_Reverse_Iterator(typename btree<T>::Elem *pointee = nullptr, typename btree<T>::Elem *head = nullptr)
            : pointee_(pointee), head_(head) {}
    reference operator*() const;
    pointer operator->() const;
    btree_Reverse_Iterator<T>& operator++();
    btree_Reverse_Iterator<T>& operator--();
    btree_Reverse_Iterator<T> operator++(int);
    btree_Reverse_Iterator<T> operator--(int);
    bool operator==(const btree_Reverse_Iterator<T>& other) const;
    bool operator==(const btree_Const_Reverse_Iterator<T>& other) const;
    bool operator!=(const btree_Reverse_Iterator<T>& other) const { return !operator==(other); }
    bool operator!=(const btree_Const_Reverse_Iterator<T>& other) const { return !operator==(other); }
    operator btree_Const_Reverse_Iterator<T>() {
        return btree_Const_Reverse_Iterator<T>{pointee_};
    }
private:
    mutable typename btree<T>::Elem *pointee_;
    typename btree<T>::Elem *head_;
};

// const_iterator, similar to 'iterator'
template <typename T> class btree_Const_Iterator {
public:
    friend class btree_Iterator<T>;
    typedef std::ptrdiff_t                     difference_type;
    typedef std::forward_iterator_tag          iterator_category;
    typedef T                                  value_type;
    typedef const T*                           pointer;
    typedef const T&                           reference;

    btree_Const_Iterator(typename btree<T>::Elem *pointee = nullptr, typename btree<T>::Elem *tail = nullptr)
            : pointee_(pointee), tail_(tail) {}
    reference operator*() const;
    pointer operator->() const;
    btree_Const_Iterator<T>& operator++();
    btree_Const_Iterator<T>& operator--();
    btree_Const_Iterator<T> operator++(int);
    btree_Const_Iterator<T> operator--(int);
    bool operator==(const btree_Const_Iterator<T>& other) const;
    bool operator!=(const btree_Const_Iterator<T>& other) const { return !operator==(other); }
private:
    mutable typename btree<T>::Elem *pointee_;
    typename btree<T>::Elem *tail_;
};

// const_reverse_iterator, similar to 'reverse_iterator'
template <typename T> class btree_Const_Reverse_Iterator {
public:
    friend class btree_Reverse_Iterator<T>;
    typedef std::ptrdiff_t                     difference_type;
    typedef std::forward_iterator_tag          iterator_category;
    typedef T                                  value_type;
    typedef const T*                           pointer;
    typedef const T&                           reference;

    btree_Const_Reverse_Iterator(typename btree<T>::Elem *pointee = nullptr, typename btree<T>::Elem *head = nullptr)
            : pointee_(pointee), head_(head) {}
    reference operator*() const;
    pointer operator->() const;
    btree_Const_Reverse_Iterator<T>& operator++();
    btree_Const_Reverse_Iterator<T>& operator--();
    btree_Const_Reverse_Iterator<T> operator++(int);
    btree_Const_Reverse_Iterator<T> operator--(int);
    bool operator==(const btree_Const_Reverse_Iterator<T>& other) const;
    bool operator!=(const btree_Const_Reverse_Iterator<T>& other) const { return !operator==(other); }
private:
    mutable typename btree<T>::Elem *pointee_;
    typename btree<T>::Elem *head_;
};

template <typename T> typename btree_Iterator<T>::reference
btree_Iterator<T>::operator*() const {
    return pointee_->elem_;
}

template <typename T> typename btree_Iterator<T>::pointer
btree_Iterator<T>::operator->() const {
    return &(operator*());
}

template <typename T>
btree_Iterator<T>& btree_Iterator<T>::operator++() {
    assert(pointee_ != nullptr);
    pointee_ = pointee_->next_;
    return *this;
}

template <typename T>
btree_Iterator<T>& btree_Iterator<T>::operator--() {
    if (pointee_ != nullptr) {
        assert(pointee_->pre_ != nullptr);
        pointee_ = pointee_->pre_;
    } else {
        pointee_ = tail_;
    }
    return *this;
}

template <typename T>
btree_Iterator<T> btree_Iterator<T>::operator++(int) {
    assert(pointee_ != nullptr);
    auto ptCopy = pointee_;
    pointee_ = pointee_->next_;
    return ptCopy;
}

template <typename T>
btree_Iterator<T> btree_Iterator<T>::operator--(int) {
    auto ptCopy = pointee_;
    if (pointee_ != nullptr) {
        assert(pointee_->pre_ != nullptr);
        pointee_ = pointee_->pre_;
    } else {
        pointee_ = tail_;
    }
    return ptCopy;
}

template <typename T>
bool btree_Iterator<T>::operator==(const btree_Iterator<T>& other) const {
    return this->pointee_ == other.pointee_;
}

template <typename T>
bool btree_Iterator<T>::operator==(const btree_Const_Iterator<T>& other) const {
    return this->pointee_ == other.pointee_;
}

template <typename T> typename btree_Reverse_Iterator<T>::reference
btree_Reverse_Iterator<T>::operator*() const {
    return pointee_->elem_;
}

template <typename T> typename btree_Reverse_Iterator<T>::pointer
btree_Reverse_Iterator<T>::operator->() const {
    return &(operator*());
}

template <typename T>
btree_Reverse_Iterator<T>& btree_Reverse_Iterator<T>::operator++() {
    assert(pointee_ != nullptr);
    pointee_ = pointee_->pre_;
    return *this;
}

template <typename T>
btree_Reverse_Iterator<T>& btree_Reverse_Iterator<T>::operator--() {
    if (pointee_ != nullptr) {
        assert( pointee_->next_ != nullptr);
        pointee_ = pointee_->next_;
    } else {
        pointee_ = head_;
    }
    return *this;
}

template <typename T>
btree_Reverse_Iterator<T> btree_Reverse_Iterator<T>::operator++(int) {
    assert(pointee_ != nullptr);
    auto ptCopy = pointee_;
    pointee_ = pointee_->pre_;
    return ptCopy;
}

template <typename T>
btree_Reverse_Iterator<T> btree_Reverse_Iterator<T>::operator--(int) {
    auto ptCopy = pointee_;
    if (pointee_ != nullptr) {
        assert( pointee_->next_ != nullptr);
        pointee_ = pointee_->next_;
    } else {
        pointee_ = head_;
    }
    return ptCopy;
}

template <typename T>
bool btree_Reverse_Iterator<T>::operator==(const btree_Reverse_Iterator<T>& other) const {
    return this->pointee_ == other.pointee_;
}

template <typename T>
bool btree_Reverse_Iterator<T>::operator==(const btree_Const_Reverse_Iterator<T>& other) const {
    return this->pointee_ == other.pointee_;
}

template <typename T> typename btree_Const_Iterator<T>::reference
btree_Const_Iterator<T>::operator*() const {
    return pointee_->elem_;
}

template <typename T> typename btree_Const_Iterator<T>::pointer
btree_Const_Iterator<T>::operator->() const {
    return &(operator*());
}

template <typename T>
btree_Const_Iterator<T>& btree_Const_Iterator<T>::operator++() {
    assert(pointee_ != nullptr);
    pointee_ = pointee_->next_;
    return *this;
}

template <typename T>
btree_Const_Iterator<T>& btree_Const_Iterator<T>::operator--() {
    if (pointee_ != nullptr) {
        assert(pointee_->pre_ != nullptr);
        pointee_ = pointee_->pre_;
    } else {
        pointee_ = tail_;
    }
    return *this;
}

template <typename T>
btree_Const_Iterator<T> btree_Const_Iterator<T>::operator++(int) {
    assert(pointee_ != nullptr);
    auto ptCopy = pointee_;
    pointee_ = pointee_->next_;
    return ptCopy;
}

template <typename T>
btree_Const_Iterator<T> btree_Const_Iterator<T>::operator--(int) {
    auto ptCopy = pointee_;
    if (pointee_ != nullptr) {
        assert(pointee_->pre_ != nullptr);
        pointee_ = pointee_->pre_;
    } else {
        pointee_ = tail_;
    }
    return ptCopy;
}

template <typename T>
bool btree_Const_Iterator<T>::operator==(const btree_Const_Iterator<T>& other) const {
    return this->pointee_ == other.pointee_;
}

template <typename T> typename btree_Const_Reverse_Iterator<T>::reference
btree_Const_Reverse_Iterator<T>::operator*() const {
    return pointee_->elem_;
}

template <typename T> typename btree_Const_Reverse_Iterator<T>::pointer
btree_Const_Reverse_Iterator<T>::operator->() const {
    return &(operator*());
}

template <typename T>
btree_Const_Reverse_Iterator<T>& btree_Const_Reverse_Iterator<T>::operator++() {
    assert(pointee_ != nullptr);
    pointee_ = pointee_->pre_;
    return *this;
}

template <typename T>
btree_Const_Reverse_Iterator<T>& btree_Const_Reverse_Iterator<T>::operator--() {
    if (pointee_ != nullptr) {
        assert( pointee_->next_ != nullptr);
        pointee_ = pointee_->next_;
    } else {
        pointee_ = head_;
    }
    return *this;
}

template <typename T>
btree_Const_Reverse_Iterator<T> btree_Const_Reverse_Iterator<T>::operator++(int) {
    assert(pointee_ != nullptr);
    auto ptCopy = pointee_;
    pointee_ = pointee_->pre_;
    return ptCopy;
}

template <typename T>
btree_Const_Reverse_Iterator<T> btree_Const_Reverse_Iterator<T>::operator--(int) {
    auto ptCopy = pointee_;
    if (pointee_ != nullptr) {
        assert( pointee_->next_ != nullptr);
        pointee_ = pointee_->next_;
    } else {
        pointee_ = head_;
    }
    return ptCopy;
}

template <typename T>
bool btree_Const_Reverse_Iterator<T>::operator==(const btree_Const_Reverse_Iterator<T>& other) const {
    return this->pointee_ == other.pointee_;
}

#endif