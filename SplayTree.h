#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#define SPLAY_DEBUG
#define SPLAY_PORTABLE

template <typename T>
class SplayTree;

template<typename T>
class SplayTreeNode {
public:
    using PtrSplayNode = SplayTreeNode<T>*;
    SplayTreeNode();
    SplayTreeNode(const T& key);
    ~SplayTreeNode();

    T getKey();

#ifndef SPLAY_PORTABLE
    static int getSize(PtrSplayNode);
#endif

    static void Zig(PtrSplayNode);
    static void Zag(PtrSplayNode);
    
    static void ZigZig(PtrSplayNode);
    static void ZigZag(PtrSplayNode);
    static void ZagZig(PtrSplayNode);
    static void ZagZag(PtrSplayNode);

    friend class SplayTree<T>;

private:
    T key;
    PtrSplayNode left;
    PtrSplayNode right;
    PtrSplayNode parent;
#ifndef SPLAY_PORTABLE
    int count;
    int size;
#endif
};

template <typename T>
SplayTreeNode<T>::SplayTreeNode() {}

template <typename T>
SplayTreeNode<T>::SplayTreeNode(const T& key) :
    key(key),
    left(nullptr),
    right(nullptr),
    parent(nullptr)
#ifndef SPLAY_PORTABLE
    ,size(1),
    count(1)
#endif
    {}

template <typename T>
SplayTreeNode<T>::~SplayTreeNode() {}

template <typename T>
T SplayTreeNode<T>::getKey() {
    return key;
}

#ifndef SPLAY_PORTABLE
template <typename T>
int SplayTreeNode<T>::getSize(PtrSplayNode node) {
    return node == nullptr ? 0 : node->size;
}
#endif

template <typename T>
void SplayTreeNode<T>::Zig(PtrSplayNode K1) {
    PtrSplayNode K2 = K1->parent;
    K2->left = K1->right;
    K1->right = K2;
    K1->parent = K2->parent;
    K2->parent = K1;
#ifndef SPLAY_PORTABLE
    K2->size = getSize(K2->left) + getSize(K2->right) + K2->count;
    K1->size = getSize(K1->left) + getSize(K1->right) + K1->count;
#endif
    if (K2->left != nullptr) {
        K2->left->parent = K2;
    }
    if (K1->parent != nullptr) {
        if (K1->parent->right == K2) {
            K1->parent->right = K1;
        } else {
            K1->parent->left = K1;
        }
    }
}

template <typename T>
void SplayTreeNode<T>::Zag(PtrSplayNode K1) {
    PtrSplayNode K2 = K1->parent;
    K2->right = K1->left;
    K1->left = K2;
    K1->parent = K2->parent;
    K2->parent = K1;
#ifndef SPLAY_PORTABLE
    K2->size = getSize(K2->left) + getSize(K2->right) + K2->count;
    K1->size = getSize(K1->left) + getSize(K1->right) + K1->count;
#endif
    if (K2->right != nullptr) {
        K2->right->parent = K2;
    }
    if (K1->parent != nullptr) {
        if (K1->parent->right == K2) {
            K1->parent->right = K1;
        } else {
            K1->parent->left = K1;
        }
    }
}

template <typename T>
void SplayTreeNode<T>::ZigZig(PtrSplayNode node) {
    Zig(node->parent);
    Zig(node);
}

template <typename T>
void SplayTreeNode<T>::ZigZag(PtrSplayNode node) {
    Zag(node);
    Zig(node);
}

template <typename T>
void SplayTreeNode<T>::ZagZig(PtrSplayNode node) {
    Zig(node);
    Zag(node);
}

template <typename T>
void SplayTreeNode<T>::ZagZag(PtrSplayNode node) {
    Zag(node->parent);
    Zag(node);
}

template <typename T>
class SplayTree {
public:
    using PtrSplayNode = SplayTreeNode<T>*;
    SplayTree();
    ~SplayTree();

    void insert(const T& x);
    void remove(const T& x);
    PtrSplayNode find(const T& x);
    PtrSplayNode predecessor(PtrSplayNode node);
    PtrSplayNode successor(PtrSplayNode node);

#ifndef SPLAY_PORTABLE
    T kthElement(int k);
    PtrSplayNode lowerBound(const T& x);
    PtrSplayNode upperBound(const T& x);
    int getRank(const T& x);
#endif

#ifdef SPLAY_DEBUG
    void printTree();
#endif

private:
    PtrSplayNode root;

    void splay(PtrSplayNode node);
    void deleteSubtree(PtrSplayNode node);

#ifndef SPLAY_PORTABLE
    PtrSplayNode realFindKth(int k);
#endif

#ifdef SPLAY_DEBUG
    void printTree(PtrSplayNode node, int depth);
#endif

};

template <typename T>
SplayTree<T>::SplayTree() : root(nullptr) {}

template <typename T>
SplayTree<T>::~SplayTree() {
    deleteSubtree(root);
}

template <typename T>
void SplayTree<T>::deleteSubtree(PtrSplayNode node) {
    if (node == nullptr) {
        return;
    }
    deleteSubtree(node->left);
    deleteSubtree(node->right);
    delete node;
}

template <typename T>
void SplayTree<T>::insert(const T& x) {
    if (root == nullptr) {
        root = new SplayTreeNode<T>(x);
        return;
    }
    PtrSplayNode cur = root, new_node;
    while (true) {
        if (cur->key < x) {
            if (cur->right != nullptr) {
                cur = cur->right;
            } else {
                cur->right = new SplayTreeNode<T>(x);
                new_node = cur->right;
                new_node->parent = cur;
#ifndef SPLAY_PORTABLE
                cur->size++;
#endif
                break;
            }
        } else if (cur->key > x) {
            if (cur->left != nullptr) {
                cur = cur->left;
            } else {
                cur->left = new SplayTreeNode<T>(x);
                new_node = cur->left;
                new_node->parent = cur;
#ifndef SPLAY_PORTABLE
                cur->size++;
#endif
                break;
            }
        } else {
#ifndef SPLAY_PORTABLE
            cur->size++;
            cur->count++;
#endif
            new_node = cur;
            break;
        }
    }
    splay(new_node);
}


template <typename T>
void SplayTree<T>::remove(const T& x) {
    PtrSplayNode position = find(x);
    if (position == nullptr) { // Not found
        return;
    }
    splay(position);

#ifndef SPLAY_PORTABLE
    if (position->count > 1)  {
        position->count--;
        position->size--;
        return;
    }
#endif

    PtrSplayNode pre = predecessor(position);
    if (pre == nullptr) { // There is no left subtree
        root = position->right;
    } else {
        position->left->parent = nullptr;
        splay(pre);
        pre->right = position->right;
#ifndef SPLAY_PORTABLE
        pre->size = SplayTreeNode<T>::getSize(pre->left) + SplayTreeNode<T>::getSize(pre->right) + pre->count;
#endif
    }
    if (position->right != nullptr) {
        position->right->parent = pre;
    }
    delete(position);
}

// Splay the node to top
template <typename T>
void SplayTree<T>::splay(PtrSplayNode node) {
    while (node->parent != nullptr) {
        if (node->parent->parent != nullptr) {
            PtrSplayNode parent_node = node->parent;
            PtrSplayNode gparent_node = parent_node->parent;
            int config = (parent_node->left == node) * 2 + (gparent_node->left == parent_node);
            switch (config) {
                case 0:
                    SplayTreeNode<T>::ZagZag(node);
                    break;
                case 1:
                    SplayTreeNode<T>::ZigZag(node);
                    break;
                case 2:
                    SplayTreeNode<T>::ZagZig(node);
                    break;
                case 3:
                    SplayTreeNode<T>::ZigZig(node);
                    break;
            }
        } else {
            if (node->parent->left == node) {
                SplayTreeNode<T>::Zig(node);
            } else {
                SplayTreeNode<T>::Zag(node);
            }
        }
    }
    root = node;
}

// Yield the pointer to the node which contains key x
// return nullptr if not found 
template <typename T>
typename SplayTree<T>::PtrSplayNode SplayTree<T>::find(const T& x) {
    PtrSplayNode cur = root;
    while (cur != nullptr) {
        if (cur->key > x) {
            cur = cur->left;
        } else if (cur->key < x) {
            cur = cur->right;
        } else {
            break;
        }
    }
    if (cur != nullptr) {
        splay(cur);
    }
    return cur;
}


// Find the precurser of the node
// return nullptr if it doesn't exist
// We assume predecessor(null) = max;
template <typename T>
typename SplayTree<T>::PtrSplayNode SplayTree<T>::predecessor(PtrSplayNode node) {
    if (node == nullptr) {
        PtrSplayNode ans = root;
        if (ans != nullptr) {
            while (ans->right != nullptr) {
                ans = ans->right;
            }
        }
        return ans;
    }
    PtrSplayNode ans = node->left;
    if (ans != nullptr) { 
        while (ans->right != nullptr) {
            ans = ans->right;
        }
    } else {
        ans = node->parent;
        while (ans != nullptr && ans->left == node) {
            node = ans;
            ans = ans->parent;
        }
    }
    return ans;
}

// Find the postcurser of the node
// return nullptr if it doesn't exist
// We assume successor(null) = min
template <typename T>
typename SplayTree<T>::PtrSplayNode SplayTree<T>::successor(PtrSplayNode node) {
    if (node == nullptr) {
        PtrSplayNode ans = root;
        if (ans != nullptr) {
            while (ans->left != nullptr) {
                ans = ans->left;
            }
        }
        return ans;
    }
    PtrSplayNode ans = node->right;
    if (ans != nullptr) {
        while (ans->left != nullptr) {
            ans = ans->left;
        }
    } else {
        ans = node->parent;
        while (ans != nullptr && ans->right == node) {
            node = ans;
            ans = ans->parent;
        }
    }
    return ans;
}


#ifndef SPLAY_PORTABLE
template <typename T>
int SplayTree<T>::getRank(const T& x) {
    int rank = 0;
    PtrSplayNode cur = root;
    // Check for Memory leak!
    while (cur != nullptr) {
        if (cur->key > x) {
            cur = cur->left;
        } else if (cur->key < x) {
            rank += SplayTreeNode<T>::getSize(cur->left) + cur->count;
            cur = cur->right;
        } else {
            break;
        }
    }
    if (cur != nullptr) {
        rank += SplayTreeNode<T>::getSize(cur->left) + 1;
        splay(cur);
        return rank;
    } else {
        return -1;
    }
}

template <typename T>
typename SplayTree<T>::PtrSplayNode SplayTree<T>::lowerBound(const T& x) {
    PtrSplayNode cur = root, ans = nullptr;
    while (cur != nullptr) {
        if (cur->key < x) {
            cur = cur->right;
        } else {
            ans = cur;
            cur = cur->left;
        }
    }
    if (ans != nullptr) {
        splay(ans);
    }
    return ans;
}

template <typename T>
typename SplayTree<T>::PtrSplayNode SplayTree<T>::upperBound(const T& x) {
    PtrSplayNode cur = root, ans = nullptr;
    while (cur != nullptr) {
        if (cur->key <= x) {
            cur = cur->right;
        } else {
            ans = cur;
            cur = cur->left;
        }
    }
    if (ans != nullptr) {
        splay(ans);
    }
    return ans;
}

// Wrapper for realFindKth
template <typename T>
T SplayTree<T>::kthElement(int k) {
    auto node = realFindKth(k);
    if (node == nullptr) {
        return -1;
    } else {
        return realFindKth(k)->key;
    }
}

// Find the Kth element
// return null if K is illegal
template <typename T>
typename SplayTree<T>::PtrSplayNode SplayTree<T>::realFindKth(int k) {
    PtrSplayNode cur = root;
    while (cur != nullptr) {
        if (SplayTreeNode<T>::getSize(cur->left) >= k) {
            cur = cur->left;
        } else if (SplayTreeNode<T>::getSize(cur->left) + cur->count < k) {
            k -= SplayTreeNode<T>::getSize(cur->left) + cur->count;
            cur = cur->right;
        } else {
            break;
        }
    }
    if (cur != nullptr) {
        splay(cur);
    }
    return cur;
}
#endif

#ifdef SPLAY_DEBUG
template <typename T>
void SplayTree<T>::printTree() {
    std::cout << "============================" << std::endl;
    printTree(root, 2);
}

template <typename T>
void SplayTree<T>::printTree(PtrSplayNode node, int depth) {
    if (node == nullptr) {
        return;
    }
    printTree(node->left, depth + 2);
    for (int i = 1; i <= depth; i++) {
        std::cout << '-';
    }
    std::cout << ' ' << node->key
#ifndef SPLAY_PORTABLE
        << " : " << node->count
#endif
        << std::endl;
    printTree(node->right, depth + 2);
}
#endif


#endif // SPLAYTREE_H