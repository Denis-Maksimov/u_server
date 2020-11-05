/*
 * Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>
 * Copyright (c) 2013 Cesanta Software Limited
 * All rights reserved
 *
 * This library is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this library under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this library under a commercial
 * license, as set out in <http://cesanta.com/products.html>.
 */

/*
 * This is a regular expression library that implements a subset of Perl RE.
 * Please refer to README.md for a detailed reference.
 */

#ifndef CS_SLRE_SLRE_H_
#define CS_SLRE_SLRE_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Сохраняет соответствующий фрагмент для выражения в скобках */
struct slre_cap {
  const char *ptr;
  int len;
};


/**
 * @brief  сопоставляет строковый буфер `buf` длиной` buf_len` с регулярным выражением `regexp`, 
 * которое должно соответствовать синтаксису, описанному выше. Если регулярное выражение 
 * `regexp` содержит скобки,` slre_match () `может захватывать соответствующие подстроки в массив структур `struct slre_cap`
 * N-й член массива `caps` будет содержать фрагмент, соответствующий
 * N-я открывающая скобка в регулярном выражении, N начинается с нуля. `slre_match ()` 
 * @note   
 * @param  regexp: 
 * @param  buf: 
 * @param  buf_len: 
 * @param  caps: 
 * @param  num_caps: 
 * @param  flags: 
 * @retval возвращает
 * количество байтов, отсканированных с начала строки. Если возвращаемое значение
 * больше или равно 0, есть совпадение. Если возвращаемое значение меньше 0, то
 * не соответствует. Отрицательные коды возврата указаны ниже
 */
int slre_match(const char* regexp, const char* buf, int buf_len, struct slre_cap* caps, int num_caps, int flags);

/* Possible flags for slre_match() */
enum { SLRE_IGNORE_CASE = 1 };


/* slre_match() failure codes */
#define SLRE_NO_MATCH               -1  //выражение не найдено
#define SLRE_UNEXPECTED_QUANTIFIER  -2 
#define SLRE_UNBALANCED_BRACKETS    -3  //проверьте скобочные группы
#define SLRE_INTERNAL_ERROR         -4  
#define SLRE_INVALID_CHARACTER_SET  -5
#define SLRE_INVALID_METACHARACTER  -6
#define SLRE_CAPS_ARRAY_TOO_SMALL   -7
#define SLRE_TOO_MANY_BRANCHES      -8
#define SLRE_TOO_MANY_BRACKETS      -9

#ifdef __cplusplus
}
#endif

#endif /* CS_SLRE_SLRE_H_ */
