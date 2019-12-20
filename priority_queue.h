#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

class TreeNode;
class Vertex;

class Priority_Queue
{
public:
    Priority_Queue();
    ~Priority_Queue();
    void insert(Vertex*, int);
    Vertex* retrieve();
    void test(TreeNode* = nullptr);
private:
    TreeNode* root;
    TreeNode* sentinel;
    void left_rotation(TreeNode*);
    void right_rotation(TreeNode*);
    void insert_fix(TreeNode*);
    void remove_fix(TreeNode*);
};

#endif // PRIORITY_QUEUE_H
