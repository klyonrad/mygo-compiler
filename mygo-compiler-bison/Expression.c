/*
 * Expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "Expression.h"

#include <stdlib.h>

/**
 * @brief Allocates space for expression
 * @return The expression or NULL if not enough memory
 */
static SExpression *allocateExpression()
{
    SExpression *b = (SExpression *)malloc(sizeof(SExpression));

    if (b == NULL)
        return NULL;

    b->type = eVALUE;
    b->value = 0;

    b->left = NULL;
    b->right = NULL;

    return b;
}

SExpression *createVar(char* name){
  SExpression *b = allocateExpression();

  if(b==NULL)
    return NULL;

    b->type = eIDENTIFIER;
    b->svalue = string(name);
}

SExpression *createNumber(int value)
{
    SExpression *b = allocateExpression();

    if (b == NULL)
        return NULL;

    b->type = eVALUE;
    b->value = value;

    return b;
}

SExpression *createFloat(float value)
{
    SExpression *b = allocateExpression();

    if (b == NULL)
        return NULL;

    b->type = eFLOAT;
    b->fvalue = value;

    return b;
}

SExpression *createDekleration(string name, SExpression *right){
      SExpression *b = allocateExpression();

      if (b == NULL)
          return NULL;

      b->type = eDEKLERATION;
      b->left = NULL;
      b->right = right;
      b->svalue= name;

      return b;
}

SExpression *createOperation(EOperationType type, SExpression *left, SExpression *right)
{
    SExpression *b = allocateExpression();

    if (b == NULL)
        return NULL;

    b->type = type;
    b->left = left;
    b->right = right;

    return b;
}

void deleteExpression(SExpression *b)
{
    if (b == NULL)
        return;

    deleteExpression(b->left);
    deleteExpression(b->right);

    free(b);
}
