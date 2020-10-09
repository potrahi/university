/**
 * @mainpage Project - Symmetric of Binary Tree 
 * @author Danyil Nikulin - NIK0038
 * @date 16 May 2020
 */

/**
 * @brief Main program file
 * @file binary_tree.cpp
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

/**
 * @brief Used headers
 */
#include "node_tree.h"

/**
 * @brief Defined macros of filenames for test algorithm
 * @def ASYM_1 AsymmetricTree1.txt
 * @def ASYM_1 AsymmetricTree2.txt
 * @def ASYM_1 AsymmetricTree3.txt
 * @def SYM_1 SymmetricTree1.txt
 * @def SYM_2 SymmetricTree2.txt
 * @def SYM_3 SymmetricTree3.txt
 * @def SYM_4 SymmetricTree4.txt
 */
#define ASYM_1 "..\\SymetrieStromu\\AsymmetricTree1.txt"
#define ASYM_2 "..\\SymetrieStromu\\AsymmetricTree2.txt"
#define ASYM_3 "..\\SymetrieStromu\\AsymmetricTree3.txt"
#define SYM_1 "..\\SymetrieStromu\\SymmetricTree1.txt"
#define SYM_2 "..\\SymetrieStromu\\SymmetricTree2.txt"
#define SYM_3 "..\\SymetrieStromu\\SymmetricTree3.txt"
#define SYM_4 "..\\SymetrieStromu\\SymmetricTree4.txt"

/**
 * @fn int main()
 * @brief Main function for using program correct. Function reassign a file pointer into stdin where then it reads by function scanf_s. Chech on -1 position. Fill the vector and call crucial function
 */
int main()
{
    int value;                                              /**< Create storage for value of string */
    std::vector<int> values;                                /**< Create vector for data from text file */
    Tree_Node* tree;                                        /**< Create sturcute of binary tree*/
    FILE* stream;                                           /**< Create buffer for stream */
    freopen_s(&stream, SYM_4, "r", stdin);                  /**< Reassign a file pointer into stdin */
    while(scanf_s("%d", &value) != EOF)                     /**< While values are in stdin, loop will be continue*/
    {
        values.push_back(value);                            /**< Add value of string into vector */
    }
    tree = build_tree(values, 0, values.size() - 1);        /**< Buld binary tree by buil_tree function and save it into created structure */
    bool result = is_symmetric(tree->left, tree->right);    /**< Save result of is_symmetric function */
    print_result(result);                                   /**< Print result of print_result function */
    return 0;
}
