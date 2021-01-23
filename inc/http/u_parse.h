#ifndef __U_PARSE_H__
#define __U_PARSE_H__



#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <assert.h>
#include <u_tree.h>

//#define SLICE(n,type) ((type*)realloc())

typedef struct _pat u_pat;
typedef struct _lexeme  u_lexeme;
typedef struct _tokkens  u_tokkens;


//-- Скомпилированная регулярка --//
struct _pat
{
    regex_t* regex;
    regmatch_t* pm;
    size_t n;
    int flags;
	size_t id;
};

//-- raw regex --//
struct _lexeme
{
	char* regex;		// <-- Сырая регулярка
//	size_t regex_n;		// <-- Число символов сырой регулярки
	int flags;			// <-- Флаги компиляции (например REG_ICASE)
};

struct u_sequence_el
{
	char* arr; //текст найдёныша
	size_t adr; //адрес найдёныша/id сортировки
	size_t id; //id-идентификатор паттерна-родителя
};

struct _tokkens
{
//	u_lexeme* tok; // <-- Лексемы токкенов
	//------------------//
  	size_t* sequence;	// <-- id токкенов в найденной последовательности
	size_t seq_n;	 	// <-- размер буфера токкенов 

	u_pat* pat;		// <-- массив паттернов
	size_t pat_n;		// <-- число паттернов
	//------------------//
	struct u_sequence_el* els; //массив найдёнышей
	size_t els_n;
};


u_lexeme* u_lex_new(const char* regex,int flags);
void u_lex_free(u_lexeme* lx);

/*  @brief: выводит на экран содержимое regmatch_t */
void print_n(char* str, regmatch_t* pm);

/* инициализирует регулярку, забивая в неё паттерн и флаги */
void pat_init(u_pat* p, u_lexeme* lx);


/* очищает структуру pat и её содержимое */
void pat_free( u_pat* p);

/* выполняет скомпилированную регулярку в си строке */
int pat_exec( u_pat* p,const char* __String);


#endif //!__U_PARSE_H__

