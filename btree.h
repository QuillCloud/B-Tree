#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>
#include <deque>
#include <algorithm>

#include "btree_iterator.h"

// Declare of output operator <<
template <typename T>
std::ostream& operator<<(std::ostream &os, const btree<T> &tree);

template <typename T> class btree {
public:
    // Friend iterator classes
    friend class btree_Iterator<T>;
    friend class btree_Reverse_Iterator<T>;
    friend class btree_Const_Iterator<T>;
    friend class btree_Const_Reverse_Iterator<T>;

    // Iterator typedefs
    typedef btree_Iterator<T> iterator;
    typedef btree_Reverse_Iterator<T> reverse_iterator;
    typedef btree_Const_Iterator<T> const_iterator;
    typedef btree_Const_Reverse_Iterator<T> const_reverse_iterator;

    // Constructs of btree
    // argument 'maxNodeElems' is maximum number of element that can be stored in each B-Tree node
    btree(size_t maxNodeElems = 40) : Node_Max(maxNodeElems), root(new Node()), head_(nullptr), tail_(nullptr) {}

    // Copy constructor
    btree(const btree<T>& original);

    // Move constructor
    btree(btree<T>&& original);

    // Copy assignment
    btree<T>& operator=(const btree<T>& rhs);

    // Move assignment
    btree<T>& operator=(btree<T>&& rhs);

    // Overload of operator '<<'
    // Puts a breadth-first traversal of the B-Tree onto the output stream os.
    friend std::ostream& operator<< <T> (std::ostream& os, const btree<T>& tree);

    // begin()/end()
    iterator begin() { return iterator(head_, tail_); }
    iterator end() { return iterator(nullptr, tail_); }
    const_iterator begin() const { return const_iterator(head_, tail_); }
    const_iterator end() const { return const_iterator(nullptr, tail_); }
    // rbegin()/rend()
    reverse_iterator rbegin() { return reverse_iterator(tail_, head_); }
    reverse_iterator rend() { return reverse_iterator(nullptr, head_); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(tail_, head_); }
    const_reverse_iterator rend() const { return const_reverse_iterator(nullptr, head_); }
    // cbegin()/cend()
    const_iterator cbegin() const { return const_iterator(head_, tail_); }
    const_iterator cend() const { return const_iterator(nullptr, tail_); }
    // crbegin()/crend()
    const_reverse_iterator crbegin() const { return const_reverse_iterator(tail_, head_); }
    const_reverse_iterator crend() const { return const_reverse_iterator(nullptr, head_); }

    // Function for find the elements in the B-Tree
    iterator find(const T& elem);

    //Identical in functionality to the non-const version of find.
    const_iterator find(const T& elem) const;
    
    // Insert elements into the B-Tree
    std::pair<iterator, bool> insert(const T& elem);

    // Destructor part
    ~btree() {
        // use funciton destructor_helper to free all Nodes and Elements in B-Tree
        destructor_helper(root);
    };
private:
    // Declare two struct Node and Elem
    struct Node;
    struct Elem;

    // Private function that make copy of Node.
    // @Param: nd is the copy target node, pre is the parent element of this node (for root is nullptr)
    // @Return: return a pair, first is copied node, second is tail of tree(including copied node and its child node)
    std::pair<typename btree<T>::Node*,typename btree<T>::Elem*> copy_node(const Node* nd, Elem* pre);
    // Private function that free the Node
    // @Param: nd is the Node that will be free.(if nd is root, whole B-Tree will be freed)
    void destructor_helper(Node*& nd);
    // Private function that find the element location in the node(use binary search)
    // @Param: nd is the Node for search, ele is the element value
    // @Return: a pair, fist is element location in Node(iterator), second bool(true if find)
    std::pair<typename std::vector<typename btree<T>::Elem*>::iterator, bool> find_ele_location(Node* nd, const T& elem) const;
    // struct Node, represent the Nodes in B-Tree
    struct Node {

        // constructor and destructor
        Node() : child_(nullptr) {}
        ~Node() {}
        // get size of Node
        size_t size() { return Elems_list.size(); }
        // set the pointer child_ point to Node's last child
        void setChild(Node *nd) { child_ = nd; }
        // Elems_list store the pointers point to elements in Node
        std::vector<Elem*> Elems_list;
        // a pointer point to the Node's last child
        Node *child_;
    };
    // struct Elem, represent the Elements in B-Tree
    struct Elem {
        // constructor and destructor
        Elem(const T& t, Elem *pre, Elem *next) : elem_(t), pre_(pre), next_(next), child_(nullptr) {}
        ~Elem() {}
        // get value of element
        const T& value() const { return elem_; }
        // set pointer 'pre_' to previous element
        void setPre(Elem *ele) { pre_ = ele;}
        // set pointer 'next_' to next element
        void setNext(Elem *ele) { next_ = ele; }
        // set pointer 'child_' point to child of Node in the location of this element
        void setChild(Node *nd) { child_ = nd; }

        // 'elem_' store the value of element
        T elem_;
        // pointers point to previous and next elements
        Elem *pre_, *next_;
        // pointer point to the child of Node in the location of this element
        Node *child_;
    };
    // maximum number of element that can be stored in each B-Tree node
    size_t Node_Max;
    // pointer point to the root node of B-Tree
    Node *root;
    // pointers point to the head and tail elements
    Elem *head_, *tail_;
};

// Copy constructor
template <typename T>
btree<T>::btree(const btree<T>& original) {
    // use function copy_node to get copy of original's root
    auto copy = copy_node(original.root, nullptr);
    Node_Max = original.Node_Max;
    root = copy.first;
    tail_ = copy.second;
}

// Move constructor
template <typename T>
btree<T>::btree(btree<T>&& original) {
    Node_Max = std::move(original.Node_Max);
    root = std::move(original.root);
    head_ = std::move(original.head_);
    tail_ = std::move(original.tail_);
    // set original to empty
    original.Node_Max = 0;
    original.root = new Node();
    original.head_ = nullptr;
    original.tail_ = nullptr;
}

template <typename T>
btree<T>& btree<T>::operator=(const btree<T>& rhs) {
    if (this != &rhs) {
        // delete 'root' and 'head_' to avoid memory leak
        destructor_helper(root);
        // use function copy_node to get copy of original's root
        auto copy = copy_node(rhs.root, nullptr);
        Node_Max = rhs.Node_Max;
        root = copy.first;
        tail_ = copy.second;
    }
    return *this;
}

template <typename T>
btree<T>& btree<T>::operator=(btree<T>&& rhs) {
    if (this != &rhs) {
        // delete 'root' and 'head_' to avoid memory leak
        destructor_helper(root);
        Node_Max = std::move(rhs.Node_Max);
        root = std::move(rhs.root);
        head_ = std::move(rhs.head_);
        tail_ = std::move(rhs.tail_);
        // set original to empty
        rhs.Node_Max = 0;
        rhs.root = new Node();
        rhs.head_ = nullptr;
        rhs.tail_ = nullptr;
    }
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream &os, const btree<T> &tree) {
    // use a deque to store each nodes, start from root
    std::deque<typename btree<T>::Node*> node_list;
    node_list.push_back(tree.root);
    while (!node_list.empty()) {
        // get first node in deque
        auto cur_node = node_list.front();
        node_list.pop_front();
        // output node's element value and push back child nodes at end of deque
        std::for_each (cur_node->Elems_list.begin(), cur_node->Elems_list.end(), [&os, &node_list] (const auto& i) {
            os << (*i).value() << " ";
            if (i->child_ != nullptr)
                node_list.push_back(i->child_);
        });
        // push back last child node to deque if exists
        if (cur_node->child_ != nullptr)
            node_list.push_back(cur_node->child_);
    }
    // delete last space
    os << '\b';
    return os;
}

template <typename T>
typename btree<T>::iterator btree<T>::find(const T &elem) {
    // start with root
    auto current_node = root;
    do {
        // use binary research to find the proper location of element in current node
        auto pair = find_ele_location(current_node, elem);
        auto it = pair.first;
        // if the element in that location, return the iterator of that element
        if (pair.second == true) {
            return iterator(*(it));
        } else {
            // if the element is not in that location, check if that location has a child node
            // if has a child, change current node to that child node for next loop
            // if not, means element not in the tree, return end()
            if ((*it)->value() < elem)
                ++it;
            if (it != current_node->Elems_list.end()) {
                if ((*it)->child_ != nullptr) {
                    current_node = (*it)->child_;
                } else {
                    return end();
                }
            } else {
                if (current_node->child_ != nullptr) {
                    current_node = current_node->child_;
                } else {
                    return end();
                }
            }
        }
    } while (1);
}

template <typename T>
typename btree<T>::const_iterator btree<T>::find(const T& elem) const {
    // function body is quite similar to non-const 'find', but return type is const_iterator
    auto current_node = root;
    do {
        auto pair = find_ele_location(current_node, elem);
        auto it = pair.first;
        if (pair.second == true) {
            return const_iterator(*(it));
        } else {
            if ((*it)->value() < elem)
                ++it;
            if (it != current_node->Elems_list.end()) {
                if ((*it)->child_ != nullptr) {
                    current_node = (*it)->child_;
                } else {
                    return cend();
                }
            } else {
                if (current_node->child_ != nullptr) {
                    current_node = current_node->child_;
                } else {
                    return cend();
                }
            }
        }
    } while (1);
}

template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &elem) {
    // if the tree is empty, set head_ and add param element to the root node
    if (!head_) {
        Elem *newElem = new Elem(elem, nullptr, nullptr);
        head_ = newElem;
        tail_ = newElem;
        root->Elems_list.push_back(newElem);
        return std::make_pair(iterator(head_, tail_), true);
    }
    // if tree is not empty, start with root node
    auto current_node = root;
    do {
        // find the proper location in current node (use binary search function 'find_ele_location')
        auto pair = find_ele_location(current_node, elem);
        auto insert_it = pair.first;
        // if element already in that location, cannot insert return pair(itearator, false)
        if (pair.second == true)
            return std::make_pair(iterator(*insert_it, tail_), false);
        if ((*insert_it)->value() < elem)
            ++insert_it;
        // if current node is not full, insert element into current node
        if (current_node->size() < Node_Max) {
            // if location is not at end of Node
            if (insert_it != current_node->Elems_list.end()) {
                // insert the param element before the found element (adjust link state before insert)
                Elem *newElem = new Elem(elem, (*insert_it)->pre_, *insert_it);
                (*insert_it)->setPre(newElem);
                if (newElem->pre_ != nullptr) {
                    newElem->pre_->setNext(newElem);
                } else {
                    head_ = newElem;
                }
                current_node->Elems_list.insert(insert_it, newElem);
                return std::make_pair(iterator(newElem, tail_), true);
            } else {
                // if location is at end of Node
                // insert the param element at end of current node (adjust link state before insert)
                --insert_it;
                Elem *newElem = new Elem(elem, *insert_it, (*insert_it)->next_);
                (*insert_it)->setNext(newElem);
                if (newElem->next_ != nullptr) {
                    newElem->next_->setPre(newElem);
                } else {
                    tail_ = newElem;
                }
                current_node->Elems_list.push_back(newElem);
                return std::make_pair(iterator(newElem, tail_), true);
            }
        } else {
            // if current node is full, need insert into sub-tree of this location
            // if location is not at end of Node
            if (insert_it != current_node->Elems_list.end()) {
                // check if has a child node, if so, set current node to child node for next loop
                if ((*insert_it)->child_ != nullptr) {
                    current_node = (*insert_it)->child_;
                } else {
                    // if has no child node, create a child node and insert the param element
                    // (adjust link state before insert)
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
                    return std::make_pair(iterator(newElem, tail_), true);
                }
            } else {
                // if location is at end of Node
                // check if last child node exists, if so, set current node to last child node for next loop
                if (current_node->child_ != nullptr) {
                    current_node = current_node->child_;
                } else {
                    // if no last child node, create a child node for current node
                    // then insert param element to child node (adjust link state before insert)
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
                    return std::make_pair(iterator(newElem, tail_), true);
                }
            }
        }
    } while (1);
}

// A recursion function for copy the node
// In this class used for copy root node, so usually start with the root node and nullptr
template <typename T>
std::pair<typename btree<T>::Node*,typename btree<T>::Elem*> btree<T>::copy_node(const Node* nd, Elem* pre) {
    // create Node 'resultNode' which is first element of return pair
    Node *resultNode = new Node();
    // go through each element in param node
    for (auto i : nd->Elems_list) {
        // create new Elem which has same value of original node
        Elem *copy_i = new Elem(i->value(), nullptr, nullptr);
        if (i->child_ != nullptr) {
            // if original node has a child
            // use 'copy_node' function to get copy of child node
            auto Nd_Ele = copy_node(i->child_, pre);
            // then update copied Elem's link(including child node) and the it's previous node's link state
            copy_i->setChild(Nd_Ele.first);
            copy_i->setPre(Nd_Ele.second);
            copy_i->pre_->setNext(copy_i);
        } else {
            // if original node has no child
            // update copied Elem's link state and the it's previous node's link state
            copy_i->setPre(pre);
            if (copy_i->pre_ != nullptr) {
                copy_i->pre_->setNext(copy_i);
            } else {
                head_ = copy_i;
            }
        }
        // update the 'pre' to copied Elem and push back copied Elem to 'resultNode'
        pre = copy_i;
        resultNode->Elems_list.push_back(copy_i);
    }
    // check if param node has last child node, if so, get copy of it and update 'resultNode'
    if (nd->child_ != nullptr) {
        auto Nd_Ele = copy_node(nd->child_, pre);
        resultNode->setChild(Nd_Ele.first);
        pre = Nd_Ele.second;
    }
    // return the pair, first is copied node 'resultNode'
    // second is 'pre', which is the tail element for tree(including 'resultNode' and its child node)
    return std::make_pair(resultNode, pre);
}

// A recursion function to free the Node
// In this class, use for free whole B-Tree, so usually start with the root node
template <typename T>
void btree<T>::destructor_helper(Node*& nd) {
    // go through each element in param node
    for (auto& i : nd->Elems_list) {
        // if Element has a child Node, use child Node as param to do recursion
        if (i->child_ != nullptr)
            destructor_helper(i->child_);
        // free the Elem
        delete i;
        i = 0;
    }
    // if param node has last child Node, use child Node as param to do recursion
    if (nd->child_ != nullptr)
        destructor_helper(nd->child_);
    // free the Node
    delete nd;
    nd = 0;
}

// use binary search to find the element location in the node
template <typename T>
std::pair<typename std::vector<typename btree<T>::Elem*>::iterator, bool>  btree<T>::find_ele_location(Node* nd, const T& elem) const {
    int lower(0), upper(nd->Elems_list.size() - 1), current((lower + upper) / 2);
    bool find_flag = false;
    while (lower <= upper) {
        if (elem < nd->Elems_list[current]->value()) {
            upper = current - 1;
        } else if (elem > nd->Elems_list[current]->value()) {
            lower = current + 1;
        } else {
            find_flag = true;
            break;
        }
        current = (lower + upper) / 2;
    }
    return std::make_pair(nd->Elems_list.begin() + current, find_flag);
}


#endif
