#include "priority_queue.h"
#include <treenode.h>

#include <queue>

Priority_Queue::Priority_Queue() : sentinel(new TreeNode())
{
    sentinel->color = false;
    root = sentinel;
    root->parent = sentinel;
}

Priority_Queue::~Priority_Queue(){
    std::queue<TreeNode*> q;
    if(root!=sentinel)
    {
        q.push(this->root);
        while (!q.empty()){

            TreeNode* current = q.front();
            q.pop();

            if (current->leftchild != sentinel)
                q.push(current->leftchild);
            if (current->rightchild != sentinel)
                q.push(current->rightchild);

            delete current;
        }
    }
    delete sentinel;
}

void
Priority_Queue::insert(Vertex* vertex, int distance)
{
    TreeNode* parent{sentinel};
    TreeNode* current{root};
    TreeNode* node{new TreeNode(vertex, distance)};

    // find suitable situation to store the new node
    while (current != sentinel) {
        parent = current;
        current = ( node->distance < current->distance )?
            current->leftchild : current->rightchild;
    }

    node->parent = parent;

    if (parent == sentinel)
        this->root = node;

    else if ( node->distance < parent->distance )
        parent->leftchild = node;
    else
        parent->rightchild = node;

    node->leftchild = sentinel;
    node->rightchild = sentinel;

    insert_fix(node);
}

void
Priority_Queue::left_rotation(TreeNode* target_parent)
{
    TreeNode* target{target_parent->rightchild};

    target_parent->rightchild = target->leftchild;

    if (target->leftchild != sentinel){
        target->leftchild->parent = target_parent;
    }

    target->parent = target_parent->parent;

    if (target_parent->parent == sentinel){
        root = target;
    }
    else if (target_parent == target_parent->parent->leftchild){
        target_parent->parent->leftchild = target;
    }
    else{
        target_parent->parent->rightchild = target;
    }
    target->leftchild = target_parent;
    target_parent->parent = target;
}

void
Priority_Queue::right_rotation(TreeNode* target_parent)
{
    TreeNode *target{target_parent->leftchild};
    target_parent->leftchild = target->rightchild;

    if (target->rightchild != sentinel)
        target->rightchild->parent = target_parent;

    target->parent = target_parent->parent;

    if (target_parent->parent == sentinel)
        root = target;

    else if (target_parent == target_parent->parent->leftchild)
        target_parent->parent->leftchild = target;

    else
        target_parent->parent->rightchild = target;

    target->rightchild = target_parent;
    target_parent->parent = target;

}

Vertex*
Priority_Queue::retrieve(){
    if(root==sentinel) return nullptr;
    TreeNode* node{root};
    while(node->leftchild!=sentinel)
        node = node->leftchild;
    TreeNode *child{
                (node->leftchild != sentinel)?
                 node->leftchild:node->rightchild
             };
    child->parent = node->parent;
    if(node->parent==sentinel) root = child;
    else node->parent->leftchild = child;
    if(!node->color) remove_fix(child);
    Vertex* vertex{node->vertex};
    delete node;
    return vertex;
}

void
Priority_Queue::insert_fix(TreeNode* current)
{
    while (current->parent->color) {
        if (current->parent == current->parent->parent->leftchild) {
            TreeNode *uncle{current->parent->parent->rightchild};
            // uncle is red
            if (uncle->color) {
                current->parent->color = false;
                uncle->color = false;
                current->parent->parent->color = true;
                current = current->parent->parent;
            }
            // uncle is black
            else {
                if (current == current->parent->rightchild){
                    // make current be leftchild
                    current = current->parent;
                    left_rotation(current);
                }
                // current is leftchild
                current->parent->color = false;
                current->parent->parent->color = true;
                right_rotation(current->parent->parent);
            }
        }

        // oppositie operation
        else {
            TreeNode *uncle{current->parent->parent->leftchild};
            // uncle is red
            if (uncle->color) {
                current->parent->color = false;
                uncle->color = false;
                current->parent->parent->color = true;
                current = current->parent->parent;
            }
            // uncle is black
            else {
                if (current == current->parent->leftchild) {
                    // make current be rightchild
                    current = current->parent;
                    right_rotation(current);
                }
                // current is leftchild
                current->parent->color = false;
                current->parent->parent->color = true;
                left_rotation(current->parent->parent);
            }
        }
    }
    // ensure root is black
    root->color = false;
}

void
Priority_Queue::remove_fix(TreeNode* current){
    while (current != root && !current->color ) {
        TreeNode *sibling{current->parent->rightchild};
        // sibling is red
        if (sibling->color) {
            sibling->color = false;
            current->parent->color = true;
            left_rotation(current->parent);
            sibling = current->parent->rightchild;
        }
        // sibling is black
        // sibling childs are black
        if (!(sibling->leftchild->color||sibling->rightchild->color)) {
            sibling->color = true;
            current = current->parent;
        }
        // at most 1 child is black
        else {
            // right child is black, then left child must be red
            if (!sibling->rightchild->color){
                sibling->leftchild->color = false;
                sibling->color = true;
                right_rotation(sibling);
                sibling = current->parent->rightchild;
                // sibling right child must be red now
            }
            // right child is red
            sibling->color = current->parent->color;
            current->parent->color = false;
            sibling->rightchild->color = false;
            left_rotation(current->parent);
            current = root;
        }
    }
    current->color = false;
}
