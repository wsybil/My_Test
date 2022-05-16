#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEC 79228162514264337593543950335.0

/* Declaration of structures */
typedef enum value_type_t value_type_t;
typedef struct s21_decimal s21_decimal;
typedef struct super_decimal super_decimal;

/* Arithmetic Operators */
s21_decimal s21_add(s21_decimal src1, s21_decimal src2);
s21_decimal s21_mul(s21_decimal src1, s21_decimal src2);
s21_decimal s21_sub(s21_decimal src1, s21_decimal src2);
s21_decimal s21_div(s21_decimal src1, s21_decimal src2);
s21_decimal s21_mod(s21_decimal src1, s21_decimal src2);

/* Comparison Operators
 * Return value:
 * 0 - TRUE
 * 1 - FALSE
 */
int s21_is_less(s21_decimal src1, s21_decimal src2);
int s21_is_less_or_equal(s21_decimal src1, s21_decimal src2);
int s21_is_greater(s21_decimal src1, s21_decimal src2);
int s21_is_greater_or_equal(s21_decimal src1, s21_decimal src2);
int s21_is_equal(s21_decimal src1, s21_decimal src2);
int s21_is_not_equal(s21_decimal src1, s21_decimal src2);

/* Convertors and parsers
 * Return value - code error:
 * 0 - SUCCESS
 * 1 - CONVERTING ERROR
 */
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

/* Another functions */
s21_decimal s21_floor(s21_decimal src);
s21_decimal s21_round(s21_decimal src);
s21_decimal s21_truncate(s21_decimal src);
s21_decimal s21_negate(s21_decimal src);

/* Additional functions */
/* Cleaning */
void s21_init_decimal(s21_decimal *src);
void s21_clean(s21_decimal *src);

/* Get, set, swap */
int s21_get_scale(s21_decimal src);
int s21_get_sign(s21_decimal src);
void s21_set_scale(s21_decimal *src, int scale);
void s21_set_sign(s21_decimal *src, int sign);
void s21_swap_bit(s21_decimal *src, const int n);

/* Convertors */
int s21_from_float_to_int(float src);
void s21_turn_into_super(s21_decimal *src1, super_decimal *src2);
void s21_turn_into_normal(super_decimal *src1, s21_decimal *src2);
void s21_converting_decimals(s21_decimal *src1, s21_decimal *src2,
                             super_decimal *dst1, super_decimal *dst2);

/* Check */
int s21_check_exp(float src);
int s21_check_neg_exp(float src);
int s21_check_empty(s21_decimal src);

/* Math */
long long s21_pow(int src1, int src2);

/* Operations with super decimal */
/* Arithmetic */
super_decimal s21_add_for_super(super_decimal src1, super_decimal src2);
super_decimal s21_sub_for_super(super_decimal src1, super_decimal src2);
super_decimal s21_mul_for_super(super_decimal src1, super_decimal src2);
super_decimal s21_div_for_super(super_decimal src1, super_decimal src2);
super_decimal s21_mod_for_super(super_decimal src1, super_decimal src2);

/* Comparison */
int s21_is_less_for_super(super_decimal src1, super_decimal src2);
int s21_is_greater_for_super(super_decimal src1, super_decimal src2);
int s21_is_greater_or_equal_for_super(super_decimal src1, super_decimal src2);

/* Cleaning */
void s21_init_sdecimal(super_decimal *src);

/* Get, set */
int s21_get_bit(super_decimal src, const int n);
int s21_get_sscale(super_decimal src);
super_decimal s21_set_sscale(super_decimal src, int scale);
void s21_set_bit(super_decimal *src, const int n, int bit);
void s21_set_scale_for_add(super_decimal *src1, super_decimal *src2,
                           super_decimal *res);
void s21_set_scale_for_div(super_decimal *src1, super_decimal *src2);
void s21_set_scale_for_sub(super_decimal *src1, super_decimal *src2,
                           super_decimal *res);
int s21_get_type_for_add(s21_decimal src1, s21_decimal src2);
int s21_get_type_for_sub(s21_decimal src1, s21_decimal src2);
int s21_get_type_for_less(s21_decimal src1, s21_decimal src2);
int s21_get_type_for_great(s21_decimal src1, s21_decimal src2);
int s21_get_type_for_eq(s21_decimal src1, s21_decimal src2);
int s21_get_type_for_div(s21_decimal src1, s21_decimal src2, int sign_1,
                         int sign_2);
int s21_get_type_for_mul(s21_decimal src1, s21_decimal src2);
int s21_get_type_for_mod(s21_decimal src1, s21_decimal src2);
int s21_get_type_for_not_eq(s21_decimal src1, s21_decimal src2);
int s21_is_one(s21_decimal src);

/* Check */
int s21_check_bits_super(super_decimal *src1, super_decimal *src2);
int s21_check_empty_super(super_decimal src);
int s21_check_overflow(super_decimal src);

/* Shift, normal etc. */
void s21_super_truncate(super_decimal *src, int scale);
void s21_super_round_scale(super_decimal *src);
void s21_shift_mul_ten(super_decimal *src);
void s21_great_shift(super_decimal *src);
void s21_normal_scale(super_decimal *src, int *scale);
void s21_reduction_sdecimal(super_decimal *src, int *scale);
super_decimal s21_one_div(super_decimal src);
super_decimal s21_normal_div(super_decimal src1, super_decimal src2);

enum value_type_t {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3
};

struct s21_decimal {
  unsigned int bits[4];
  value_type_t value_type;
};

struct super_decimal {
  unsigned int bits[7];
  value_type_t value_type;
};

#endif  // SRC_S21_DECIMAL_H_
