/**
 * @brief Function implementation file
 * @file node_tree.cpp
 * @author Danyil Nikulin
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "node_tree.h"

 /**
 * @brief Recursively build a binary tree from inputed data
 * @param values is contain data from input file
 * @param left_bound is index of first value in vector
 * @param right_bound is index of last value in vector
 * @return Function return builded binary tree
 */
Tree_Node* build_tree(std::vector<int> values, int left_bound, int right_bound)
{
    if (left_bound > right_bound)                                       /**< If index number of first value more than index number of last value */
        return nullptr;                                                 /**< Than return nullptr */
    Tree_Node* root = new Tree_Node();                                  /**< Create structure that represent one segment of binary tree */
    root->data = values[left_bound];                                    /**< Copy value from vector on first index into data storage in Tree_Node structure */
    int bound = 0;                                                      /**< Create indexer which help to count position of next binary tree segment */
    for (bound = left_bound + 1; bound <= right_bound; bound++)         /**< Loop while indexer won't be less or equals than index number of last element in vector */
    {
        if (root->data < values[bound])                                 /**< If this binary tree element value less than current value from vector */
        {
            break;                                                      /**< End the loop*/
        }
    }
    root->left = build_tree(values, left_bound + 1, bound - 1);         /**< Recursively call current function for create left child */
    root->right = build_tree(values, bound, right_bound);               /**< Recursively call current function for create right child */
    return root;                                                        /**< Return created element of binary tree */
}

/**
 * @brief This function will determine type of builded binary tree
 * @param current_root is a left child of binary tree
 * @param pair_root is a right child of binary tree
 * @return Function will return TRUE or FALSE value
 */
bool is_symmetric(Tree_Node* current_root, Tree_Node* pair_root)
{
    if (current_root == nullptr && pair_root == nullptr)                /**< If left and right child not exist */
        return true;                                                    /**< After return true */

    if (current_root == nullptr || pair_root == nullptr)                /**< If left or right child not exist */
        return false;                                                   /**< After return false */

    if (current_root != nullptr && pair_root != nullptr)                /**< If left and right are exist */
    {
        if (!(is_symmetric(current_root->left, pair_root->right)) ||    /**< And if left child of left child and right child of right child or */
            !(is_symmetric(current_root->right, pair_root->left)))      /**< right child of left child and left child of right child aren't symmetric */
        {
            return false;                                               /**< Than return false */
        }
    }
    return true;                                                        /**< In other cases return true */
}

/**
 * @brief This function will print actual result into console
 * @param result a bool argument
 */
void print_result(bool result)
{
    printf("%s\n", result ? "TRUE -> This binary tree is symmetric" : "FALSE -> This binary tree is not symmetric"); /**< If resilt is true write true message else false message*/
}
