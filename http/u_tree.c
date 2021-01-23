
/******************************************
 * Структура данных бинарного дерева поиска,
 * добавляет, попутно сортируя значения.
 * слева - меньше; справа - больше
 * ***************************************/
#include "../inc/http/u_tree.h"

//-- Заглушка для теста --
extern int main(int argc, char* argv[] ) __attribute__((weak,alias("test")));

// typedef struct tnode_tag tnode;



// Вывод узлов дерева (обход в инфиксной форме)
void treeprint(tnode *tree)
{
  static int i=0;
  i++;
  if (tree != NULL) {      //Пока не встретится пустой узел
    treeprint(tree->left);  //Рекурсивная функция вывода левого поддерева
    printf("lvl:%d - %d\n",i, tree->key);//Отображаем корень дерева
    treeprint(tree->right); //Рекурсивная функция вывода правого поддерева
  }
  i--;
}


// Добавление узлов в дерево
tnode* addnode( tnode *tree, int id, void* data)
{
  if (!tree)     // Если дерева нет, то формируем корень
  {
    tree = c_new(tnode); //память под узел
    tree->key = id;   //поле данных
    tree->left = 0;
    tree->right = 0; //ветви инициализируем пустотой
    tree->data = data;
  }
  else{     // иначе
  
    if (id < tree->key){   //Если элемент x меньше корневого, уходим влево
      tree->left = addnode(tree->left, id, data); //Рекурсивно добавляем элемент
    }else{  //иначе уходим вправо
      tree->right = addnode(tree->right, id, data); //Рекурсивно добавляем элемент
    }
  }
  return(tree);
}

//Освобождение памяти дерева
void freemem(tnode *tree, void(*data_free_f)(void*))
{
  if (tree != NULL)    // если дерево не пустое
  {
    freemem(tree->left, data_free_f);   // рекурсивно удаляем левую ветку
    freemem(tree->right, data_free_f);  // рекурсивно удаляем правую ветку
	
	//
	if(data_free_f)
	{	
		data_free_f(tree->data);
	}
    free(tree);           // удаляем корень
	
  }
}

//--взять ноду по id
tnode* get_elem_by_key(tnode* tree,int key)
{
  if(!tree) return 0;
  if(key==tree->key){
    return tree;
  }
  if(key > tree->key)
  {
    return get_elem_by_key(tree->right,key);
  }
  if(key < tree->key)
  {
    return get_elem_by_key(tree->left,key);
  }
}

//данные по id
void* get_data_by_key(tnode* tree,int key)
{
	return get_elem_by_key(tree,key)->data;
}


#include <math.h>



// Тестирование работы
int test(int argc, char* argv[] ) 
{
  tnode *root = 0;    // Объявляем структуру дерева
  // В цикле вводим 8 узлов дерева
  for (int i = 0; i<1024; i++) 
  {
    root = addnode(root, rand() , malloc(4096) ); // размещаем введенный узел на дереве
  }
  treeprint(root);    // выводим элементы дерева, получаем отсортированный массив
  freemem(root, free);      // удаляем выделенную память
  return 0;
}













//----------------------------- END -----------------------------------------

