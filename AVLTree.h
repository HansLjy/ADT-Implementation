#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <algorithm>
#include <string>
using std::string;
using std::cout;
using std::endl;
using std::max;
using std::min;

namespace AVLTreeSpace {
    class AVLTreeNode {
        using PtrAVLNode = AVLTreeNode*;

    public:
        AVLTreeNode() {}
        AVLTreeNode(const int& key) : key(key), left(nullptr), right(nullptr), height(-1) {}
        ~AVLTreeNode() {}

        friend class AVLTree;
        friend AVLTreeNode* SingleRotationWithLeft(AVLTreeNode*);
        friend AVLTreeNode* SingleRotationWithRight(AVLTreeNode*);
        friend AVLTreeNode* DoubleRotationWithLeft(AVLTreeNode*);
        friend AVLTreeNode* DoubleRotationWithRight(AVLTreeNode*);
        friend int height(AVLTreeNode*);

    private:
        int key;
        PtrAVLNode left;
        PtrAVLNode right;
        int height;
        // int count;
    };

    inline int height(AVLTreeNode* ptr_node) {
        if (ptr_node == nullptr) {
            return -1;
        }
        return ptr_node->height;
    }

    class AVLTree {
        using PtrAVLNode = AVLTreeNode*;

    public:
        AVLTree() : root(nullptr) {}
        ~AVLTree() {}

        void insert(const int& x) {
            root = realInsert(x, root);
        }

        int getTop() {
            return root->key;
        }

        void printTree() {
            realPrintTree(root, 2);
        }

    private:
        PtrAVLNode root;

        void realPrintTree(PtrAVLNode node, int depth) {
            if (node == nullptr) {
                return;
            }
            realPrintTree(node->left, depth + 2);
            for (int i = 1; i <= depth; i++) {
                cout << '-';
            }
            cout << ' ' << node->key << endl;
            realPrintTree(node->right, depth + 2);
        }

        PtrAVLNode realInsert(const int& x, PtrAVLNode node) {
            if (node == nullptr) {
                node = new AVLTreeNode;
                node->key = x;
                node->left = node->right = nullptr;
                node->height = 0;
                return node;
            }
            if (node->key > x) {
                node->left = realInsert(x, node->left);
                if (height(node->left) > height(node->right) + 1) {
                    // Rebalance
                    if (x < node->left->key) {
                        return SingleRotationWithLeft(node);
                    } else {
                        return DoubleRotationWithLeft(node);
                    }
                    // Note : there is no way that x == node->left->key
                }
            } else if (node->key < x) {
                node->right = realInsert(x, node->right);
                if (height(node->right) > height(node->left) + 1) {
                    if (x > node->right->key) {
                        return SingleRotationWithRight(node);
                    } else {
                        return DoubleRotationWithRight(node);
                    }
                }
            } else {
                // Update Something
                return node;
            }
            node->height = max(height(node->left), height(node->right)) + 1;
            return node;
        }
    };

    AVLTreeNode* SingleRotationWithLeft(AVLTreeNode* K1) {
        AVLTreeNode* K2 = K1->left;
        K1->left = K2->right;
        K2->right = K1;
        K1->height = max(height(K1->left), height(K1->right)) + 1;
        K2->height = max(height(K2->left), height(K2->right)) + 1;
        return K2;
    }
    AVLTreeNode* SingleRotationWithRight(AVLTreeNode* K1) {
        AVLTreeNode* K2 = K1->right;
        K1->right = K2->left;
        K2->left = K1;
        K1->height = max(height(K1->left), height(K1->right)) + 1;
        K2->height = max(height(K2->left), height(K2->right)) + 1;
        return K2;
    }
    AVLTreeNode* DoubleRotationWithLeft(AVLTreeNode* K1) {
        K1->left = SingleRotationWithRight(K1->left);
        return SingleRotationWithLeft(K1);
    }
    AVLTreeNode* DoubleRotationWithRight(AVLTreeNode* K1) {
        K1->right = SingleRotationWithLeft(K1->right);
        return SingleRotationWithRight(K1);
    }
}

#endif  // AVLTREE_H