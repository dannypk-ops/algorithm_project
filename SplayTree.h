#pragma once

#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "Processing_Tree.h"
#include <vector>

struct splay {
    KEY_TYPE key;
    vector<node> items;
    struct splay *lchild;
    struct splay *rchild;
};

class SplayTree {
public:
    splay *Insert(KEY_TYPE key, splay *root, node node);

    splay *Delete(KEY_TYPE key, splay *root);

    splay *Search(KEY_TYPE key, splay *root);

    void InOrder(splay *root);

private:
    inline splay *RR_Rotate(splay *k2);

    inline splay *LL_Rotate(splay *k2);

    splay *Splay(KEY_TYPE key, splay *root);

    splay *New_Node(KEY_TYPE key, node node);

};

#endif