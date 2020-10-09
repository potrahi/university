/**
 * @brief Header file
 * @file node_tree.h
 * @author Danyil Nikulin
 */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

/**
 * @struct Tree_Node
 * @brief A structure of binary tree element
 * @param int data              value of binary tree element
 * @param Tree_Node* left       left child adress of binary tree element
 * @param Tree_Node* right      right child adress of binary tree element
 */
struct Tree_Node {
    int data;
    Tree_Node* left;
    Tree_Node* right;
};

/**
 * @brief Recursively build a binary tree from inputed data
 * @param values is vector of data from input file
 * @param left_bound is index of value in vector
 * @param right_bound is index of last value in vector
 * @return Function return builded binary tree
 */
Tree_Node* build_tree(std::vector<int> values, int left_bound, int right_bound);

/**
 * A normal member taking two arguments and returning a bool value
 * @fn This function will determine tipy of builded binary tree
 * @param current_root a class Tree_Node pointer argument
 * @param pair_root a class Tree_Node pointer argument
 */
bool is_symmetric(Tree_Node* current_root, Tree_Node* pair_root);

/**
 * @brief This function will print actual result into console
 * @param result a bool argument
 */
void print_result(bool result);
