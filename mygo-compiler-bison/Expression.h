/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include <iostream>
using namespace std;

/**
 * @brief The operation type
 */
typedef enum tagEOperationType
{
    eVALUE,
    eFLOAT,
    eMULTIPLY,
    ePLUS,
    eMINUS,
    eDIVIDE,
    eDEKLERATION,
    eIDENTIFIER
} EOperationType;

/**
 * @brief The expression structure
 */
typedef struct tagSExpression
{
    EOperationType type;///< type of operation

    int value;///< valid only when type is eVALUE
    float fvalue;///< valid only when type is eFLOAT or eDEKLERATION
    string svalue;/// valid only when type is eDEKLERATION
    struct tagSExpression *left; ///< left side of the tree
    struct tagSExpression *right;///< right side of the tree
} SExpression;

/**
 * @brief It creates an identifier
 * @param value The number value
 * @return The expression or NULL in case of no memory
 */
SExpression *createNumber(int value);


/**
 * @brief It creates an identifier
 * @param value The number value
 * @return The expression or NULL in case of no memory
 */
SExpression *createFloat(float value);



SExpression *createDekleration(SExpression *left, SExpression *right);

/**
 * @brief It creates an identifier
 * @param value The identifier name
 * @return The expression or NULL in case of no memory
 */
SExpression *createVar(char* name);


/**
 * @brief It creates an operation
 * @param type The operation type
 * @param left The left operand
 * @param right The right operand
 * @return The expression or NULL in case of no memory
 */
SExpression *createOperation(EOperationType type, SExpression *left, SExpression *right);

/**
 * @brief Deletes a expression
 * @param b The expression
 */
void deleteExpression(SExpression *b);

#endif // __EXPRESSION_H__
