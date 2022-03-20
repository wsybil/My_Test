#include "s21_decimal.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void s21_decimal_max(s21_decimal *src) {
  for (int i = 0; i < 3; i++) {
    src->bits[i] = 4294967295;
  }
  src->value_type = 0;
}

int s21_check_null(s21_decimal src) {
  int res = 0;
  if (src.bits[0] == 0 && src.bits[1] == 0 && src.bits[2] == 0) {
    res = 1;
  }
  return res;
}

int get(s21_decimal data, int idx) {
  unsigned int mask = 1u << (idx % 32);
  return data.bits[idx / 32] & mask ? 1 : 0;
}

void set(s21_decimal *data, int idx, int bit) {
  if (data != NULL) {
    unsigned int mask = 1u << (idx % 32);
    if (bit == 0) {
      mask = ~mask;
      data->bits[idx / 32] &= mask;
    } else {
      data->bits[idx / 32] |= mask;
    }
  }
}

void set_scale(s21_decimal *src, unsigned int scale) {
  for (int i = 112; i < 120; i++) {
    if (scale % 2 == 1) {
      set(src, i, 1);
    } else {
      set(src, i, 0);
    }
    scale /= 2;
  }
}

void s21_clean(s21_decimal *src) {
  for (int i = 0; i < 4; i++) {
    src->bits[i] = 0;
  }
}

void s21_clean_array(short int *src, int col) {
  for (int i = 0; i < col; i++) {
    src[i] = 0;
  }
}

void set_float(float *data, int idx, int bit) {
  unsigned int *flt = ((unsigned int *)data);
  if (data != NULL) {
    unsigned int mask = 1u << idx;
    if (bit == 0) {
      mask = ~mask;
      (*flt) &= mask;
    } else {
      (*flt) |= mask;
    }
  }
}

int get_float(float src, int idx) {
  unsigned int *flt = ((unsigned int *)&src);
  unsigned int mask = 1u << idx;
  return *flt & mask ? 1 : 0;
}

short int get_scale(s21_decimal src) {
  short int scale = 0;
  for (int i = 119, n = 7; i > 111; i--, n--) {
    if (get(src, i) == 1) {
      scale += pow(2, n);
    }
  }
  return scale;
}

short int get_scale_float(float src) {
  short int scale = 0;
  for (int i = 30, n = 7; i > 22; i--, n--) {
    if (get_float(src, i) == 1) {
      scale += pow(2, n);
    }
  }
  return scale - 127;
}

void s21_sum(short int *res, short int *pow, int col) {
  for (int i = 0; i < col; i++) {
    res[i] += pow[i];
  }
  for (int i = col - 1; i > -1; i--) {
    if (i > 0) {
      res[i - 1] += res[i] / 10;
      res[i] -= (res[i] / 10) * 10;
    }
  }
}

void s21_mult(short int *arr, short int *res, short int *pow,
              int col) {  // not free point which return
  int col_new = ((col * 2) - 1);
  short int multiplication[col_new];
  s21_clean_array(arr, col_new);
  for (int i = col - 1, sdvig = col_new - 1; i > -1; i--, sdvig--) {
    s21_clean_array(multiplication, col_new);
    for (int j = col - 1, n = sdvig; j > -1 && n > -1; j--, n--) {
      multiplication[n] = res[i] * pow[j];
    }
    s21_sum(arr, multiplication, col_new);
  }
}

void s21_razn(short int *res, short int *pow, int col) {
  for (int i = col - 1; i > 0; i--) {
    if (res[i] >= pow[i]) {
      res[i] -= pow[i];
    } else {
      res[i] += 10;
      res[i] -= pow[i];
      res[i - 1] -= 1;
    }
  }
}

void s21_add_in_arr_end(short int *src, int col, short int ell) {
  for (int i = 0; i < col; i++) {
    if (i != col - 1) {
      src[i] = src[i + 1];
    } else {
      src[i] = ell;
    }
  }
}

int s21_compare_sup(short int *src, short int *dst, int col) {  // COMPLETE
  int res = -2;
  for (int i = 0; i < col; i++) {
    if (src[i] != 0 || dst[i] != 0) {
      res = 0;
    }
    if (src[i] > dst[i]) {
      res = 1;
      break;
    } else if (dst[i] > src[i]) {
      res = -1;
      break;
    }
  }
  return res;
}

void s21_copy(short int *src, short int *res) {
  for (int i = 0; i < 58; i++) {
    src[i] = res[i];
  }
}

short int s21_norm_div(short int *src, int dec) {
  short int last_res = 0;
  short int res[58];
  s21_clean_array(res, 58);
  if (dec > 30) {
    res[0] = 9;
  } else {
    for (int i = 0, j = 30 - dec; i < 58 && j < 58; i++, j++) {
      res[j] = src[i];
      if (j == 57 && i + 1 < 58) {
        last_res = src[i + 1];
      }
    }
  }
  s21_copy(src, res);
  return last_res;
}

short int s21_divis(short int *arr, short int *src, short int *dst, int col,
                    int type) {  // not free point which return
  short int last_res = 0;
  short int sup[col];
  s21_clean_array(arr, col);
  s21_clean_array(sup, col);
  int ind = 0;
  int ind2 = 0;
  int col_dob = 0;
  for (int i = 0; i < col; i++) {
    if (src[i] > 0) {
      ind = 1;
    }
    if (ind == 1) {
      short int res_add = 0;
      s21_add_in_arr_end(sup, col, src[i]);
      while (s21_compare_sup(sup, dst, 58) >= 0) {
        s21_razn(sup, dst, col);
        res_add++;
      }
      if (res_add != 0) {
        ind2 = 1;
      }
      if (ind2 == 1) {
        s21_add_in_arr_end(arr, col, res_add);
        col_dob++;
      }
    }
  }
  if (type == 1) {
    s21_copy(arr, sup);
  } else {
    int indicator = 0;
    for (int i = col_dob; i < col && indicator == 0; i++) {
      s21_add_in_arr_end(sup, col, 0);
      short int res_add = 0;
      while (s21_compare_sup(sup, dst, 58) >= 0) {
        s21_razn(sup, dst, col);
        res_add++;
      }
      s21_add_in_arr_end(arr, col, res_add);
      if (sup[0] != 0) {
        indicator = 1;
      }
    }
    last_res = s21_norm_div(arr, col_dob);
  }
  return last_res;
}

void s21_pow(short int *pow, int i, int col) {
  if (i == 0) {
    pow[col - 1] = 1;
  } else {
    s21_sum(pow, pow, col);
  }
}

void s21_normalization_decimal(short int *res,
                               s21_decimal src) {  // not free point
  short int pow[29];
  s21_clean_array(res, 29);
  s21_clean_array(pow, 29);
  for (int j = 0; j < 96; j++) {
    s21_pow(pow, j, 29);
    if (get(src, j) == 1) {
      s21_sum(res, pow, 29);
    }
  }
}

void s21_array_div_2(short int *src, int lst) {
  for (int i = lst; i > -1; i--) {
    if (i != lst) {
      if (src[i] % 2 == 1) {
        src[i + 1] += 5;
      }
    }
    src[i] /= 2;
  }
}

s21_decimal s21_from_array_to_decimal(short int *res, s21_decimal src) {
  int idx = 0;
  for (int i = 0; i < 29; i++) {
    while (res[i] != 0 && idx < 96) {
      if (res[28] % 2 == 1) {
        set(&src, idx, 1);
      } else {
        set(&src, idx, 0);
      }
      idx++;
      s21_array_div_2(res, 28);
    }
  }
  for (int i = idx; i < 96; i++) {
    set(&src, i, 0);
  }
  return src;
}

void s21_array_away(short int *res, short int *src, unsigned int *scale,
                    int col, short int last_res) {  // not free point
  int gran;
  if (col % 2 == 1) {
    gran = 58;
  } else {
    gran = 29;
  }
  s21_clean_array(res, 29);
  int ind = 0;
  *scale = 0;
  int i = 1;
  for (int j = 0; j < 29 && i < col; i++) {
    if (src[i] != 0 || i >= gran) {
      ind = 1;
    }
    if (ind == 1) {
      if (j < 29 && i < col) {
        res[j++] = src[i];
      }
      if (i > gran) {
        (*scale)++;
      }
    }
  }
  i--;
  if (i + 1 < col && i >= gran - 1) {
    if ((src[i] % 2 == 1 && src[i + 1] == 5) || src[i + 1] > 5) {
      short int sum[29];
      s21_clean_array(sum, 29);
      sum[28] = 1;
      s21_sum(res, sum, 29);
    }
  } else if (i < col && i >= gran - 1) {
    if (((src[i] % 2 == 1 && last_res == 5) || last_res > 5)) {
      short int sum[29];
      s21_clean_array(sum, 29);
      sum[28] = 1;
      s21_sum(res, sum, 29);
    }
  }
}

void s21_array_to_add(short int *res, short int *src,
                      short int scale) {  // not free point
  s21_clean_array(res, 58);
  for (int i = scale + 1, j = 0; j < 29; j++, i++) {
    res[i] = src[j];
  }
}

int s21_compare(s21_decimal src1, s21_decimal src2) {
  int res = -2;
  if (src1.value_type == 0 && src2.value_type == 0) {
    short int norm1[29];
    short int norm2[29];
    s21_normalization_decimal(norm1, src1);
    s21_normalization_decimal(norm2, src2);
    short int dst1[58];
    s21_array_to_add(dst1, norm1, get_scale(src1));
    short int dst2[58];
    s21_array_to_add(dst2, norm2, get_scale(src2));
    res = s21_compare_sup(dst1, dst2, 58);
    if (get(src1, 127) == 1 && get(src2, 127) == 0 && res != -2) {
      res = -1;
    } else if (get(src1, 127) == 0 && get(src2, 127) == 1 && res != -2) {
      res = 1;
    } else if (get(src1, 127) == 1 && get(src2, 127) == 1 && res != -2) {
      res *= -1;
    }
    if (res == -2) {
      res = 0;
    }
  } else if ((src1.value_type == 0 && src2.value_type == 1) ||
             (src1.value_type == 2 && src2.value_type == 0) ||
             (src1.value_type == 2 && src2.value_type == 1)) {
    res = -1;
  } else if ((src1.value_type == 0 && src2.value_type == 2) ||
             (src1.value_type == 1 && src2.value_type == 0) ||
             (src1.value_type == 1 && src2.value_type == 2)) {
    res = 1;
  }
  return res;
}

void s21_array_sdvig(short int *src, unsigned int *count_int, int count) {
  int ind = 0;
  int j = count - 1;
  short int fin[29];
  s21_clean_array(fin, 29);
  for (int i = count - 1; i > -1 && j > -1; i--) {
    if (src[i] == 0 && *count_int > 0 && ind == 0) {
      (*count_int)--;
    } else {
      ind = 1;
    }
    if (ind == 1) {
      fin[j--] = src[i];
    }
  }
  for (int i = 0; i < 29; i++) {
    src[i] = fin[i];
  }
}

short int *s21_add_first_null(short int *src) {
  short int res[29];
  s21_clean_array(res, 29);
  if ((src[27] % 2 == 1 && src[28] == 5) || src[28] > 5) {
    short int sum[29];
    s21_clean_array(sum, 29);
    sum[27] = 1;
    s21_sum(src, sum, 29);
  }
  for (int i = 1, j = 0; i < 29 && j < 29; i++, j++) {
    res[i] = src[j];
  }
  for (int i = 0; i < 29; i++) {
    src[i] = res[i];
  }
  return src;
}

void s21_array_norm(short int *res, unsigned int *scale) {  // COMPLETE
  s21_decimal dst;
  s21_decimal_max(&dst);
  short int src[29];
  s21_normalization_decimal(src, dst);
  while (s21_compare_sup(res, src, 29) > 0 && *scale > 0) {
    res = s21_add_first_null(res);
    (*scale)--;
  }
}

int s21_compape_to_inf(short int *src, int col) {
  int result = 0;
  int gran;
  if (col % 2 == 1) {
    gran = 58;
  } else {
    gran = 29;
  }
  s21_decimal dst;
  s21_decimal_max(&dst);
  short int comp_res[29];
  s21_normalization_decimal(comp_res, dst);
  short int res[col];
  s21_clean_array(res, col);
  for (int i = gran, j = 28; j > -1 && i > -1; i--, j--) {
    res[i] = comp_res[j];
  }
  if (s21_compare_sup(src, res, col) > 0) {
    result = 1;
  }
  return result;
}

s21_decimal s21_summ_pol(s21_decimal src, s21_decimal dst) {
  unsigned int scale;
  int error = 0;
  short int norm1[29];
  short int norm2[29];
  s21_normalization_decimal(norm1, src);
  s21_normalization_decimal(norm2, dst);
  short int res[58];
  s21_array_to_add(res, norm1, get_scale(src));
  short int res_a[58];
  s21_array_to_add(res_a, norm2, get_scale(dst));
  s21_sum(res, res_a, 58);
  if (s21_compape_to_inf(res, 58) == 0) {
    short int array_away[29];
    s21_array_away(array_away, res, &scale, 58, 0);
    s21_array_sdvig(array_away, &scale, 29);
    s21_array_norm(array_away, &scale);
    src = s21_from_array_to_decimal(array_away, src);
    set_scale(&src, scale);
    src.value_type = 0;
  } else {
    s21_clean(&src);
    src.value_type = 1;
  }
  if (error == 1) {
    s21_clean(&src);
    src.value_type = 3;
  }
  return src;
}

s21_decimal s21_mult_pol(s21_decimal src, s21_decimal dst) {
  unsigned int scale;
  short int norm1[29];
  s21_normalization_decimal(norm1, src);
  short int norm2[29];
  s21_normalization_decimal(norm2, dst);
  short int res[58];
  s21_array_to_add(res, norm1, get_scale(src));
  short int res_a[58];
  s21_array_to_add(res_a, norm2, get_scale(dst));
  short int multic[115];
  s21_mult(multic, res, res_a, 58);
  if (s21_compape_to_inf(multic, 115) == 0) {
    short int array_away[29];
    s21_array_away(array_away, multic, &scale, 115, 0);
    s21_array_sdvig(array_away, &scale, 29);
    s21_array_norm(array_away, &scale);
    src = s21_from_array_to_decimal(array_away, src);
    set_scale(&src, scale);
    src.value_type = 0;
  } else {
    s21_clean(&src);
    src.value_type = 1;
  }
  return src;
}

s21_decimal s21_razn_pol(s21_decimal src, s21_decimal dst) {
  unsigned int scale;
  int first_sign = get(src, 127);
  int second_sign = get(dst, 127);
  short int norm1[29];
  s21_normalization_decimal(norm1, src);
  short int norm2[29];
  s21_normalization_decimal(norm2, dst);
  short int res[58];
  s21_array_to_add(res, norm1, get_scale(src));
  short int res_a[58];
  s21_array_to_add(res_a, norm2, get_scale(dst));
  short int array_away[29];
  if (s21_compare_sup(res, res_a, 58) >= 0) {
    s21_razn(res, res_a, 58);
    s21_array_away(array_away, res, &scale, 58, 0);
  } else {
    s21_razn(res_a, res, 58);
    s21_array_away(array_away, res_a, &scale, 58, 0);
    first_sign = second_sign;
  }
  s21_array_sdvig(array_away, &scale, 29);
  s21_array_norm(array_away, &scale);
  src = s21_from_array_to_decimal(array_away, src);
  set_scale(&src, scale);
  set(&src, 127, first_sign);
  src.value_type = 0;
  return src;
}

s21_decimal s21_mod_pol(s21_decimal src, s21_decimal dst, int type) {
  unsigned int scale;
  short int norm1[29];
  s21_normalization_decimal(norm1, src);
  short int norm2[29];
  s21_normalization_decimal(norm2, dst);
  short int res[58];
  s21_array_to_add(res, norm1, get_scale(src));
  short int res_a[58];
  s21_array_to_add(res_a, norm2, get_scale(dst));
  if (s21_check_null(dst) == 0) {
    short int divis[58];
    short int last_res = s21_divis(divis, res, res_a, 58, type);
    if (s21_compape_to_inf(divis, 58) == 0) {
      short int array_away[29];
      s21_array_away(array_away, divis, &scale, 58, last_res);
      s21_array_sdvig(array_away, &scale, 29);
      s21_array_norm(array_away, &scale);
      src = s21_from_array_to_decimal(array_away, src);
      set_scale(&src, scale);
      src.value_type = 0;
    } else {
      s21_clean(&src);
      src.value_type = 1;
    }
  } else {
    s21_clean(&src);
    if (get(dst, 127) == 1) {
      src.value_type = 2;
    } else {
      src.value_type = 1;
    }
  }
  return src;
}

int s21_from_array_to_int(short int *src, short int scale) {
  int res = 0;
  for (int i = 0, j = 28 - scale; i < 29; i++, j--) {
    res += src[i] * pow(10, j);
  }
  return res;
}

s21_decimal s21_add(s21_decimal src, s21_decimal dst) {
  if (src.value_type == 0 && dst.value_type == 0) {
    if (get(src, 127) == 1 && get(dst, 127) == 1) {
      src = s21_summ_pol(src, dst);
      if (src.value_type == 0) {
        set(&src, 127, 1);
      } else if (src.value_type == 1) {
        src.value_type = 2;
      }
    } else if (get(src, 127) == 0 && get(dst, 127) == 0) {
      src = s21_summ_pol(src, dst);
    } else {
      src = s21_razn_pol(src, dst);
    }
  } else if ((src.value_type == 0 && dst.value_type == 1) ||
             (src.value_type == 1 && dst.value_type == 0) ||
             (src.value_type == 1 && dst.value_type == 1)) {
    s21_clean(&src);
    src.value_type = 1;
  } else if ((src.value_type == 0 && dst.value_type == 2) ||
             (src.value_type == 2 && dst.value_type == 0) ||
             (src.value_type == 2 && dst.value_type == 2)) {
    s21_clean(&src);
    src.value_type = 2;
  } else {
    s21_clean(&src);
    src.value_type = 3;
  }
  return src;
}

s21_decimal s21_sub(s21_decimal src, s21_decimal dst) {
  if (src.value_type == 0 && dst.value_type == 0) {
    if (get(dst, 127) == 1) {
      set(&dst, 127, 0);
    } else {
      set(&dst, 127, 1);
    }
    src = s21_add(src, dst);
  } else if ((src.value_type == 0 && dst.value_type == 2) ||
             (src.value_type == 1 && dst.value_type == 0) ||
             (src.value_type == 1 && dst.value_type == 2)) {
    s21_clean(&src);
    src.value_type = 1;
  } else if ((src.value_type == 0 && dst.value_type == 1) ||
             (src.value_type == 2 && dst.value_type == 0)) {
    s21_clean(&src);
    src.value_type = 2;
  } else {
    s21_clean(&src);
    src.value_type = 3;
  }
  return src;
}

s21_decimal s21_mul(s21_decimal src, s21_decimal dst) {
  if (src.value_type == 0 && dst.value_type == 0) {
    if ((get(src, 127) == 1 && get(dst, 127) == 1) ||
        (get(src, 127) == 0 && get(dst, 127) == 0)) {
      src = s21_mult_pol(src, dst);
      if (src.value_type == 0) {
        set(&src, 127, 0);
      }
    } else {
      src = s21_mult_pol(src, dst);
      if (src.value_type == 0) {
        set(&src, 127, 1);
      } else if (src.value_type == 1) {
        src.value_type = 2;
      }
    }
  } else if ((src.value_type == 0 && dst.value_type == 1) ||
             (src.value_type == 1 && dst.value_type == 0)) {
    if (src.value_type == 0) {
      if (get(src, 127) == 1) {
        s21_clean(&src);
        src.value_type = 2;
      } else {
        s21_clean(&src);
        src.value_type = 1;
      }
    } else {
      if (get(dst, 127) == 1) {
        s21_clean(&src);
        src.value_type = 2;
      } else {
        s21_clean(&src);
        src.value_type = 1;
      }
    }
  } else if ((src.value_type == 0 && dst.value_type == 2) ||
             (src.value_type == 2 && dst.value_type == 0)) {
    if (src.value_type == 0) {
      if (get(src, 127) == 1) {
        s21_clean(&src);
        src.value_type = 1;
      } else {
        s21_clean(&src);
        src.value_type = 2;
      }
    } else {
      if (get(dst, 127) == 1) {
        s21_clean(&src);
        src.value_type = 1;
      } else {
        s21_clean(&src);
        src.value_type = 2;
      }
    }
  } else if ((src.value_type == 1 && dst.value_type == 1) ||
             (src.value_type == 2 && dst.value_type == 2)) {
    s21_clean(&src);
    src.value_type = 1;
  } else if ((src.value_type == 1 && dst.value_type == 2) ||
             (src.value_type == 2 && dst.value_type == 1)) {
    s21_clean(&src);
    src.value_type = 2;
  } else {
    s21_clean(&src);
    src.value_type = 3;
  }
  return src;
}

s21_decimal s21_mod(s21_decimal src, s21_decimal dst) {
  if (src.value_type == 0 && dst.value_type == 0) {
    src = s21_mod_pol(src, dst, 1);
    if (get(src, 127) == 1) {
      set(&src, 127, 1);
    } else {
      set(&src, 127, 0);
    }
  } else {
    s21_clean(&src);
    src.value_type = 3;
  }
  return src;
}

s21_decimal s21_div(s21_decimal src, s21_decimal dst) {
  if (src.value_type == 0 && dst.value_type == 0) {
    if ((get(src, 127) == 1 && get(dst, 127) == 1) ||
        (get(src, 127) == 0 && get(dst, 127) == 0)) {
      src = s21_mod_pol(src, dst, 2);
      if (src.value_type == 0) {
        set(&src, 127, 0);
      }
    } else {
      src = s21_mod_pol(src, dst, 2);
      if (src.value_type == 0) {
        set(&src, 127, 1);
      } else if (src.value_type == 1) {
        src.value_type = 2;
      }
    }
  } else if (src.value_type == 1 && dst.value_type == 0) {
    s21_clean(&src);
    if (get(dst, 127) == 1) {
      src.value_type = 2;
    } else {
      src.value_type = 1;
    }
  } else if (src.value_type == 2 && dst.value_type == 0) {
    s21_clean(&src);
    if (get(dst, 127) == 1) {
      src.value_type = 1;
    } else {
      src.value_type = 2;
    }
  } else {
    s21_clean(&src);
    src.value_type = 3;
  }
  return src;
}

int s21_is_less(s21_decimal dst1, s21_decimal dst2) {
  int res = 1;
  if (s21_compare(dst1, dst2) == -1) {
    res = 0;
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 1;
  if (s21_compare(dst1, dst2) == -1 || s21_compare(dst1, dst2) == 0) {
    res = 0;
  }
  return res;
}

int s21_is_greater(s21_decimal dst1, s21_decimal dst2) {
  int res = 1;
  if (s21_compare(dst1, dst2) == 1) {
    res = 0;
  }
  return res;
}

int s21_is_greater_or_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 1;
  if (s21_compare(dst1, dst2) == 1 || s21_compare(dst1, dst2) == 0) {
    res = 0;
  }
  return res;
}

int s21_is_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 1;
  if (s21_compare(dst1, dst2) == 0) {
    res = 0;
  }
  return res;
}

int s21_is_not_equal(s21_decimal dst1, s21_decimal dst2) {
  int res = 1;
  if (s21_compare(dst1, dst2) == -1 || s21_compare(dst1, dst2) == 1) {
    res = 0;
  }
  return res;
}

s21_decimal s21_negate(s21_decimal dst) {
  if (dst.value_type == 0) {
    if (get(dst, 127) == 1) {
      set(&dst, 127, 0);
    } else if (get(dst, 127) == 0) {
      set(&dst, 127, 1);
    }
  } else if (dst.value_type == 1) {
    s21_clean(&dst);
    dst.value_type = 2;
  } else if (dst.value_type == 2) {
    s21_clean(&dst);
    dst.value_type = 1;
  } else {
    s21_clean(&dst);
    dst.value_type = 3;
  }
  return dst;
}

s21_decimal s21_floor(s21_decimal dst) {
  if (dst.value_type == 0) {
    s21_decimal src;
    src = s21_truncate(dst);
    if (s21_compare(dst, src) != 0) {
      if (get(dst, 127) == 1) {
        float x = 1;
        if (s21_from_float_to_decimal(x, &src) == 0) {
          dst = s21_sub(dst, src);
        }
      }
      dst = s21_truncate(dst);
    }
  } else {
    s21_clean(&dst);
  }
  return dst;
}

s21_decimal s21_round(s21_decimal dst) {
  if (dst.value_type == 0) {
    s21_decimal flot;
    int minus = 0;
    if (s21_from_float_to_decimal(0.5, &flot) == 0) {
      if (get(dst, 127) == 1) {
        dst = s21_negate(dst);
        minus = 1;
      }
      s21_decimal src = s21_truncate(dst);
      if (s21_compare(s21_sub(dst, src), flot) >= 0) {
        if (s21_from_float_to_decimal(1, &flot) == 0) {
          src = s21_add(src, flot);
        }
      }
      if (minus == 1) {
        set(&src, 127, 1);
      }
      dst = src;
    }
  } else {
    s21_clean(&dst);
  }
  return dst;
}

s21_decimal s21_truncate(s21_decimal dst) {
  if (dst.value_type == 0) {
    short int res[29];
    s21_normalization_decimal(res, dst);
    unsigned int scale = (unsigned int)get_scale(dst);
    if (scale != 0) {
      for (int j = 0; j < 29; j++) {
        if (j >= (29 - (int)scale)) {
          res[j] = 0;
        }
      }
      for (int i = 112; i < 120; i++) {
        set(&dst, i, 0);
      }
      s21_array_sdvig(res, &scale, 29);
      dst = s21_from_array_to_decimal(res, dst);
    }
  } else {
    s21_clean(&dst);
  }
  return dst;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  if (dst != NULL) {
    s21_clean(dst);
    if (src < 0) {
      src *= -1;
      set(dst, 127, 1);
    }
    dst->bits[0] = (unsigned int)src;
    dst->value_type = 0;
  } else {
    error = 1;
  }
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  if (dst != NULL && src.value_type == 0) {
    short int res[29];
    s21_normalization_decimal(res, src);
    short int scale = get_scale(src);
    s21_decimal max_int;
    s21_from_int_to_decimal(2147483647, &max_int);
    if (s21_compare(src, max_int) <= 0 && s21_compare(src, max_int) != -2) {
      *dst = s21_from_array_to_int(res, scale);
      if (get(src, 127) == 1) {
        *dst *= -1;
      }
    } else {
      error = 1;
    }
  } else {
    error = 1;
  }
  return error;
}

void s21_float_norm(short int *src, unsigned int *scale) {
  int i = 28;
  int count_null = 0;
  while (*scale > 8 && i > -1) {
    if (*scale == 9) {
      if (src[i] >= 5) {
        short int sum[29];
        s21_clean_array(sum, 29);
        int idx;
        if (count_null > 0) {
          idx = 58 - count_null;
        } else {
          idx = 29;
        }
        sum[idx - 1] = 1;
        s21_sum(src, sum, 29);
      }
    }
    s21_add_first_null(src);
    count_null++;
    (*scale)--;
    i--;
  }
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  if (dst != NULL && src == src && src != S21_INFL && src != -S21_INFL) {
    s21_clean(dst);
    if (src < 0) {
      set(dst, 127, 1);
    }
    short int exp = get_scale_float(src);
    if (exp <= 95) {
      if (src <= -(1e-28) || src >= 1e-28) {
        set_float(&src, 23, 1);
        short int array_int[128];
        short int array_float[128];
        short int pow[128];
        s21_clean_array(array_int, 128);
        s21_clean_array(array_float, 128);
        s21_clean_array(pow, 128);
        if (exp >= 0) {
          for (int j = 0, i = 23 - exp; j < exp + 1; j++, i++) {
            s21_pow(pow, j, 128);
            if (get_float(src, i) && i >= 0) {
              s21_sum(array_int, pow, 128);
            }
          }
        }
        int bit;
        s21_clean_array(pow, 128);
        pow[0] = 1;
        if (exp < 0) {
          bit = 23;
          for (int i = 0; i < abs(exp) - 1; i++) {
            s21_array_div_2(pow, 127);
          }
        } else {
          bit = 22 - exp;
        }
        for (int i = bit; i > -1; i--) {
          s21_array_div_2(pow, 127);
          if (get_float(src, i)) {
            s21_sum(array_float, pow, 128);
          }
        }
        unsigned int idx = 0;
        int ind = 0;
        for (int i = 0; i < 128; i++) {
          if (array_int[i] != 0) {
            ind = 1;
          }
          if (ind != 0) {
            array_int[idx++] = array_int[i];
            array_int[i] = 0;
          }
        }
        if (idx <= 0) {
          idx = 1;
        }
        for (int i = idx, raz = 1; i < 128 && raz < 128; raz++, i++) {
          array_int[i] = array_float[raz];
        }
        short int res_array[29];
        s21_clean_array(res_array, 29);
        for (int i = 0; i < 29; i++) {
          res_array[i] = array_int[i];
        }
        idx = 29 - idx;
        if (exp >= 0) {
          s21_array_sdvig(res_array, &idx, 29);
          s21_float_norm(res_array, &idx);
          set_scale(dst, idx);
        } else {
          unsigned int scale1 = 28;
          s21_float_norm(res_array, &scale1);
          set_scale(dst, scale1);
        }
        *dst = s21_from_array_to_decimal(res_array, *dst);
        dst->value_type = 0;
      } else {
        error = 1;
        dst->value_type = 0;
      }
    } else {
      error = 1;
      if (src > 0) {
        dst->value_type = 1;
      } else {
        dst->value_type = 2;
      }
    }
  } else if (src != src) {
    error = 1;
    s21_clean(dst);
    dst->value_type = 3;
  } else if (src == S21_INFL) {
    error = 1;
    s21_clean(dst);
    dst->value_type = 1;
  } else if (src == -S21_INFL) {
    error = 1;
    s21_clean(dst);
    dst->value_type = 2;
  } else {
    error = 1;
  }
  return error;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  if (dst != NULL && src.value_type == 0) {
    double res = 0;
    for (int i = 0; i < 96; i++) {
      if (get(src, i)) {
        res += pow(2, i);
      }
    }
    short int scale = get_scale(src);
    while (scale != 0) {
      res /= 10;
      scale--;
    }
    if (get(src, 127) == 1) {
      res *= -1;
    }
    *dst = (float)res;
  } else if (src.value_type == 3) {
    *dst = S21_NANF;
    error = 1;
  } else if (src.value_type == 2) {
    *dst = -S21_INFL;
    error = 1;
  } else if (src.value_type == 1) {
    *dst = S21_INFL;
    error = 1;
  } else {
    error = 1;
  }
  return error;
}

s21_decimal str_to_dec(char binary_number[], s21_decimal *dec,
                       int point_place) {
  if (dec == NULL) {
    s21_decimal res;
    dec = &res;
  }
  init_dec(dec);
  char copy[100];
  int shift = 0;
  if (binary_number[0] == '-') {
    dec->bits[3] += SIGN_MINUS;
    strncpy(copy, binary_number + 1, 97);
  } else {
    strncpy(copy, binary_number, 98);
  }
  dec->bits[3] += point_place << 16;
  int len_number = strlen(copy);
  for (int i = shift; i < len_number; i++) {
    int bit_place = (i - shift) % 32;
    int int_place = (i - shift) / 32;
    int bit = copy[len_number - 1 - i + shift] - '0';
    dec->bits[int_place] += bit << bit_place;
  }

  return *dec;
}

void init_dec(s21_decimal *dec) {
  dec->bits[0] = 0;
  dec->bits[1] = 0;
  dec->bits[2] = 0;
  dec->bits[3] = 0;
  dec->value_type = s21_NORMAL_VALUE;
}

void s21_print_decimal(s21_decimal src) {  // delete whith finish
  short int res[29];
  s21_normalization_decimal(res, src);
  short int scale = get_scale(src);
  if (src.value_type == 0) {
    printf("s21_decimal = ");
    if (get(src, 127) == 1) {
      printf("-");
    }
    int ind = 0;
    for (int j = 0; j < 29; j++) {
      if (scale != 0 && j == (29 - scale)) {
        if (ind == 0) {
          printf("0");
          ind = 1;
        }
        printf(".");
      }
      if (res[j] != 0) {
        ind = 1;
      }
      if (ind != 0) {
        printf("%hd", res[j]);
      }
    }
    printf("\n");
  } else if (src.value_type == 1) {
    printf("inf\n");
  } else if (src.value_type == 2) {
    printf("-inf\n");
  } else if (src.value_type == 3) {
    printf("nan\n");
  }
}
