#include<fstream>
#include<string.h>
#include<iostream>
using namespace std;

struct BSTNode
{
    char key[20], val[20];
    int left, right;

    BSTNode(string _key = "", string _val = "", int _left = -1, int _right = -1){
        strcpy(key, _key.c_str());
        strcpy(val, _val.c_str());
        left = _left;
        right = _right;
    }
};


class BST{
private:
    string file;
    BSTNode* root;

    BSTNode* readNode(int pos){
        if(pos == -1){
            return nullptr;
        }

        ifstream fin;
        fin.open(file);

        if(!fin){
            cerr << "File not found" << endl;
            return nullptr;
        }

        BSTNode *node = new BSTNode();

        fin.seekg(pos);
        fin.read(reinterpret_cast<char *>(node), sizeof(BSTNode));

        fin.close();
        return node;
    }

    int writeNode(BSTNode* node){
        ofstream fout;
        fout.open(file, ios::app);

        int pos = fout.tellp();
        fout.write(reinterpret_cast<char *>(node), sizeof(BSTNode));

        fout.close();
        return pos;
    }

    void updateNode(int pos, BSTNode* node){
        fstream fout;
        fout.open(file);

        fout.seekp(pos);
        fout.write(reinterpret_cast<char *>(node), sizeof(BSTNode));

        fout.close();
    }

    void deleteNode(int pos){
        fstream fout;
        fout.open(file);

        BSTNode *node = new BSTNode();

        fout.seekp(pos);
        fout.write(reinterpret_cast<char *>(node), sizeof(BSTNode));

        fout.close();
    }

    void viewNode(BSTNode* node){
        cout << "[" << node->key << " : " << node->val << " : "  << node->left << " : " << node->right << "], ";
    }

    bool isLeaf(BSTNode* node){
        return node->left == -1 && node->right == -1;
    }

    void inorder(int nodePos){
        BSTNode *node = readNode(nodePos);
        if(node != nullptr){
            inorder(node->left);
            viewNode(node);
            inorder(node->right);
        }
    }
public:
    BST(string _file = "animals.txt"){
        // first node keeps the address of root
        file = _file;
        root = readNode(0);
        cout << root->left << endl;
    }

    void insert(string key, string val){
        if(root->left == -1){//BST is empty
            BSTNode *node = new BSTNode(key, val);

            root->left = writeNode(node);
            updateNode(0, root);

            return;
        }
        BSTNode *parent = root, *node = readNode(root->left);
        int parentPos = 0, nodePos = root->left;

        while(node != nullptr && node->key != key){
            parent = node;
            parentPos = nodePos;

            nodePos = (key < node->key) ? node->left : node->right;

            node = readNode(nodePos);
        }
        if(node == nullptr){
            node = new BSTNode(key, val);

            nodePos = writeNode(node);

            if(key < parent->key){
                parent->left = nodePos;
            }else{
                parent->right = nodePos;
            }
            updateNode(parentPos, parent);
        }else{
            cout << key << " already present in BST" << endl;
        }
    }

    string search(string key){
        BSTNode *node = readNode(root->left);
        int nodePos = root->left;

        while(node != nullptr && node->key != key){
            nodePos = (key < node->key) ? node->left : node->right;
            node = readNode(nodePos);
        }
        if(node != nullptr){
            return node->val;
        }else{
            return key + " not found";
        }
    }
    
    void deleteKey(string key) {
        BSTNode *parent = root, *node = readNode(root->left);
        int parentPos = 0, nodePos = root->left;

        while(node != nullptr && node->key != key){
            parentPos = nodePos;
            parent = node;
            
            nodePos = (key < node->key) ? node->left : node->right;
            node = readNode(nodePos);
        }
        if(node){
            if(isLeaf(node)){
                if(parentPos == 0)root->left = -1;
                else if(nodePos == parent->left)parent->left = -1;
                else if(nodePos == parent->right)parent->right = -1;
            }else{
                if(node->left != -1){
                    if(parentPos == 0)root->left = node->left;
                    else if(nodePos == parent->left)parent->left = node->left;
                    else if(nodePos == parent->right)parent->right = node->left;
                    
                    int leftRightMostPos = node->left;
                    BSTNode* leftRightMost = readNode(leftRightMostPos);
                    
                    while(leftRightMost->right != -1){
                        leftRightMostPos = leftRightMost->right;
                        leftRightMost = readNode(leftRightMostPos);
                    }
                    leftRightMost->right = node->right;

                    updateNode(leftRightMostPos, leftRightMost);
                }else{
                    if(parentPos == 0)root->left = node->right;
                    else if(nodePos == parent->left)parent->left = node->right;
                    else if(nodePos == parent->right)parent->right = node->right;
                }
            }

            updateNode(0, root);
            updateNode(parentPos, parent);

            // deleteNode(nodePos);
        }
    }

    void inorder(){
        cout << "Inorder : ";
        inorder(root->left);
        cout << endl;
    }
};