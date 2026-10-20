#ifndef RB_TREE_H
#define RB_TREE_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class RBTree {
private:
    enum Color { RED, BLACK };
    struct Node {
        int key;
        Color color;
        Node *left, *right, *parent;
        explicit Node(int value) : key(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };
    Node* root = nullptr;
    long long rotations = 0;

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x; x->parent = y; ++rotations;
    }
    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y; y->parent = x; ++rotations;
    }
    void fixInsert(Node* z) {
        while (z->parent && z->parent->color == RED) {
            Node* p = z->parent;
            Node* g = p->parent;
            if (p == g->left) {
                Node* uncle = g->right;
                if (uncle && uncle->color == RED) {
                    p->color = BLACK; uncle->color = BLACK; g->color = RED; z = g;
                } else {
                    if (z == p->right) { z = p; rotateLeft(z); p = z->parent; g = p->parent; }
                    p->color = BLACK; g->color = RED; rotateRight(g);
                }
            } else {
                Node* uncle = g->left;
                if (uncle && uncle->color == RED) {
                    p->color = BLACK; uncle->color = BLACK; g->color = RED; z = g;
                } else {
                    if (z == p->left) { z = p; rotateRight(z); p = z->parent; g = p->parent; }
                    p->color = BLACK; g->color = RED; rotateLeft(g);
                }
            }
        }
        root->color = BLACK;
    }
    Node* minimum(Node* node) const { while (node && node->left) node = node->left; return node; }
    Color colorOf(Node* node) const { return node ? node->color : BLACK; }
    void transplant(Node* u, Node* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }
    void fixDelete(Node* x, Node* parent) {
        while (x != root && colorOf(x) == BLACK) {
            if (!parent) break;
            if (x == parent->left) {
                Node* w = parent->right;
                if (colorOf(w) == RED) {
                    w->color = BLACK; parent->color = RED; rotateLeft(parent); w = parent->right;
                }
                if (!w) { x = parent; parent = x->parent; continue; }
                if (colorOf(w->left) == BLACK && colorOf(w->right) == BLACK) {
                    w->color = RED; x = parent; parent = x->parent;
                } else {
                    if (colorOf(w->right) == BLACK) {
                        if (w->left) w->left->color = BLACK;
                        w->color = RED; rotateRight(w); w = parent->right;
                    }
                    w->color = parent->color; parent->color = BLACK;
                    if (w->right) w->right->color = BLACK;
                    rotateLeft(parent); x = root; parent = nullptr;
                }
            } else {
                Node* w = parent->left;
                if (colorOf(w) == RED) {
                    w->color = BLACK; parent->color = RED; rotateRight(parent); w = parent->left;
                }
                if (!w) { x = parent; parent = x->parent; continue; }
                if (colorOf(w->right) == BLACK && colorOf(w->left) == BLACK) {
                    w->color = RED; x = parent; parent = x->parent;
                } else {
                    if (colorOf(w->left) == BLACK) {
                        if (w->right) w->right->color = BLACK;
                        w->color = RED; rotateLeft(w); w = parent->left;
                    }
                    w->color = parent->color; parent->color = BLACK;
                    if (w->left) w->left->color = BLACK;
                    rotateRight(parent); x = root; parent = nullptr;
                }
            }
        }
        if (x) x->color = BLACK;
    }
    Node* findNode(int key) const {
        Node* cur = root;
        while (cur) { if (key == cur->key) return cur; cur = key < cur->key ? cur->left : cur->right; }
        return nullptr;
    }
    void inorder(Node* node) const { if (!node) return; inorder(node->left); std::cout << node->key << ' '; inorder(node->right); }
    void print(Node* node, int depth) const {
        if (!node) return;
        print(node->right, depth + 1);
        std::cout << std::string(depth * 4, ' ') << node->key << (node->color == RED ? " (V)\n" : " (P)\n");
        print(node->left, depth + 1);
    }
    int blackHeight(Node* node, bool& ok, int minKey, int maxKey, int& height) const {
        if (!node) { height = 0; return 1; }
        if (node->key <= minKey || node->key >= maxKey) ok = false;
        if (node->color == RED && (colorOf(node->left) == RED || colorOf(node->right) == RED)) ok = false;
        int lh = 0, rh = 0;
        int lb = blackHeight(node->left, ok, minKey, node->key, lh);
        int rb = blackHeight(node->right, ok, node->key, maxKey, rh);
        if (lb != rb) ok = false;
        height = 1 + std::max(lh, rh);
        return lb + (node->color == BLACK ? 1 : 0);
    }
    void clear(Node* node) { if (node) { clear(node->left); clear(node->right); delete node; } }
public:
    RBTree() = default;
    ~RBTree() { clear(root); }
    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;

    bool insert(int key) {
        Node* parent = nullptr; Node* cur = root;
        while (cur) { parent = cur; if (key == cur->key) return false; cur = key < cur->key ? cur->left : cur->right; }
        Node* z = new Node(key); z->parent = parent;
        if (!parent) root = z; else if (key < parent->key) parent->left = z; else parent->right = z;
        fixInsert(z); return true;
    }
    bool search(int key) const { return findNode(key) != nullptr; }
    bool remove(int key) {
        Node* z = findNode(key); if (!z) return false;
        Node* y = z; Color original = y->color; Node* x = nullptr; Node* xParent = nullptr;
        if (!z->left) { x = z->right; xParent = z->parent; transplant(z, z->right); }
        else if (!z->right) { x = z->left; xParent = z->parent; transplant(z, z->left); }
        else {
            y = minimum(z->right); original = y->color; x = y->right;
            if (y->parent == z) { xParent = y; if (x) x->parent = y; }
            else {
                xParent = y->parent; transplant(y, y->right);
                y->right = z->right; y->right->parent = y;
            }
            transplant(z, y); y->left = z->left; y->left->parent = y; y->color = z->color;
        }
        delete z;
        if (original == BLACK) fixDelete(x, x ? x->parent : xParent);
        return true;
    }
    void inorder() const { if (!root) std::cout << "(arvore vazia)"; else inorder(root); std::cout << '\n'; }
    void print() const { if (!root) std::cout << "(arvore vazia)\n"; else print(root, 0); }
    int getHeight() const { bool ok = true; int h = 0; blackHeight(root, ok, -2147483647 - 1, 2147483647, h); return h; }
    long long getRotations() const { return rotations; }
    bool isValid() const {
        if (root && (root->color != BLACK || root->parent != nullptr)) return false;
        bool ok = true; int h = 0; blackHeight(root, ok, -2147483647 - 1, 2147483647, h); return ok;
    }
};

#endif
