#include<bits/stdc++.h>
#include "LRU.h"
#include "BST.h"
using namespace std;


void createFile(){
    ofstream fout;
    fout.open("animals.init.txt");

    BSTNode node = {"", "", -1, -1};
    fout.write(reinterpret_cast<char *>(&node), sizeof(BSTNode));

    fout.close();
}

int main(){

    // createFile();

    BST tree;
    tree.insert("DOG", "WOOF");
    tree.insert("CAT", "MEOW");
    tree.insert("SNAKE", "HISS");
    tree.insert("PIG", "GRUNT");
    tree.insert("HORSE", "NEIGHS");
    tree.insert("DOG", "MEOW");
    
    tree.inorder();

    cout << tree.search("dog") << endl;
    cout << tree.search("hap") << endl;
    cout << tree.search("tap") << endl;
    
    tree.deleteKey("tap");
    tree.inorder();

    tree.deleteKey("dog");
    tree.deleteKey("tap");
    tree.inorder();
    
    LRU cache(3);

    cout << cache.search("dog") << endl;
    
    cache.add("dog", "woo");
    cache.add("cat", "meow");
    cache.add("bat", "moo");

    cout << cache.search("dog") << endl;

    cache.add("cow", "moo");

    cout << cache.search("cow") << endl;
    cout << cache.search("dog") << endl;

    return 0;
}