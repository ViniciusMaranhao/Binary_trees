#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <iostream>
#include <string>

class AVLTree {
private:
    struct Node {
        int key;
        int height;
        Node* left;
        Node* right;
        explicit Node(int value) : key(value), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;
    long long rotations = 0;

    int height(Node* node) const { return node ? node->height : 0; }
    int balance(Node* node) const { return node ? height(node->left) - height(node->right) : 0; }
    void updateHeight(Node* node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* middle = x->right;
        x->right = y;
        y->left = middle;
        updateHeight(y);
        updateHeight(x);
        ++rotations;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* middle = y->left;
        y->left = x;
        x->right = middle;
        updateHeight(x);
        updateHeight(y);
        ++rotations;
        return y;
    }

    Node* insert(Node* node, int key, bool& inserted) {
        if (!node) { inserted = true; return new Node(key); }
        if (key < node->key) node->left = insert(node->left, key, inserted);
        else if (key > node->key) node->right = insert(node->right, key, inserted);
        else return node;
        if (!inserted) return node;
        updateHeight(node);
        int b = balance(node);
        if (b > 1 && key < node->left->key) return rotateRight(node);       
        if (b < -1 && key > node->right->key) return rotateLeft(node);      
        if (b > 1 && key > node->left->key) { node->left = rotateLeft(node->left); return rotateRight(node); }  
        if (b < -1 && key < node->right->key) { node->right = rotateRight(node->right); return rotateLeft(node); } 
        return node;
    }

    Node* minimum(Node* node) const { while (node && node->left) node = node->left; return node; }
    Node* remove(Node* node, int key, bool& removed) {
        if (!node) return nullptr;
        if (key < node->key) node->left = remove(node->left, key, removed);
        else if (key > node->key) node->right = remove(node->right, key, removed);
        else {
            removed = true;
            if (!node->left || !node->right) {
                Node* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            Node* successor = minimum(node->right);
            node->key = successor->key;
            bool dummy = false;
            node->right = remove(node->right, successor->key, dummy);
        }
        if (!node) return nullptr;
        updateHeight(node);
        int b = balance(node);
        if (b > 1 && balance(node->left) >= 0) return rotateRight(node);
        if (b > 1 && balance(node->left) < 0) { node->left = rotateLeft(node->left); return rotateRight(node); }
        if (b < -1 && balance(node->right) <= 0) return rotateLeft(node);
        if (b < -1 && balance(node->right) > 0) { node->right = rotateRight(node->right); return rotateLeft(node); }
        return node;
    }

    bool search(Node* node, int key) const {
        if (!node) return false;
        if (key == node->key) return true;
        return key < node->key ? search(node->left, key) : search(node->right, key);
    }
    void inorder(Node* node) const {
        if (!node) return;
        inorder(node->left); std::cout << node->key << ' '; inorder(node->right);
    }
    void print(Node* node, int depth) const {
        if (!node) return;
        print(node->right, depth + 1);
        std::cout << std::string(depth * 4, ' ') << node->key << " (h=" << node->height << ")\n";
        print(node->left, depth + 1);
    }
    bool validate(Node* node, int minKey, int maxKey, int& computedHeight) const {
        if (!node) { computedHeight = 0; return true; }
        if (node->key <= minKey || node->key >= maxKey) return false;
        int lh = 0, rh = 0;
        if (!validate(node->left, minKey, node->key, lh) || !validate(node->right, node->key, maxKey, rh)) return false;
        computedHeight = 1 + std::max(lh, rh);
        return node->height == computedHeight && std::abs(lh - rh) <= 1;
    }
    void clear(Node* node) { if (node) { clear(node->left); clear(node->right); delete node; } }
public:
    AVLTree() = default;
    ~AVLTree() { clear(root); }
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    bool insert(int key) { bool inserted = false; root = insert(root, key, inserted); return inserted; }
    bool remove(int key) { bool removed = false; root = remove(root, key, removed); return removed; }
    bool search(int key) const { return search(root, key); }
    void inorder() const { if (!root) std::cout << "(arvore vazia)"; else inorder(root); std::cout << '\n'; }
    void print() const { if (!root) std::cout << "(arvore vazia)\n"; else print(root, 0); }
    int getHeight() const { return height(root); }
    long long getRotations() const { return rotations; }
    bool isValid() const { int h = 0; return validate(root, -2147483647 - 1, 2147483647, h); }
};

#endif
