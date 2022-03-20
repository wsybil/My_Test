#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S21_NANF __builtin_nanf("0x7fc00000")
#define S21_INFL __builtin_infl()
#define S21_HUGE_VAL __builtin_huge_vall()

#define SIGN_MINUS 1U << 31

typedef enum {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3
} value_type_t;

typedef struct {
  unsigned int bits[4];
  value_type_t value_type;
} s21_decimal;

typedef union {
  float fl;
  struct {
    unsigned mantisa : 23;
    unsigned exp : 8;
    unsigned sign : 1;
  } bits;
} fl_bits;

short int *s21_transformation(short int *res, short int *pow, int type,
                              int col);
void s21_normalization_decimal(short int *res, s21_decimal src);
short int get_scale(s21_decimal src);
short int get_scale_float(float src);

void init_dec(s21_decimal *dec);
s21_decimal str_to_dec(char binary_number[], s21_decimal *dec, int point_place);

int get_float(float src, int idx);
int get(s21_decimal data, int idx);
int compare_variable(int var_1, int var_2);

int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_is_less_or_equal(s21_decimal dst1, s21_decimal dst2);
int s21_is_less(s21_decimal dst1, s21_decimal dst2);
int s21_is_less_or_equal(s21_decimal dst1, s21_decimal dst2);
int s21_is_greater(s21_decimal dst1, s21_decimal dst2);
int s21_is_greater_or_equal(s21_decimal dst1, s21_decimal dst2);
int s21_is_equal(s21_decimal dst1, s21_decimal dst2);
int s21_is_not_equal(s21_decimal dst1, s21_decimal dst2);

void s21_clean(s21_decimal *dst);
void s21_print_decimal(s21_decimal src);
void set_float(float *data, int idx, int bit);
void set(s21_decimal *data, int idx, int bit);
void s21_print_normalized_array(short int *res);
void set_scale(s21_decimal *src, unsigned int scale);

s21_decimal s21_floor(s21_decimal dst);
s21_decimal s21_round(s21_decimal dst);
s21_decimal s21_truncate(s21_decimal dst);
s21_decimal s21_negate(s21_decimal dst);

s21_decimal s21_add(s21_decimal src, s21_decimal dst);
s21_decimal s21_sub(s21_decimal src, s21_decimal dst);
s21_decimal s21_mul(s21_decimal src, s21_decimal dst);
s21_decimal s21_mod(s21_decimal src, s21_decimal dst);
s21_decimal s21_div(s21_decimal src, s21_decimal dst);

#endif  //  SRC_S21_DECIMAL_H_
