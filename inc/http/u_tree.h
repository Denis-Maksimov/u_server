#ifndef __U_TREE_H__
#define __U_TREE_H__

#include <stdio.h>
#include <stdlib.h>

#ifndef c_new
#define c_new(t)     ((t*)malloc(sizeof(t)))
#endif




// Структура - узел дерева
typedef struct tnode_tag
{
  int key;           // поле данных
  struct tnode_tag *left;  // левый потомок
  struct tnode_tag *right; // правый потомок
  void* data;             //данные
} tnode;

// typedef struct tnode_tag tnode;



// Вывод узлов дерева (обход в инфиксной форме)
void treeprint(tnode *tree);



// Добавление узлов в дерево
tnode* addnode( tnode *tree, int id, void* data);

//Освобождение памяти дерева
void freemem(tnode *tree, void(*data_free_f)(void*));


//--взять ноду по id
tnode* get_elem_by_key(tnode* tree,int key);

//данные по id
void* get_data_by_key(tnode* tree,int key);














#endif //!__U_TREE_H__

//----------------------------- END -----------------------------------------

