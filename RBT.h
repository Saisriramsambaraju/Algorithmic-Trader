#ifndef RBT_H
#define RBT_H


#include<vector>
#include<string>
using namespace std;
struct Node
{
    string Company;
    Node *left;
    Node *right;
    char colour;
    Node *parent;
    int Stockprice;
    int buy_num;
    int sell_num;
    vector<int> buy_price;
    vector<int> sell_price;
    Node(string Company, int val) : Company(Company), Stockprice(val), left(nullptr), right(nullptr), colour('R'), parent(nullptr) {}
};
class RBT
{
public:
    Node *root;
    bool ll; // Left-Left Rotation flag
    bool rr; // Right-Right Rotation flag
    bool lr; // Left-Right Rotation flag
    bool rl; // Right-Left Rotation flag

    // Function to perform Left Rotation
    Node *rotateLeft(Node *node)
    {
        Node *x = node->right;
        Node *y = x->left;
        x->left = node;
        node->right = y;
        node->parent = x;
        if (y != nullptr)
            y->parent = node;
        return x;
    }

    // Function to perform Right Rotation
    Node *rotateRight(Node *node)
    {
        Node *x = node->left;
        Node *y = x->right;
        x->right = node;
        node->left = y;
        node->parent = x;
        if (y != nullptr)
            y->parent = node;
        return x;
    }

    // Helper function for insertion
    Node *insertHelp(Node *root, string Company, int val)
    {
        bool f = false; // Flag to check RED-RED conflict

        if (root == nullptr)
            return new Node(Company, val);
        else if (Company < root->Company)
        {
            root->left = insertHelp(root->left, Company, val);
            root->left->parent = root;
            if (root != this->root)
            {
                if (root->colour == 'R' && root->left->colour == 'R')
                    f = true;
            }
        }
        else
        {
            root->right = insertHelp(root->right, Company, val);
            root->right->parent = root;
            if (root != this->root)
            {
                if (root->colour == 'R' && root->right->colour == 'R')
                    f = true;
            }
        }

        // Perform rotations
        if (ll)
        {
            root = rotateLeft(root);
            root->colour = 'B';
            root->left->colour = 'R';
            ll = false;
        }
        else if (rr)
        {
            root = rotateRight(root);
            root->colour = 'B';
            root->right->colour = 'R';
            rr = false;
        }
        else if (rl)
        {
            root->right = rotateRight(root->right);
            root->right->parent = root;
            root = rotateLeft(root);
            root->colour = 'B';
            root->left->colour = 'R';
            rl = false;
        }
        else if (lr)
        {
            root->left = rotateLeft(root->left);
            root->left->parent = root;
            root = rotateRight(root);
            root->colour = 'B';
            root->right->colour = 'R';
            lr = false;
        }

        // Handle RED-RED conflicts
        if (f)
        {
            if (root->parent->right == root)
            {
                if (root->parent->left == nullptr || root->parent->left->colour == 'B')
                {
                    if (root->left != nullptr && root->left->colour == 'R')
                        rl = true;
                    else if (root->right != nullptr && root->right->colour == 'R')
                        ll = true;
                }
                else
                {
                    root->parent->left->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            }
            else
            {
                if (root->parent->right == nullptr || root->parent->right->colour == 'B')
                {
                    if (root->left != nullptr && root->left->colour == 'R')
                        rr = true;
                    else if (root->right != nullptr && root->right->colour == 'R')
                        lr = true;
                }
                else
                {
                    root->parent->right->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            }
            f = false;
        }
        return root;
    }
    RBT() : root(nullptr), ll(false), rr(false), lr(false), rl(false) {}

    // Function to insert data into the tree
    void insert(string Company, int val)
    {
        if (root == nullptr)
        {
            root = new Node(Company, val);
            root->colour = 'B';
        }
        else
            root = insertHelp(root, Company, val);
    }
    struct Node *find(string company)
    {
        struct Node *a = root;
        while (a != NULL)
        {
            if (company == a->Company)
            {
                return a;
            }
            else if (company > a->Company)
            {
                a = a->right;
            }
            else if (company < a->Company)
            {
                a = a->left;
            }
        }
        return NULL;
    }
    void preorderhelper(Node*item,vector<Node*>&result){
        if(item == NULL){
            return;
        }
        result.push_back(item);
        preorderhelper(item->left,result);
        preorderhelper(item->right,result);
    }
    vector<Node*>preorder(){
        vector<Node*>result;
        preorderhelper(root,result);
        return result;
    }
};


#endif