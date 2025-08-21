#ifndef RBT2_H
#define RBT2_H

using namespace std;
#include<vector>
#include<string>
vector<int>func(int val,int line_number){
    vector<int>result(line_number-1,0);
    result.push_back(val);
    return result;
}
struct stock
{
    string Company;
    stock *left;
    stock *right;
    char colour;
    stock *parent;
    vector<int>quantity;
    vector<int>dynamic_vector;
    stock(string Company, int val,int line_number) : Company(Company), left(nullptr), right(nullptr), colour('R'), parent(nullptr),quantity(func(val,line_number)) {}
};
class RBT2{
    public:
    stock *root;
    bool ll; // Left-Left Rotation flag
    bool rr; // Right-Right Rotation flag
    bool lr; // Left-Right Rotation flag
    bool rl; // Right-Left Rotation flag

    // Function to perform Left Rotation
    stock *rotateLeft(stock *stock)
    {
        struct stock*x = stock->right;
        struct stock*y = x->left;
        x->left = stock;
        stock->right = y;
        stock->parent = x;
        if (y != nullptr)
            y->parent = stock;
        return x;
    }

    // Function to perform Right Rotation
    stock *rotateRight(stock *stock)
    {
        struct stock *x = stock->left;
        struct stock *y = x->right;
        x->right = stock;
        stock->left = y;
        stock->parent = x;
        if (y != nullptr)
            y->parent = stock;
        return x;
    }

    // Helper function for insertion
    stock *insertHelp(stock *root, string Company, int val,int line_number)
    {
        bool f = false; // Flag to check RED-RED conflict

        if (root == nullptr)
            return new stock(Company, val,line_number);
        else if (Company < root->Company)
        {
            root->left = insertHelp(root->left, Company, val,line_number);
            root->left->parent = root;
            if (root != this->root)
            {
                if (root->colour == 'R' && root->left->colour == 'R')
                    f = true;
            }
        }
        else
        {
            root->right = insertHelp(root->right, Company, val,line_number);
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
    RBT2() : root(nullptr), ll(false), rr(false), lr(false), rl(false) {}

    // Function to insert data into the tree
    void insert(string Company, int val,int line_number)
    {
        if (root == nullptr)
        {
            root = new stock(Company, val,line_number);
            root->colour = 'B';

        }
        else
            root = insertHelp(root, Company, val,line_number);
    }
    struct stock *find(string company)
    {
        struct stock *a = root;
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
    void preorderhelper(stock*item,vector<stock*>&result){
        if(item == NULL){
            return;
        }
        result.push_back(item);
        preorderhelper(item->left,result);
        preorderhelper(item->right,result);
    }
    vector<stock*>preorder(){
        vector<stock*>result;
        preorderhelper(root,result);
        return result;
    }
};

#endif