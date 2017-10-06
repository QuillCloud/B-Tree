/**
 * The btree is a linked structure which operates much like
 * a binary search tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the tree into two ordered subtrees, a node
 * that stores m client elements partition the tree
 * into m + 1 sorted subtrees.
 */

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
//#include <mach/mach_types.h>
#include <vector>
#include <deque>

// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)
template <typename T>
std::ostream& operator<<(std::ostream &os, const btree<T> &tree);

template <typename T> class btree {
public:
    friend class btree_Iterator<T>;
    friend class btree_Reverse_Iterator<T>;
    friend class btree_Const_Iterator<T>;
    friend class btree_Const_Reverse_Iterator<T>;
    typedef btree_Iterator<T> iterator;
    typedef btree_Reverse_Iterator<T> reverse_iterator;
    typedef btree_Const_Iterator<T> const_iterator;
    typedef btree_Reverse_Iterator<T> const_reverse_iterator;
    /** Hmm, need some iterator typedefs here... friends? **/

    /**
    * Constructs an empty btree.  Note that
    * the elements stored in your btree must
    * have a well-defined zero-arg constructor,
    * copy constructor, operator=, and destructor.
    * The elements must also know how to order themselves
    * relative to each other by implementing operator<
    * and operator==. (These are already implemented on
    * behalf of all built-ins: ints, doubles, strings, etc.)
    *
    * @param maxNodeElems the maximum number of elements
    *        that can be stored in each B-Tree node
    */
    btree(size_t maxNodeElems = 40) : Node_Max(maxNodeElems), root(new Node()), head_(nullptr), tail_(nullptr) {}

    /**
    * The copy constructor and  assignment operator.
    * They allow us to pass around B-Trees by value.
    * Although these operations are likely to be expensive
    * they make for an interesting programming exercise.
    * Implement these operations using value semantics and
    * make sure they do not leak memory.
    */

    /**
    * Copy constructor
    * Creates a new B-Tree as a copy of original.
    *
    * @param original a const lvalue reference to a B-Tree object
    */
    btree(const btree<T>& original);

    /**
    * Move constructor
    * Creates a new B-Tree by "stealing" from original.
    *
    * @param original an rvalue reference to a B-Tree object
    */
    btree(btree<T>&& original);


    /**
    * Copy assignment
    * Replaces the contents of this object with a copy of rhs.
    *
    * @param rhs a const lvalue reference to a B-Tree object
    */
    btree<T>& operator=(const btree<T>& rhs);

    /**
    * Move assignment
    * Replaces the contents of this object with the "stolen"
    * contents of original.
    *
    * @param rhs a const reference to a B-Tree object
    */
    btree<T>& operator=(btree<T>&& rhs);

    /**
    * Puts a breadth-first traversal of the B-Tree onto the output
    * stream os. Elements must, in turn, support the output operator.
    * Elements are separated by space. Should not output any newlines.
    *
    * @param os a reference to a C++ output stream
    * @param tree a const reference to a B-Tree object
    * @return a reference to os
    */
    friend std::ostream& operator<< <T> (std::ostream& os, const btree<T>& tree);


    /**
    * The following can go here
    * -- begin()
    * -- end()
    * -- rbegin()
    * -- rend()
    * -- cbegin()
    * -- cend()
    * -- crbegin()
    * -- crend()
    */
    iterator begin() const { return iterator(head_, tail_); }
    iterator end() const { return iterator(nullptr, tail_); }
    reverse_iterator rbegin() const { return reverse_iterator(tail_, head_); }
    reverse_iterator rend() const { return reverse_iterator(nullptr, head_); }
    const_iterator cbegin() const { return const_iterator(head_, tail_); }
    const_iterator cend() const { return const_iterator(nullptr, tail_); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(tail_, head_); }
    const_reverse_iterator crend() const { return const_reverse_iterator(nullptr, head_); }

    /**
    * Returns an iterator to the matching element, or whatever
    * the non-const end() returns if the element could
    * not be found.
    *
    * @param elem the client element we are trying to match.  The elem,
    *        if an instance of a true class, relies on the operator< and
    *        and operator== methods to compare elem to elements already
    *        in the btree.  You must ensure that your class implements
    *        these things, else code making use of btree<T>::find will
    *        not compile.
    * @return an iterator to the matching element, or whatever the
    *         non-const end() returns if no such match was ever found.
    */
    iterator find(const T& elem);

    /**
    * Identical in functionality to the non-const version of find,
    * save the fact that what's pointed to by the returned iterator
    * is deemed as const and immutable.
    *
    * @param elem the client element we are trying to match.
    * @return an iterator to the matching element, or whatever the
    *         const end() returns if no such match was ever found.
    */
    const_iterator find(const T& elem) const;

    /**
    * Operation which inserts the specified element
    * into the btree if a matching element isn't already
    * present.  In the event where the element truly needs
    * to be inserted, the size of the btree is effectively
    * increases by one, and the pair that gets returned contains
    * an iterator to the inserted element and true in its first and
    * second fields.
    *
    * If a matching element already exists in the btree, nothing
    * is added at all, and the size of the btree stays the same.  The
    * returned pair still returns an iterator to the matching element, but
    * the second field of the returned pair will store false.  This
    * second value can be checked to after an insertion to decide whether
    * or not the btree got bigger.
    *
    * The insert method makes use of T's zero-arg constructor and
    * operator= method, and if these things aren't available,
    * then the call to btree<T>::insert will not compile.  The implementation
    * also makes use of the class's operator== and operator< as well.
    *
    * @param elem the element to be inserted.
    * @return a pair whose first field is an iterator positioned at
    *         the matching element in the btree, and whose second field
    *         stores true if and only if the element needed to be added
    *         because no matching element was there prior to the insert call.
    */
    std::pair<iterator, bool> insert(const T& elem);

    /**
    * Disposes of all internal resources, which includes
    * the disposal of any client objects previously
    * inserted using the insert operation.
    * Check that your implementation does not leak memory!
    */
    ~btree() {
        delete root;
        delete head_;
        root = 0;
        head_ = 0;
    };
    void test();
private:
    struct Node;
    struct Elem;
    std::pair<typename btree<T>::Node*,typename btree<T>::Elem*> copy_recrusion(const Node* nd, Elem* pre);
    struct Node {
        Node() : child_(nullptr) {}
        ~Node() {
            delete child_;
            child_ = 0;
        }
        size_t size() { return Elems_list.size(); }
        void setChild(Node *nd) { child_ = nd; }
        void output();
        std::vector<Elem*> Elems_list;
        Node *child_;
    };
    struct Elem {
        Elem(const T& t, Elem *pre, Elem *next) : elem_(t), pre_(pre), next_(next), child_(nullptr) {}
        const T& value() const { return elem_; }
        void setPre(Elem *ele) { pre_ = ele; }
        void setNext(Elem *ele) { next_ = ele; }
        void setChild(Node *nd) { child_ = nd; }
        ~Elem() {
            delete next_;
            delete child_;
            next_ = 0;
            child_ = 0;
        }
        T elem_;
        Elem *pre_, *next_;
        Node *child_;
    };
    // The details of your implementation go here
    size_t Node_Max;
    Node *root;
    Elem *head_, *tail_;
};

template <typename T>
btree<T>::btree(const btree<T>& original) {
    auto copy = copy_recrusion(original.root, nullptr);
    Node_Max = original.Node_Max;
    root = copy.first;
    tail_ = copy.second;
}

template <typename T>
btree<T>::btree(btree<T>&& original) {
    Node_Max = std::move(original.Node_Max);
    root = std::move(original.root);
    head_ = std::move(original.head_);
    tail_ = std::move(original.tail_);
    original.Node_Max = 0;
    original.root = new Node();
    original.head_ = nullptr;
    original.tail_ = nullptr;
}

template <typename T>
btree<T>& btree<T>::operator=(const btree<T>& rhs) {
    if (this != &rhs) {
        auto copy = copy_recrusion(rhs.root, nullptr);
        Node_Max = rhs.Node_Max;
        root = copy.first;
        tail_ = copy.second;
    }
    return *this;
}

template <typename T>
btree<T>& btree<T>::operator=(btree<T>&& rhs) {
    if (this != &rhs) {
        Node_Max = std::move(rhs.Node_Max);
        root = std::move(rhs.root);
        head_ = std::move(rhs.head_);
        tail_ = std::move(rhs.tail_);
        rhs.Node_Max = 0;
        rhs.root = new Node();
        rhs.head_ = nullptr;
        rhs.tail_ = nullptr;
    }
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream &os, const btree<T> &tree) {
    std::deque<typename btree<T>::Node*> node_list;
    node_list.push_back(tree.root);
    while (!node_list.empty()) {
        auto cur_node = node_list.front();
        node_list.pop_front();
        std::for_each (cur_node->Elems_list.begin(), cur_node->Elems_list.end(), [&os] (const auto& i) {
            os << (*i).value() << " ";
        });
        std::for_each(cur_node->Elems_list.begin(), cur_node->Elems_list.end(), [&node_list] (const auto& i) {
            if (i->child_ != nullptr)
                node_list.push_back(i->child_);
        });
        if (cur_node->child_ != nullptr)
            node_list.push_back(cur_node->child_);
    }
    os << '\b';
    return os;
}

template <typename T>
typename btree<T>::iterator btree<T>::find(const T &elem) {
    auto current_node = root;
    do {
        auto find_ele = std::find_if(current_node->Elems_list.begin(), current_node->Elems_list.end(), [&elem] (const auto& ele) {
            return (elem <= ele->value());
        });
        if (find_ele != current_node->Elems_list.end()) {
            if ((*find_ele)->value() == elem) {
                return iterator((*find_ele));
            } else {
                if ((*find_ele)->child_ != nullptr) {
                    current_node = (*find_ele)->child_;
                } else {
                    return end();
                }
            }
        } else {
            if (current_node->child_ != nullptr) {
                current_node = current_node->child_;
            } else {
                return end();
            }
        }
    } while (1);
}

template <typename T>
typename btree<T>::const_iterator btree<T>::find(const T& elem) const {
    auto current_node = root;
    do {
        auto find_ele = std::find_if(current_node->Elems_list.begin(), current_node->Elems_list.end(), [&elem] (const auto& ele) {
            return (elem <= ele->value());
        });
        if (find_ele != current_node->Elems_list.end()) {
            if ((*find_ele)->value() == elem) {
                return const_iterator((*find_ele));
            } else {
                if ((*find_ele)->child_ != nullptr) {
                    current_node = (*find_ele)->child_;
                } else {
                    return cend();
                }
            }
        } else {
            if (current_node->child_ != nullptr) {
                current_node = current_node->child_;
            } else {
                return cend();
            }
        }
    } while (1);
}

template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &elem) {
    // if the tree is empty
    if (!head_) {
        Elem *newElem = new Elem(elem, nullptr, nullptr);
        head_ = newElem;
        tail_ = newElem;
        root->Elems_list.push_back(newElem);
        return std::make_pair(iterator(head_), true);
    } else {
        auto current_node = root;
        while (1) {
            if (current_node->size() < Node_Max) {
                auto insert_it = std::find_if(current_node->Elems_list.begin(), current_node->Elems_list.end(), [&elem] (const Elem *ele) {
                    return (elem <= ele->value());
                });
                // if not find, insert at end
                if (insert_it != current_node->Elems_list.end()) {
                    if ((*insert_it)->value() == elem) {
                        std::cout << "duplicated" << std::endl;
                        return std::make_pair(iterator(), false);
                    } else {
                        Elem *newElem = new Elem(elem, (*insert_it)->pre_, *insert_it);
                        (*insert_it)->setPre(newElem);
                        if (newElem->pre_ != nullptr) {
                            newElem->pre_->setNext(newElem);
                        } else {
                            head_ = newElem;
                        }
                        current_node->Elems_list.insert(insert_it, newElem);
                        return std::make_pair(iterator(newElem), true);
                    }
                } else {
                    --insert_it;
                    Elem *newElem = new Elem(elem, *insert_it, (*insert_it)->next_);
                    (*insert_it)->setNext(newElem);
                    if (newElem->next_ != nullptr) {
                        newElem->next_->setPre(newElem);
                    } else {
                        tail_ = newElem;
                    }
                    ++insert_it;
                    current_node->Elems_list.insert(insert_it, newElem);
                    return std::make_pair(iterator(newElem), true);
                }
            } else {
                auto insert_it = std::find_if(current_node->Elems_list.begin(), current_node->Elems_list.end(), [&elem] (const Elem *ele) {
                    return (elem <= ele->value());
                });
                if (insert_it != current_node->Elems_list.end()) {
                    if ((*insert_it)->value() == elem) {
                        std::cout << "duplicated2" << std::endl;
                        return std::make_pair(iterator(), false);
                    } else {
                        if ((*insert_it)->child_ != nullptr) {
                            current_node = (*insert_it)->child_;
                        } else {
                            Node *newNode = new Node();
                            Elem *newElem = new Elem(elem, (*insert_it)->pre_, *insert_it);
                            (*insert_it)->setChild(newNode);
                            (*insert_it)->setPre(newElem);
                            newNode->Elems_list.push_back(newElem);
                            if (newElem->pre_ != nullptr) {
                                (newElem->pre_)->setNext(newElem);
                            } else {
                                head_ = newElem;
                            }
                            return std::make_pair(iterator(newElem), true);
                        }
                    }
                } else {
                    // last child is exist
                    if (current_node->child_ != nullptr) {
                        current_node = current_node->child_;
                    } else {
                        --insert_it;
                        Node *newNode = new Node();
                        Elem *newElem = new Elem(elem, *insert_it, (*insert_it)->next_);
                        newNode->Elems_list.push_back(newElem);
                        current_node->setChild(newNode);
                        (*insert_it)->setNext(newElem);
                        if (newElem->next_ != nullptr) {
                            (newElem->next_)->setPre(newElem);
                        } else {
                            tail_ = newElem;
                        }
                        return std::make_pair(iterator(newElem), true);
                    }
                }
            }
        }
        return std::make_pair(iterator(), false);

    }

}

template <typename T>
void btree<T>::test() {
    std::deque<Node*> node_list;
    node_list.push_back(root);
    while (!node_list.empty()) {
        auto cur_node = node_list.front();
        node_list.pop_front();
        std::cout << "node" << std::endl;
        cur_node->output();
        std::for_each(cur_node->Elems_list.begin(), cur_node->Elems_list.end(), [&node_list] (const auto& i) {
            if (i->child_ != nullptr)
                node_list.push_back(i->child_);
        });
        if (cur_node->child_ != nullptr)
            node_list.push_back(cur_node->child_);
    }
}

template <typename T>
std::pair<typename btree<T>::Node*,typename btree<T>::Elem*> btree<T>::copy_recrusion(const Node* nd, Elem* pre) {
    Node *resultNode = new Node();
    for (auto i : nd->Elems_list) {
        Elem *copy_i = new Elem(i->value(), nullptr, nullptr);
        if (i->child_ != nullptr) {
            auto Nd_Ele = copy_recrusion(i->child_, pre);
            copy_i->setChild(Nd_Ele.first);
            copy_i->setPre(Nd_Ele.second);
            copy_i->pre_->setNext(copy_i);
            resultNode->Elems_list.push_back(copy_i);
            pre = copy_i;
        } else {
            copy_i->setPre(pre);
            if (copy_i->pre_ != nullptr) {
                copy_i->pre_->setNext(copy_i);
            } else {
                head_ = copy_i;
            }
            resultNode->Elems_list.push_back(copy_i);
            pre = copy_i;
        }
    }
    if (nd->child_ != nullptr) {
        auto Nd_Ele = copy_recrusion(nd->child_, pre);
        resultNode->setChild(Nd_Ele.first);
        pre = Nd_Ele.second;
    }
    return std::make_pair(resultNode, pre);
}

template <typename T>
void btree<T>::Node::output() {
    for (auto i : Elems_list) {
        std::cout << (*i).value() << " ";
    }
    std::cout << std::endl;
}

#endif
