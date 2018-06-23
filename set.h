#ifndef SET_H
#define SET_H

#include <memory>
#include <iostream>
#include <list>
#include <cassert>

template <typename T>
struct node{
    node(T const& val) :
        val(val),
        ptr(nullptr),
        left(nullptr),
        right(nullptr)
    {}
	
    T val;
    typename std::list< std::shared_ptr< node<T> > >::iterator ptr;
    std::shared_ptr< node<T> > left, right;
};

template <typename T>
struct set{
    struct iterator{
        using difference_type = std::ptrdiff_t;
        using value_type = T const;
        using pointer = T const*;
        using reference = T const&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator();
        iterator(typename std::list< std::shared_ptr< node<T> > >::const_iterator other, set<T> const* owner);
        iterator(iterator const& other);
        T const& operator*() const;
        T const* operator->() const;
        iterator operator++(int);
        iterator& operator++();
        iterator operator--(int);
        iterator& operator--();
        bool operator!=(iterator const& other) const;
        bool operator!=(typename std::list< std::shared_ptr< node<T> > >::const_iterator other) const;
        bool operator==(iterator const& other) const;
        iterator& operator=(iterator const& other);

    private:
        typename std::list< std::shared_ptr< node<T> > >::const_iterator it;
        set<T> const* owner;
    };


    using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = reverse_iterator;
	using const_iterator = iterator;

    set();
    set(set<T> const& other);
    ~set() = default;
    set<T>& operator=(set<T> const& other);

    std::pair<iterator, bool> insert(T const& val);
    iterator erase(iterator it);
    iterator find(T const& val) const;
    iterator lower_bound(T const& val) const;
    iterator upper_bound(T const& val) const;

    bool empty() const { return q.size() == 0; }
    void clear();
    template <typename U>
    friend void swap(set<U> &a, set<U> &b);

    iterator begin() const;
    iterator end() const;
    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

private:
    std::shared_ptr< node<T> > head;
    std::list< std::shared_ptr< node<T> > > q;

    std::shared_ptr< node<T> > tree_traversal(std::shared_ptr< node<T> > root, T const& val) const;
    void tree_copy(std::shared_ptr< node<T> > root, std::shared_ptr< node<T> > const& other_root, set<T> const& other);
    std::shared_ptr< node<T> > parent(std::shared_ptr< node<T> > root, T const& val);
};




















template <typename T>
set<T>::iterator::iterator() :
    it(nullptr),
    owner(nullptr)
{}

template <typename T>
set<T>::iterator::iterator(typename std::list< std::shared_ptr< node<T> > >::const_iterator other, set<T> const* owner) :
    it(other),
    owner(owner)
{}

template <typename T>
set<T>::iterator::iterator(set<T>::iterator const& other) :
    it(other.it),
    owner(other.owner)
{}

template <typename T>
T const& set<T>::iterator::operator*() const{
    return (*it)->val;
}

template <typename T>
T const* set<T>::iterator::operator->() const{
    return &(*it)->val;
}

template <typename T>
typename set<T>::iterator set<T>::iterator::operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
}

template <typename T>
typename set<T>::iterator& set<T>::iterator::operator++() {
    it++;
    return *this;
}

template <typename T>
typename set<T>::iterator set<T>::iterator::operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
}

template <typename T>
typename set<T>::iterator& set<T>::iterator::operator--() {
    it--;
    return *this;
}

template <typename T>
bool set<T>::iterator::operator!=(iterator const& other) const {
    return it != other.it;
}

template <typename T>
bool set<T>::iterator::operator!=(typename std::list< std::shared_ptr< node<T> > >::const_iterator other) const {
    return it != other;
}

template <typename T>
bool set<T>::iterator::operator==(iterator const& other) const {
    return it == other.it;
}

template <typename T>
typename set<T>::iterator& set<T>::iterator::operator=(iterator const& other) {
    it = other.it;
	owner = other.owner;
    return *this;
}
















































template <typename T>
set<T>::set() :
    head(nullptr)
{}

template <typename T>
std::pair<typename set<T>::iterator, bool> set<T>::insert(T const& val) {
    if (head) {
        std::shared_ptr< node<T> > it = tree_traversal(head, val);
        if (val < it->val) {
            new (&it->left) std::shared_ptr< node<T> >(new node<T>(val));
            it->left->ptr = q.insert(it->ptr, it->left);
            return {{it->left->ptr, this}, true};
        } else if (it->val < val) {
            new (&it->right) std::shared_ptr< node<T> >(new node<T>(val));
            auto tmp = it->ptr;
            tmp++;
            it->right->ptr = q.insert(tmp, it->right);
            return {{it->right->ptr, this}, true};
        } else {
            return {{it->ptr, this}, false};
        }
    } else {
        new (&head) std::shared_ptr< node<T> >(new node<T>(val));
        head->ptr = q.insert(q.begin(), head);
        return {{head->ptr, this}, true};
    }
}

template <typename T>
std::shared_ptr< node<T> > set<T>::tree_traversal(std::shared_ptr< node<T> > root, T const& val) const{
    if (val < root->val) {
        if (root->left) {
            return tree_traversal(root->left, val);
        } else {
            return root;
        }
    } else if (root->val < val) {
        if (root->right) {
            return tree_traversal(root->right, val);
        } else {
            return root;
        }
    } else {
        return root;
    }
}

template <typename T>
typename set<T>::iterator set<T>::find(T const& val) const {
    if (!head) {
        return end();
    }
    std::shared_ptr< node<T> > it = tree_traversal(head, val);
    if (val < it->val || it->val < val) {
        return end();
    } else {
        return {it->ptr, this};
    }
}

template <typename T>
typename set<T>::iterator set<T>::lower_bound(T const& val) const {
    if (!head) {
        return end();
    }
    std::shared_ptr< node<T> > it = tree_traversal(head, val);
    if (it->val < val) {
        auto tmp = it->ptr;
        tmp++;
        return {tmp, this};
    } else {
        return {it->ptr, this};
    }
}

template <typename T>
typename set<T>::iterator set<T>::upper_bound(T const& val) const {
    if (!head) {
        return end();
    }
    std::shared_ptr< node<T> > it = tree_traversal(head, val);
    if (val < it->val) {
        return {it->ptr, this};
    } else {
        auto tmp = it->ptr;
        tmp++;
        return {tmp, this};
    }
}

template <typename U>
void swap(set<U> &a, set<U> &b) {
    swap(a.q, b.q);
    swap(a.head, b.head);
}

template <typename T>
set<T>::set(set<T> const& other) : set() {
    if (!other.head) {
        return;
    }
    new (&head) std::shared_ptr< node<T> >(new node<T>(other.head->val));
    head->ptr = q.insert(q.begin(), head);
    tree_copy(head, other.head, other);
}

template <typename T>
set<T>& set<T>::operator=(set<T> const& other) {
    set<T> tmp(other);
    swap(*this, tmp);
    return *this;
}

template <typename T>
void set<T>::tree_copy(std::shared_ptr< node<T> > root, std::shared_ptr< node<T> > const& other_root, set<T> const& other) {
    if (other_root->left) {
        new (&root->left) std::shared_ptr< node<T> >(new node<T>(other_root->left->val));
        root->left->ptr = q.insert(root->ptr, root->left);
        tree_copy(root->left, other_root->left, other);
    }
    if (other_root->right) {
        new (&root->right) std::shared_ptr< node<T> >(new node<T>(other_root->right->val));
        auto tmp = root->ptr;
        tmp++;
        root->right->ptr = q.insert(tmp, root->right);
        tree_copy(root->right, other_root->right, other);
    }
}

template <typename T>
void set<T>::clear() {
    q.clear();
	head = nullptr;
}

template <typename T>
typename set<T>::iterator set<T>::erase(iterator it) {
    if (!head) {
        return end();
    }
    T val = *it;
    std::shared_ptr< node<T> > const me = tree_traversal(head, val);
	auto tmp = me->ptr;
    tmp++;
    if (me->val < *it || *it < me->val || it != me->ptr) {
		if (me->val < val) {
		    return {tmp, this};
		} else {
		    return {me->ptr, this};
		}
    }
    std::shared_ptr< node<T> > par = parent(head, val);
    if (me->left && me->right) {
        std::shared_ptr< node<T> > par2 = parent(head, (*tmp)->val);
        std::shared_ptr< node<T> > next;
        if (par2 == me) {
            next = me->right;
            next->left = me->left;
        } else {
            next = par2->left;
            par2->left = nullptr;
            next->left = me->left;
            next->right = me->right;
        }
        if (!par) {
            head = next;
        } else if (par->left == me) {
            par->left = next;
        } else {
            par->right = next;
        }
    } else if (me->left) {
        if (!par) {
            head = me->left;
        } else if (par->left == me) {
            par->left = me->left;
        } else {
            par->right = me->left;
        }
    } else {
        if (!par) {
            head = me->right;
        } else if (par->left == me) {
            par->left = me->right;
        } else {
            par->right = me->right;
        }
    }
    q.erase(me->ptr);
    return {tmp, this};
}

template <typename T>
std::shared_ptr< node<T> > set<T>::parent(std::shared_ptr< node<T> > root, T const& val) {
    if (val < root->val) {
        if (root->left->val < val || val < root->left->val) {
            return parent(root->left, val);
        }
        return root;
    } else if (root->val < val) {
        if (root->right->val < val || val < root->right->val) {
            return parent(root->right, val);
        }
        return root;
    } else {
        return nullptr;
    }
}

template <typename T>
typename set<T>::iterator set<T>::begin() const {
    return set<T>::iterator(q.begin(), this);
}

template <typename T>
typename set<T>::iterator set<T>::end() const {
    return set<T>::iterator(q.end(), this);
}

template <typename T>
typename set<T>::reverse_iterator set<T>::rbegin() const {
    return std::make_reverse_iterator(end());
}

template <typename T>
typename set<T>::reverse_iterator set<T>::rend() const {
    return std::make_reverse_iterator(begin());
}

#endif // SET_H
