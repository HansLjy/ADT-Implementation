#include <queue>
#include <iostream>

template <typename T, int Order>
class BPlusTree;

// Order: maximum number of childs
template <typename T, int Order>
class BPlusNode {
public:
    friend class BPlusTree<T, Order>;
protected:
    using PtrNode = BPlusNode*;
    bool leaf;
    int n;          // Number of keys in the node
    T key[Order + 2];
    PtrNode child[Order + 2];
    // child[i] < key[i] <= child[i + 1] < key[i + 1]
};

template <typename T, int Order>
class BPlusTree {
public:
    using PtrNode = BPlusNode<T, Order>*;
    BPlusTree();

    void insert(int x);
    bool find(int x);
    void printTree();

private:
    PtrNode root;

    bool realInsert(int x, PtrNode cur);
    void splitChild(PtrNode cur, int pos);
};

template <typename T, int Order>
BPlusTree<T, Order>::BPlusTree() {
    root = new BPlusNode<T, Order>;
    root->n = 0;
    root->leaf = 1;
}

template <typename T, int Order>
void BPlusTree<T, Order>::insert(int x) {
    bool split_root = realInsert(x, root);
    if (split_root) {
        PtrNode new_root = new BPlusNode<T, Order>;
        new_root->n = 0;
        new_root->leaf = false;
        new_root->child[1] = root;
        splitChild(new_root, 1);
        root = new_root;
    }
}

template <typename T, int Order>
bool BPlusTree<T, Order>::find(int x) {
    PtrNode cur = root;
    while (!cur->leaf) {
        int pos;
        for (pos = 1; pos <= cur->n; pos++) {
            if (cur->key[pos] > x) {
                break;
            }
        }
        cur = cur->child[pos];
    }
    for (int i = 1; i <= cur->n; i++) {
        if (cur->key[i] == x) {
            return true;
        }
    }
    return false;
}

template <typename T, int Order>
bool BPlusTree<T, Order>::realInsert(int x, PtrNode cur) {
    if (cur->leaf) {
        // If it is a leaf, simply insert it
        int pos;
        for (pos = 1; pos <= cur->n; pos++) {
            if (cur->key[pos] > x) {
                break;
            }
        }
        for (int i = cur->n; i >= pos; i--) {
            cur->key[i + 1] = cur->key[i];
        }
        cur->key[pos] = x;
        cur->n++;
    } else {
        int pos;
        for (pos = 1; pos <= cur->n; pos++) {
            if (cur->key[pos] > x) {
                break;
            }
        }
        bool split = realInsert(x, cur->child[pos]);
        if (split) {
            splitChild(cur, pos);
        }
    }
    if ((cur->leaf && cur->n == Order + 1) || (!cur->leaf && cur->n == Order)) {
        return true;
    } else {
        return false;
    }
}

template <typename T, int Order>
void BPlusTree<T, Order>::splitChild(PtrNode cur, int pos) {
    int mid = (Order + 1) / 2;
    // For leaf
    //          keys are partitioned into [1, mid], [mid + 1, Order + 1];
    // For internal node
    //          keys are partitioned into [1, mid - 1], [mid + 1, Order]
    //          children are partitioned into [1, mid], [mid + 1, Order + 1]
    //          the middle key will go into the parent

    PtrNode node_to_split = cur->child[pos];
    PtrNode new_node = new BPlusNode<T, Order>;
    new_node->leaf = node_to_split->leaf;
    
    if (node_to_split->leaf) {
        for (int i = mid + 1; i <= Order + 1; i++) {
            new_node->key[i - mid] = node_to_split->key[i];
        }
        new_node->n = Order + 1 - mid;
        node_to_split->n = mid;
    } else {
        for (int i = mid + 1; i <= Order; i++) {
            new_node->key[i - mid] = node_to_split->key[i];
        }
        for (int i = mid + 1; i <= Order + 1; i++) {
            new_node->child[i - mid] = node_to_split->child[i];
        }
        new_node->n = Order - mid;
        node_to_split->n = mid - 1;
    }
    for (int i = cur->n; i >= pos; i--) {
        cur->key[i + 1] = cur->key[i];
    }
    for (int i = cur->n + 1; i > pos; i--) {
        cur->child[i + 1] = cur->child[i];
    }
    (cur->n)++;
    cur->key[pos] = node_to_split->leaf ? new_node->key[1] : node_to_split->key[mid];
    cur->child[pos + 1] = new_node;
}

template <typename T, int Order>
void BPlusTree<T, Order>::printTree() {
    std::queue<PtrNode> Q;
    int next_level_remain = 0;
    int cur_level_remain = 1;
    Q.push(root);
    while (!Q.empty()) {
        PtrNode cur = Q.front();
        Q.pop();
        cur_level_remain--;
        std::cout << '[';
        for (int i = 1; i < cur->n; i++) {
            std::cout << cur->key[i] << ',';
        }
        if (cur->n) {
            std::cout << cur->key[cur->n] << ']';
        }
        if (!cur->leaf) {
            for (int i = 1; i <= cur->n + 1; i++) {
                Q.push(cur->child[i]);
                next_level_remain++;
            }
        }
        if (!cur_level_remain) {
            std::cout << std::endl;
            cur_level_remain = next_level_remain;
            next_level_remain = 0;
        }
    }
}