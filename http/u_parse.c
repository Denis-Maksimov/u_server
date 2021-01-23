
#include <u_parse.h>

//-------токенизация------------
/**
 /// 1)Разбить по переносам строк
* 2) Исследовать HTTP
* 3) HTML 
*/


//-------синтаксический анализ------------



/**************************************************
 *  @brief: выводит на экран содержимое regmatch_t
 *
 *
 ************************************************* */
void print_n(char* str, regmatch_t* pm)
{
    regoff_t off = pm->rm_eo-pm->rm_so;
    char* s_t=(char*)malloc(sizeof(char)*(off+1));
    strncpy(s_t, str + pm->rm_so, off);
    s_t[off]=0;
    puts(s_t);
    free(s_t);
}


char* str = \
"GET / HTTP/1.1 \r\n\
Host: site.ru   \n\
User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:18.0) Gecko/20100101 Firefox/18.0   \r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \n\
Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3    \n\
Accept-Encoding: gzip, deflate  \n\
Cookie: wp-settings \n\
Connection: keep-alive\r\n\r\n \
<html>" ;



typedef struct {
    regex_t* regex;
    regmatch_t* pm;
    size_t n;
    int flags;

}pat;

static pat aa[4];

u_lexeme* u_lex_new(const char* regex,int flags)
{
		size_t n=strlen(regex);
		u_lexeme* lx=malloc(sizeof(u_lexeme));
		lx->regex=(char*)malloc(n);
		memcpy(lx->regex,regex,n);
		lx->flags=flags;
		return lx;
}

void u_lex_free(u_lexeme* lx)
{
		free(lx->regex);
		free(lx);
}

/* инициализирует регулярку, забивая в неё паттерн и флаги */
void pat_init(u_pat* p, u_lexeme* lx) 
{
    size_t brackets=1; //главная группа
    static size_t IDs=0;
    //считаем скобочные группы
    char * ptr=strchr(lx->regex, ')' )+1;
    if((char*)1 != ptr){
        do{
            brackets++;
        }while ((char*)1!=(ptr=strchr(ptr,')')+1));
    }

    //выделяем память под структуры
    p->regex=malloc(sizeof(regex_t));
    p->pm=calloc(brackets,sizeof(regmatch_t));
    p->n=brackets;
    //компилируем регулярку
    int err;
    err = regcomp (p->regex, lx->regex, p->flags);
	p->id=IDs;
	IDs++;

	//TODO: проверить на ошибки
}

/* выполняет скомпилированную регулярку в си строке */
int pat_exec(u_pat* p,const char* __String)
{

    puts("execute regex...");
        int regerr = regexec (p->regex, __String, p->n, p->pm, p->flags);
        char errbuf[512];
        regerror(regerr, p->regex, errbuf, sizeof(errbuf));
        puts(errbuf);

        if(!regerr){ 
            for (size_t i = 0; i < p->n; i++)
            {   
				// TODO: в патерне сохранить надо..???
                print_n(str,&(p->pm[i]));                
            }
        }
		return regerr;

}

/* очищает структуру pat и её содержимое */
void pat_free(u_pat* p)
{
    regfree(p->regex);
    free(p->regex);
    free(p->pm);
}




const char* pattern[] = {
    "^(GET|POST)\\s*(\\S+)\\s*HTTP", "host\\s*:\\s*(\\S+)",             "<(\\w|\\W)+$",
};

int flags[] = {
    REG_EXTENDED|REG_NEWLINE,     REG_EXTENDED|REG_ICASE|REG_NEWLINE,   REG_EXTENDED
};


int main()
{

    // aa[0].brackets=
    for (size_t i = 0; i < 3; i++)
    {
        // pat_init(&aa[i], pattern[i], flags[i]);
        pat_exec(&aa[i],str);
        pat_free(&aa[i]);
        // regfree(&regex[0]);

    }

    return 0;
}


char* post=
"POST / HTTP/1.1\n\
Host: localhost:3001\n\
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\n\
Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\n\
Accept-Encoding: gzip, deflate\n\
Content-Type: multipart/form-data; boundary=---------------------------321337701126208626203916113922\n\
Content-Length: 968\n\
Origin: http://localhost:3001\n\
DNT: 1\n\
Connection: keep-alive\n\
Referer: http://localhost:3001/\n\
Upgrade-Insecure-Requests: 1\n\
\n\
-----------------------------321337701126208626203916113922\n\
Content-Disposition: form-data; name=\"f\"; filename=\"&#1058;&#1077;&#1082;&#1089;&#1090;&#1086;&#1074;&#1099;&#1081; &#1092;&#1072;&#1081;&#1083;\"\n\
Content-Type: application/octet-stream\n\
\n\
\n\
[255416.718736] mce: [Hardware Error]: Machine check events logged\n\
[255416.718744] mce: [Hardware Error]: CPU 1: Machine Check: 0 Bank 0: 9000004000010005\n\
[255416.718749] mce: [Hardware Error]: TSC 605b5a9a423c \n\
[255416.718751] mce: [Hardware Error]: PROCESSOR 0:806e9 TIME 1584744994 SOCKET 0 APIC 2 microcode 84\n\
[255416.718753] mce: [Hardware Error]: Machine check events logged\n\
[255416.718754] mce: [Hardware Error]: CPU 3: Machine Check: 0 Bank 0: 9000004000010005\n\
[255416.718754] mce: [Hardware Error]: TSC 605b5a9a4d24 \n\
[255416.718756] mce: [Hardware Error]: PROCESSOR 0:806e9 TIME 1584744994 SOCKET 0 APIC 3 microcode 84\n\
\n\
\n\
0|(1 ) b|\"(*&o:.L;'Z\n\
\n\
-----------------------------321337701126208626203916113922--\n\
";


//---------- высокий уровень --------------

// что здесь происходит???
void get_lexeme(u_tokkens* toks, char* text_buffer)
{
	size_t n;
	tnode *root = 0; 
	//NOTE: лучше сортировать деревом?
	
	//Для каждого патттерна
	for(size_t i; i < toks->pat_n; i++){
		//пока ищется	
		while(!pat_exec(&toks->pat[i], text_buffer))
		{
 
				toks->els_n++; 
				n = toks->els_n; 

				toks->els=realloc(toks->els, n*sizeof(struct u_sequence_el));
				toks->els[n].id = toks->pat[i].id; //id parent pattern
				//toks->els[n]->adr //Да бля(((
				//toks->pat[i]->id;
				
				//размер найденного блока
				regoff_t off = toks->pat[i].pm->rm_eo - toks->pat[i].pm->rm_so;

				//найдёныша копируем в кучу
				toks->els[n].arr = (char*)calloc(off+1, sizeof(char) );
				strncpy(toks->els[n].arr,  text_buffer + toks->pat[i].pm->rm_so, off);
				toks->els[n].arr[off]=0;
			// чё добавлять то, бляха-муха?	
				root = addnode(root, toks->pat[i].pm->rm_so,  &toks->els[n] ); 


		}

	}


}










//------------------------------ END ------------------------------------------//

