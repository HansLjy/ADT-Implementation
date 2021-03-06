#ifndef SPLAYTREE_H
#define SPLAYTREE_H

class SplayTreeNode {
public:
    using PtrSplayNode = SplayTreeNode*;
    SplayTreeNode();
    SplayTreeNode(const int& key);
    ~SplayTreeNode();

    int getKey();

    static int getSize(PtrSplayNode);

    static void Zig(PtrSplayNode);
    static void Zag(PtrSplayNode);
    
    static void ZigZig(PtrSplayNode);
    static void ZigZag(PtrSplayNode);
    static void ZagZig(PtrSplayNode);
    static void ZagZag(PtrSplayNode);

    friend class SplayTree;

private:
    int key;
    PtrSplayNode left;
    PtrSplayNode right;
    PtrSplayNode parent;
    int count;
    int size;
};

SplayTreeNode::SplayTreeNode() {}
SplayTreeNode::SplayTreeNode(const int& key) : key(key), left(nullptr), right(nullptr), parent(nullptr), size(1), count(1) {}
SplayTreeNode::~SplayTreeNode() {}

int SplayTreeNode::getKey() {
    return key;
}

int SplayTreeNode::getSize(PtrSplayNode node) {
    return node == nullptr ? 0 : node->size;
}

void SplayTreeNode::Zig(PtrSplayNode K1) {
    PtrSplayNode K2 = K1->parent;
    K2->left = K1->right;
    K1->right = K2;
    K1->parent = K2->parent;
    K2->parent = K1;
    K2->size = getSize(K2->left) + getSize(K2->right) + K2->count;
    K1->size = getSize(K1->left) + getSize(K1->right) + K1->count;
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

void SplayTreeNode::Zag(PtrSplayNode K1) {
    PtrSplayNode K2 = K1->parent;
    K2->right = K1->left;
    K1->left = K2;
    K1->parent = K2->parent;
    K2->parent = K1;
    K2->size = getSize(K2->left) + getSize(K2->right) + K2->count;
    K1->size = getSize(K1->left) + getSize(K1->right) + K1->count;
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

void SplayTreeNode::ZigZig(PtrSplayNode node) {
    Zig(node->parent);
    Zig(node);
}

void SplayTreeNode::ZigZag(PtrSplayNode node) {
    Zag(node);
    Zig(node);
}

void SplayTreeNode::ZagZig(PtrSplayNode node) {
    Zig(node);
    Zag(node);
}

void SplayTreeNode::ZagZag(PtrSplayNode node) {
    Zag(node->parent);
    Zag(node);
}

class SplayTree {
public:
    using PtrSplayNode = SplayTreeNode*;
    SplayTree();
    ~SplayTree();

    void insert(const int& x);
    void remove(const int& x);
    PtrSplayNode find(const int& x);
    int kthElement(int k);
    PtrSplayNode lowerBound(const int& x);
    PtrSplayNode upperBound(const int& x);
    PtrSplayNode predecessor(PtrSplayNode node);
    PtrSplayNode successor(PtrSplayNode node);
    int getRank(const int& x);
    void printTree();

private:
    PtrSplayNode root;

    void splay(PtrSplayNode node);
    PtrSplayNode realFindKth(int k);
    void printTree(PtrSplayNode node, int depth);

};

SplayTree::SplayTree() : root(nullptr) {}
SplayTree::~SplayTree() {}

void SplayTree::insert(const int& x) {
    if (root == nullptr) {
        root = new SplayTreeNode(x);
        return;
    }
    PtrSplayNode cur = root, new_node;
    while (true) {
        if (cur->key < x) {
            if (cur->right != nullptr) {
                cur = cur->right;
            } else {
                cur->right = new SplayTreeNode(x);
                new_node = cur->right;
                new_node->parent = cur;
                cur->size++;
                break;
            }
        } else if (cur->key > x) {
            if (cur->left != nullptr) {
                cur = cur->left;
            } else {
                cur->left = new SplayTreeNode(x);
                new_node = cur->left;
                new_node->parent = cur;
                cur->size++;
                break;
            }
        } else {
            cur->size++;
            cur->count++;
            new_node = cur;
            break;
        }
    }
    splay(new_node);
}

// Splay the node to top
void SplayTree::splay(PtrSplayNode node) {
    while (node->parent != nullptr) {
        if (node->parent->parent != nullptr) {
            PtrSplayNode parent_node = node->parent;
            PtrSplayNode gparent_node = parent_node->parent;
            int config = (parent_node->left == node) * 2 + (gparent_node->left == parent_node);
            switch (config) {
                case 0:
                    SplayTreeNode::ZagZag(node);
                    break;
                case 1:
                    SplayTreeNode::ZigZag(node);
                    break;
                case 2:
                    SplayTreeNode::ZagZig(node);
                    break;
                case 3:
                    SplayTreeNode::ZigZig(node);
                    break;
            }
        } else {
            if (node->parent->left == node) {
                SplayTreeNode::Zig(node);
            } else {
                SplayTreeNode::Zag(node);
            }
        }
    }
    root = node;
}

// Yield the pointer to the node which contains key x
// return nullptr if not found 
SplayTree::PtrSplayNode SplayTree::find(const int& x) {
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

int SplayTree::getRank(const int& x) {
    int rank = 0;
    PtrSplayNode cur = root;
    // Check for Memory leak!
    while (cur != nullptr) {
        if (cur->key > x) {
            cur = cur->left;
        } else if (cur->key < x) {
            rank += SplayTreeNode::getSize(cur->left) + cur->count;
            cur = cur->right;
        } else {
            break;
        }
    }
    if (cur != nullptr) {
        rank += SplayTreeNode::getSize(cur->left) + 1;
        splay(cur);
        return rank;
    } else {
        return -1;
    }
}

SplayTree::PtrSplayNode SplayTree::lowerBound(const int& x) {
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

SplayTree::PtrSplayNode SplayTree::upperBound(const int& x) {
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

// Find the precurser of the node
// return nullptr if it doesn't exist
// We assume predecessor(null) = max;
SplayTree::PtrSplayNode SplayTree::predecessor(PtrSplayNode node) {
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
SplayTree::PtrSplayNode SplayTree::successor(PtrSplayNode node) {
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

void SplayTree::remove(const int& x) {
    PtrSplayNode position = find(x);
    if (position == nullptr) { // Not found
        return;
    }
    splay(position);
    if (position->count > 1) {
        position->count--;
        position->size--;
        return;
    }

    PtrSplayNode pre = predecessor(position);
    if (pre == nullptr) { // There is no left subtree
        root = position->right;
    } else {
        position->left->parent = nullptr;
        splay(pre);
        pre->right = position->right;
        pre->size = SplayTreeNode::getSize(pre->left) + SplayTreeNode::getSize(pre->right) + pre->count;
    }
    if (position->right != nullptr) {
        position->right->parent = pre;
    }
    delete(position);
}

// Wrapper for realFindKth
int SplayTree::kthElement(int k) {
    auto node = realFindKth(k);
    if (node == nullptr) {
        return -1;
    } else {
        return realFindKth(k)->key;
    }
}

// Find the Kth element
// return null if K is illegal
SplayTree::PtrSplayNode SplayTree::realFindKth(int k) {
    PtrSplayNode cur = root;
    while (cur != nullptr) {
        if (SplayTreeNode::getSize(cur->left) >= k) {
            cur = cur->left;
        } else if (SplayTreeNode::getSize(cur->left) + cur->count < k) {
            k -= SplayTreeNode::getSize(cur->left) + cur->count;
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

void SplayTree::printTree() {
    std::cout << "============================" << std::endl;
    printTree(root, 2);
}

void SplayTree::printTree(PtrSplayNode node, int depth) {
    if (node == nullptr) {
        return;
    }
    printTree(node->left, depth + 2);
    for (int i = 1; i <= depth; i++) {
        std::cout << '-';
    }
    std::cout << ' ' << node->key << " : " << node->count << std::endl;
    printTree(node->right, depth + 2);
}


#endif // SPLAYTREE_H