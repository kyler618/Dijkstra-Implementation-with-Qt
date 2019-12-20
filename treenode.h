#ifndef TREENODE_H
#define TREENODE_H

#include <QPair>

class Priority_Queue;
class Vertex;

class TreeNode
{
public:
    TreeNode();
    TreeNode(Vertex*, int);
private:
    TreeNode *leftchild{nullptr};
    TreeNode *rightchild{nullptr};
    TreeNode *parent{nullptr};
    Vertex* vertex{nullptr};
    int distance{-1};
    bool color{true}; // true : red ; false : black ;
    friend Priority_Queue;
};

#endif // TREENODE_H
