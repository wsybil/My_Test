#include "s21_decimal.h"

s21_decimal s21_add(s21_decimal src1, s21_decimal src2) {
  if (!s21_get_type_for_add(src1, src2)) {
    int flag = 0, scale, sign_1 = s21_get_sign(src1),
        sign_2 = s21_get_sign(src2);
    super_decimal first, second, result;
    s21_converting_decimals(&src1, &src2, &first, &second);
    s21_init_sdecimal(&result);

    s21_set_scale_for_add(&first, &second, &result);
    scale = result.bits[6];

    if (!s21_is_greater_for_super(first, second)) {
      if (sign_1 && sign_2) {
        result = s21_add_for_super(first, second);
        flag = 1;
      } else if (sign_1 && !sign_2) {
        if (s21_is_greater_for_super(first, second)) {
          result = s21_sub_for_super(second, first);
        } else {
          result = s21_sub_for_super(first, second);
          flag = 1;
        }
      } else if (!sign_1 && sign_2) {
        result = s21_sub_for_super(first, second);
      } else {
        result = s21_add_for_super(first, second);
      }
    } else {
      if (sign_1 && sign_2) {
        result = s21_add_for_super(first, second);
        flag = 1;
      } else if (!sign_1 && sign_2) {
        if (s21_is_greater_for_super(first, second)) {
          result = s21_sub_for_super(second, first);
          flag = 1;
        } else {
          result = s21_sub_for_super(first, second);
        }
      } else if (sign_1 && !sign_2) {
        result = s21_sub_for_super(second, first);
      } else {
        result = s21_add_for_super(first, second);
      }
    }
    if (s21_check_overflow(result) && scale) {
      s21_reduction_sdecimal(&result, &scale);
    }
    if (s21_check_overflow(result) && !scale) {
      src1.value_type = flag ? 2 : 1;
      s21_clean(&src1);
    } else {
      s21_converting_decimals(&src1, &src2, &first, &second);
      if (!s21_check_empty_super(first) && s21_get_sscale(first)) {
        scale = s21_get_sscale(first);
      } else if (!s21_check_empty_super(second) && s21_get_sscale(second)) {
        scale = s21_get_sscale(second);
      }
      s21_turn_into_normal(&result, &src1);
      s21_set_scale(&src1, scale);
      if (flag) s21_set_sign(&src1, 1);
    }
  } else {
    src1.value_type = s21_get_type_for_add(src1, src2);
    s21_clean(&src1);
  }
  return src1;
}

s21_decimal s21_mul(s21_decimal src1, s21_decimal src2) {
  if (!s21_get_type_for_mul(src1, src2)) {
    int sign_1 = s21_get_sign(src1), sign_2 = s21_get_sign(src2);
    super_decimal first, second, result, sub;
    s21_converting_decimals(&src1, &src2, &first, &second);
    int scl_1 = s21_get_sscale(first), scl_2 = s21_get_sscale(second);
    s21_normal_scale(&first, &scl_1);
    s21_normal_scale(&second, &scl_2);
    int scale = scl_1 + scl_2;
    s21_init_sdecimal(&result);
    s21_init_sdecimal(&sub);
    if (s21_check_empty_super(first) && s21_check_empty_super(second)) {
      for (int i = 0; i < 96; i++) {
        int score = i;
        for (int j = 0; j < 96; j++) {
          if (s21_get_bit(first, j) && s21_get_bit(second, i)) {
            s21_set_bit(&sub, j, 1);
          } else {
            s21_set_bit(&sub, j, 0);
          }
        }
        if (i > 0) {
          while (score) {
            s21_great_shift(&sub);
            score--;
          }
        }
        result = s21_add_for_super(result, sub);
        s21_init_sdecimal(&sub);
      }
    } else {
      s21_converting_decimals(&src1, &src2, &first, &second);
      if (!s21_check_empty_super(first)) {
        scale = s21_get_sscale(first);
      } else if (!s21_check_empty_super(second)) {
        scale = s21_get_sscale(second);
      }
      s21_init_sdecimal(&result);
    }
    if (s21_check_overflow(result) && scale) {
      s21_reduction_sdecimal(&result, &scale);
    }
    while (scale > 28) {
      if (!s21_check_overflow(result) && s21_check_empty_super(result)) {
        s21_super_round_scale(&result);
        scale--;
      } else {
        scale--;
      }
    }

    if (scale <= 0 && s21_check_overflow(result) && sign_1 && !sign_2) {
      s21_init_decimal(&src1);
      src1.value_type = 2;
    } else if (scale <= 0 && s21_check_overflow(result) && !sign_1 && sign_2) {
      s21_init_decimal(&src1);
      src1.value_type = 2;
    } else if (scale <= 0 && s21_check_overflow(result)) {
      s21_init_decimal(&src1);
      src1.value_type = 1;
    } else {
      result.bits[6] = scale;
      s21_turn_into_normal(&result, &src1);
      s21_set_scale(&src1, src1.bits[3]);

      if (sign_1 && !sign_2)
        s21_set_sign(&src1, 1);
      else if (!sign_1 && sign_2)
        s21_set_sign(&src1, 1);
    }
  } else {
    src1.value_type = s21_get_type_for_mul(src1, src2);
    s21_clean(&src1);
  }
  return src1;
}

s21_decimal s21_sub(s21_decimal src1, s21_decimal src2) {
  if (!s21_get_type_for_sub(src1, src2)) {
    int flag = 0, scale, sign_1 = s21_get_sign(src1),
        sign_2 = s21_get_sign(src2);
    super_decimal first, second, result;
    s21_converting_decimals(&src1, &src2, &first, &second);
    s21_init_sdecimal(&result);
    s21_set_scale_for_sub(&first, &second, &result);
    scale = result.bits[6];

    if (!s21_is_greater_or_equal_for_super(first, second)) {
      if (sign_1 && !sign_2) {
        result = s21_add_for_super(first, second);
        flag = 1;
      } else if (sign_1 && sign_2) {
        result = s21_sub_for_super(first, second);
        flag = 1;
      } else if (!sign_1 && sign_2) {
        result = s21_add_for_super(first, second);
      } else {
        result = s21_sub_for_super(first, second);
      }
    } else {
      if (sign_1 && !sign_2) {
        result = s21_add_for_super(first, second);
        flag = 1;
      } else if (!sign_1 && sign_2) {
        result = s21_add_for_super(first, second);
      } else if (sign_1 && sign_2) {
        result = s21_sub_for_super(second, first);
      } else {
        result = s21_sub_for_super(second, first);
        flag = 1;
      }
    }
    if (s21_check_overflow(result) && scale) {
      s21_reduction_sdecimal(&result, &scale);
    }
    if (s21_check_overflow(result) && scale <= 0) {
      src1.value_type = flag ? 2 : 1;
      s21_clean(&src1);
    } else {
      s21_converting_decimals(&src1, &src2, &first, &second);
      if (!s21_check_empty_super(first) && s21_get_sscale(first)) {
        scale = s21_get_sscale(first);
      } else if (!s21_check_empty_super(second) && s21_get_sscale(second)) {
        scale = s21_get_sscale(second);
      }
      s21_turn_into_normal(&result, &src1);
      s21_set_scale(&src1, scale);
      if (flag) s21_set_sign(&src1, 1);
    }
  } else {
    src1.value_type = s21_get_type_for_sub(src1, src2);
    s21_clean(&src1);
  }
  return src1;
}

s21_decimal s21_div(s21_decimal src1, s21_decimal src2) {
  int sign_1 = s21_get_sign(src1), sign_2 = s21_get_sign(src2);
  if (!s21_get_type_for_div(src1, src2, sign_1, sign_2)) {
    super_decimal first, second, result, one_plus;
    s21_converting_decimals(&src1, &src2, &first, &second);
    s21_init_sdecimal(&result);
    s21_init_sdecimal(&one_plus);
    one_plus.bits[0] = 1;

    s21_set_scale_for_div(&first, &second);
    second = s21_normal_div(first, second);

    int scale = 0;
    scale -= second.bits[6];

    while (s21_check_empty_super(first)) {
      while (!s21_is_less_for_super(first, second)) {
        s21_shift_mul_ten(&first);
        s21_shift_mul_ten(&result);
        scale++;
      }
      while (!s21_is_greater_or_equal_for_super(first, second)) {
        first = s21_sub_for_super(first, second);
        result = s21_add_for_super(result, one_plus);
      }
      if (scale > 28) {
        while (scale != 28 && !s21_check_overflow(result) &&
               s21_check_empty_super(first)) {
          s21_super_round_scale(&result);
          scale--;
        }
        break;
      }
    }
    if (scale < 0) {
      scale *= -1;
      while (scale) {
        s21_shift_mul_ten(&result);
        scale--;
      }
    }
    if (s21_check_overflow(result) && scale) {
      s21_reduction_sdecimal(&result, &scale);
    }
    if (scale <= 0 && s21_check_overflow(result) &&
        ((!sign_1 && !sign_2) || (sign_1 && sign_2))) {
      s21_init_decimal(&src1);
      src1.value_type = 1;
    } else if (scale <= 0 && s21_check_overflow(result)) {
      s21_init_decimal(&src1);
      src1.value_type = 2;
    } else {
      s21_normal_scale(&result, &scale);
      s21_turn_into_normal(&result, &src1);
      if (s21_check_empty(src1)) s21_set_scale(&src1, scale);

      if (sign_1 && !sign_2) {
        s21_set_sign(&src1, 1);
      } else if (!sign_1 && sign_2) {
        s21_set_sign(&src1, 1);
      }
    }
  } else if (s21_get_type_for_div(src1, src2, sign_1, sign_2) == 6) {
    if (sign_1 && !sign_2) {
      s21_set_sign(&src1, 1);
    } else if (!sign_1 && sign_2) {
      s21_set_sign(&src1, 1);
    }
  } else if (s21_get_type_for_div(src1, src2, sign_1, sign_2) == 4) {
    src1.value_type = 0;
    s21_clean(&src1);
  } else if (s21_get_type_for_div(src1, src2, sign_1, sign_2) == 5) {
    src1.value_type = 0;
    s21_clean(&src1);
    s21_set_sign(&src1, 1);
  } else {
    src1.value_type = s21_get_type_for_div(src1, src2, sign_1, sign_2);
    s21_clean(&src1);
  }
  return src1;
}

s21_decimal s21_mod(s21_decimal src1, s21_decimal src2) {
  if (!s21_get_type_for_mod(src1, src2)) {
    int sign_1 = s21_get_sign(src1);
    super_decimal first, second, result;
    s21_init_sdecimal(&result);

    s21_converting_decimals(&src1, &src2, &first, &second);
    int scale_1 = s21_get_sscale(first), scale_2 = s21_get_sscale(second);

    if (s21_check_empty_super(first)) {
      s21_set_scale_for_div(&first, &second);
      if (!s21_is_greater_or_equal_for_super(first, second)) {
        result = s21_mod_for_super(first, second);
      } else {
        s21_normal_scale(&first, &scale_2);
        result = first;
      }
      s21_turn_into_normal(&result, &src1);
      if (scale_1 > scale_2) {
        s21_set_scale(&src1, scale_1);
      } else {
        s21_set_scale(&src1, scale_2);
      }
      if (sign_1) s21_set_sign(&src1, 1);
    }

  } else if (s21_get_type_for_mod(src1, src2) == 4) {
    src1.value_type = 0;
  } else if (s21_get_type_for_mod(src1, src2) == 2) {
    src1.value_type = 0;
    s21_clean(&src1);
  } else {
    src1.value_type = s21_get_type_for_mod(src1, src2);
    s21_clean(&src1);
  }
  return src1;
}

int s21_is_less(s21_decimal src1, s21_decimal src2) {
  int flag = 1;
  if (s21_get_type_for_less(src1, src2) < 0) {
    super_decimal first, second;
    s21_converting_decimals(&src1, &src2, &first, &second);
    int scl_1 = s21_get_sscale(first), scl_2 = s21_get_sscale(second);
    s21_normal_scale(&first, &scl_1);
    s21_normal_scale(&second, &scl_2);
    int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
    int sign_one = s21_get_sign(src1), sign_two = s21_get_sign(src2);
    super_decimal scale_dec;
    s21_init_sdecimal(&scale_dec);
    scale_dec = s21_set_sscale(scale_dec, scale);
    if (scl_1 > scl_2) {
      second = s21_mul_for_super(second, scale_dec);
    } else {
      first = s21_mul_for_super(first, scale_dec);
    }

    scale = s21_is_less_for_super(first, second);
    if (s21_check_bits_super(&first, &second) < 0 &&
        s21_check_empty_super(first) && s21_check_empty_super(second)) {
      if (sign_one && !sign_two) {
        flag = 0;
      }
    } else {
      if (sign_one && !sign_two && s21_check_empty_super(first) &&
          s21_check_empty_super(second)) {
        flag = 0;
      } else if (sign_one && sign_two && scale) {
        flag = 0;
      } else if (!sign_one && !sign_two && !scale) {
        flag = 0;
      }
    }
  } else {
    flag = s21_get_type_for_less(src1, src2);
  }
  return flag;
}

int s21_is_less_or_equal(s21_decimal src1, s21_decimal src2) {
  return (s21_is_less(src1, src2) && s21_is_equal(src1, src2)) ? 1 : 0;
}

int s21_is_greater(s21_decimal src1, s21_decimal src2) {
  int flag = 1;
  if (s21_get_type_for_great(src1, src2) < 0) {
    super_decimal first, second;
    s21_converting_decimals(&src1, &src2, &first, &second);
    int scl_1 = s21_get_sscale(first), scl_2 = s21_get_sscale(second);
    s21_normal_scale(&first, &scl_1);
    s21_normal_scale(&second, &scl_2);
    int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
    int sign_one = s21_get_sign(src1), sign_two = s21_get_sign(src2);
    super_decimal scale_dec;
    s21_init_sdecimal(&scale_dec);
    scale_dec = s21_set_sscale(scale_dec, scale);
    if (scl_1 > scl_2) {
      second = s21_mul_for_super(second, scale_dec);
    } else {
      first = s21_mul_for_super(first, scale_dec);
    }

    scale = s21_is_less_for_super(first, second);
    if (s21_check_bits_super(&first, &second) < 0 &&
        s21_check_empty_super(first) && s21_check_empty_super(second)) {
      if (!sign_one && sign_two) {
        flag = 0;
      }
    } else {
      if (!sign_one && sign_two && s21_check_empty_super(first) &&
          s21_check_empty_super(second)) {
        flag = 0;
      } else if (sign_one && sign_two && !scale) {
        flag = 0;
      } else if (!sign_one && !sign_two && scale) {
        flag = 0;
      }
    }
  } else {
    flag = s21_get_type_for_great(src1, src2);
  }
  return flag;
}

int s21_is_greater_or_equal(s21_decimal src1, s21_decimal src2) {
  return (s21_is_greater(src1, src2) && s21_is_equal(src1, src2)) ? 1 : 0;
}

int s21_is_equal(s21_decimal src1, s21_decimal src2) {
  int flag = 0;
  if (!s21_get_type_for_eq(src1, src2)) {
    int sign_1 = s21_get_sign(src1), sign_2 = s21_get_sign(src2);
    super_decimal first, second;
    s21_init_sdecimal(&first);
    s21_init_sdecimal(&second);
    s21_converting_decimals(&src1, &src2, &first, &second);
    int scl_1 = s21_get_sscale(first), scl_2 = s21_get_sscale(second);
    s21_normal_scale(&first, &scl_1);
    s21_normal_scale(&second, &scl_2);
    if (s21_check_bits_super(&first, &second) != -1) {
      flag = 1;
    } else if (sign_1 != sign_2 && s21_check_empty_super(first) &&
               s21_check_empty_super(second)) {
      flag = 1;
    } else if (scl_1 != scl_2 && s21_check_empty_super(first) &&
               s21_check_empty_super(second)) {
      flag = 1;
    }

  } else {
    flag = s21_get_type_for_eq(src1, src2);
  }
  return flag;
}

int s21_is_not_equal(s21_decimal src1, s21_decimal src2) {
  int flag = 1;
  if (s21_get_type_for_not_eq(src1, src2) == 2) {
    int sign_1 = s21_get_sign(src1), sign_2 = s21_get_sign(src2);
    super_decimal first, second;
    s21_init_sdecimal(&first);
    s21_init_sdecimal(&second);
    s21_converting_decimals(&src1, &src2, &first, &second);
    int scl_1 = s21_get_sscale(first), scl_2 = s21_get_sscale(second);
    s21_normal_scale(&first, &scl_1);
    s21_normal_scale(&second, &scl_2);
    if (s21_check_bits_super(&first, &second) != -1) {
      flag = 0;
    } else if (sign_1 != sign_2 && s21_check_empty_super(first) &&
               s21_check_empty_super(second)) {
      flag = 0;
    } else if (scl_1 != scl_2 && s21_check_empty_super(first) &&
               s21_check_empty_super(second)) {
      flag = 0;
    }
  } else {
    flag = s21_get_type_for_not_eq(src1, src2);
  }
  return flag;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int flag = 0;
  if (dst != NULL) {
    int sign = 0;
    super_decimal buf;
    s21_init_decimal(dst);
    s21_init_sdecimal(&buf);
    s21_turn_into_super(dst, &buf);

    if (src < 0) {
      sign = 1;
      src *= -1;
    }
    for (int i = 0; i < 96; i++) {
      if (src % 2 != 0) {
        s21_set_bit(&buf, i, 1);
        src -= 1;
        src /= 2;
      } else {
        s21_set_bit(&buf, i, 0);
        src /= 2;
      }
    }

    s21_turn_into_normal(&buf, dst);
    if (sign) s21_set_sign(dst, 1);
  } else {
    flag = 1;
  }
  return flag;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int flag = 0;
  if (dst == NULL) {
    flag = 1;
  } else {
    s21_init_decimal(dst);
    if (isinf(src) || isnan(src) || src > MAX_DEC || src < -MAX_DEC) {
      flag = 1;
      if (src == INFINITY || src > MAX_DEC) {
        dst->value_type = 1;
      } else if (src == -INFINITY || src < -MAX_DEC) {
        dst->value_type = 2;
      } else if (isnan(src)) {
        dst->value_type = 3;
      }
    } else {
      if (src == (int)src && (int)src < 1000000) {
        s21_from_int_to_decimal((int)src, dst);
      } else {
        int result, total, sign = 0, scale = 0;
        if (src < 0) {
          sign = 1;
          src *= -1;
        }
        result = s21_from_float_to_int(src);

        if (src < 1000000) {
          total = (int)src;
          int tmp = result;
          while (tmp > total) {
            tmp /= 10;
            scale++;
          }
        }

        super_decimal buf;
        s21_init_decimal(dst);
        s21_init_sdecimal(&buf);
        s21_turn_into_super(dst, &buf);

        for (int i = 0; i < 96; i++) {
          if (result % 2 != 0) {
            s21_set_bit(&buf, i, 1);
            result -= 1;
            result /= 2;
          } else {
            s21_set_bit(&buf, i, 0);
            result /= 2;
          }
        }

        total = s21_check_exp(src);
        if (s21_check_neg_exp(src) && total > 1) {
          scale += total - 1;
          while (scale > 28 && s21_check_empty_super(buf)) {
            s21_super_round_scale(&buf);
            scale--;
          }
          if (!s21_check_empty_super(buf)) {
            scale = 0;
          }
        } else {
          while (total > 6) {
            s21_shift_mul_ten(&buf);
            total--;
          }
        }

        s21_turn_into_normal(&buf, dst);
        if (scale < 29) {
          s21_set_scale(dst, scale);
          if (sign) s21_set_sign(dst, 1);
        }
      }
    }
  }
  return flag;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int flag = 0;
  if (!src.value_type && dst != NULL) {
    int sign = s21_get_sign(src);
    src = s21_truncate(src);
    *dst = 0;
    super_decimal buf;
    s21_init_sdecimal(&buf);
    s21_turn_into_super(&src, &buf);

    for (int i = 95; i >= 0; i--) {
      if (s21_get_bit(buf, i)) {
        *dst += s21_pow(2, i);
      }
    }
    if (sign) {
      *dst *= -1;
    }
  } else {
    flag = 1;
  }
  return flag;
}

int s21_from_decimal_to_float(s21_decimal src, float *dec) {
  int flag = 0;
  if (dec == NULL) {
    flag = 1;
  } else {
    if (src.value_type == 1) {
      *dec = INFINITY;
      flag = 1;
    } else if (src.value_type == 2) {
      *dec = -INFINITY;
      flag = 1;
    } else if (src.value_type == 3) {
      *dec = NAN;
      flag = 1;
    } else {
      double result = 0;
      int sign = s21_get_sign(src);
      super_decimal buf;
      s21_init_sdecimal(&buf);
      s21_turn_into_super(&src, &buf);

      for (int i = 0; i < 192; i++) {
        result += pow(2, i) * s21_get_bit(buf, i);
      }

      result /= pow(10, s21_get_sscale(buf));

      if (sign) {
        result *= -1;
      }
      *dec = result;
    }
  }
  return flag;
}

s21_decimal s21_floor(s21_decimal src) {
  if (src.value_type == 0) {
    int flag = 0;
    if (s21_check_empty(src)) {
      flag = 1;
    }
    int sign = s21_get_sign(src), scale = s21_get_scale(src);
    src = s21_truncate(src);
    super_decimal one_plus, buf;
    s21_init_sdecimal(&buf);
    s21_init_sdecimal(&one_plus);
    s21_turn_into_super(&src, &buf);
    one_plus.bits[0] = 1;

    if (flag) {
      if (sign && scale) {
        buf = s21_add_for_super(buf, one_plus);
      }
    } else {
      s21_init_sdecimal(&buf);
    }

    s21_turn_into_normal(&buf, &src);
    s21_set_sign(&src, sign);
  } else {
    s21_clean(&src);
  }
  return src;
}

s21_decimal s21_round(s21_decimal src) {
  if (src.value_type == 0) {
    int sign = s21_get_sign(src), scale = s21_get_scale(src);
    if (s21_check_empty(src)) {
      super_decimal one_plus, buf;
      s21_init_sdecimal(&buf);
      s21_init_sdecimal(&one_plus);
      s21_turn_into_super(&src, &buf);
      one_plus.bits[0] = 1;

      s21_normal_scale(&buf, &scale);

      if (scale > 1) {
        scale--;
        s21_super_truncate(&buf, scale);
        s21_super_round_scale(&buf);
      } else if (scale == 1) {
        s21_super_round_scale(&buf);
      }

      buf.bits[6] = 0;
      s21_turn_into_normal(&buf, &src);
      s21_set_sign(&src, sign);

    } else {
      s21_init_decimal(&src);
      s21_set_sign(&src, sign);
    }
  } else {
    s21_clean(&src);
  }
  return src;
}

s21_decimal s21_truncate(s21_decimal src) {
  if (src.value_type == 0) {
    int sign = s21_get_sign(src);
    if (s21_check_empty(src)) {
      super_decimal result;
      s21_init_sdecimal(&result);
      s21_turn_into_super(&src, &result);
      int scale = s21_get_sscale(result);

      s21_normal_scale(&result, &scale);
      if (scale) s21_super_truncate(&result, scale);
      result.bits[6] = 0;

      s21_turn_into_normal(&result, &src);
    } else {
      s21_init_decimal(&src);
    }
    s21_set_sign(&src, sign);
  } else {
    s21_clean(&src);
  }
  return src;
}

s21_decimal s21_negate(s21_decimal src) {
  if (!src.value_type) {
    s21_swap_bit(&src, 127);
  } else {
    s21_clean(&src);
  }
  return src;
}

void s21_init_decimal(s21_decimal *src) {
  for (int i = 0; i < 4; i++) src->bits[i] = 0;
  src->value_type = s21_NORMAL_VALUE;
}

void s21_clean(s21_decimal *src) {
  for (int i = 0; i < 4; i++) src->bits[i] = 0;
}

int s21_get_scale(s21_decimal src) { return ((src.bits[3] & 0xFF0000) >> 16); }

int s21_get_sign(s21_decimal src) {
  super_decimal x;
  s21_init_sdecimal(&x);
  s21_turn_into_super(&src, &x);
  return s21_get_bit(x, 223);
}

void s21_set_scale(s21_decimal *src, int scale) { src->bits[3] = scale << 16; }

void s21_set_sign(s21_decimal *src, int sign) {
  super_decimal x;
  s21_init_sdecimal(&x);
  s21_turn_into_super(src, &x);
  if (sign == 0)
    s21_set_bit(&x, 223, 0);
  else
    s21_set_bit(&x, 223, 1);
  s21_turn_into_normal(&x, src);
}

void s21_swap_bit(s21_decimal *src, const int n) {
  src->bits[n / 32] ^= (1 << (n % 32));
}

int s21_from_float_to_int(float src) {
  char buffer[33], buffer_2[33];
  int result = 0, lenght = 0;
  snprintf(buffer, sizeof(buffer), "%.6e", src);
  for (int i = 0; i < 33; i++) {
    if (buffer[i] == 'e' || buffer[i] == 'E') {
      buffer[i] = '\0';
      break;
    }
  }
  int j = 0;
  lenght = strlen(buffer);
  for (int i = 0; i < lenght; i++) {
    if (buffer[i] != '.') {
      buffer_2[j] = buffer[i];
      j++;
    }
  }
  lenght = strlen(buffer_2);
  for (int i = 0; i < lenght; i++) {
    result += (buffer_2[i] + '0' - 96);
    if (i < lenght - 1) result *= 10;
  }
  return result;
}

void s21_turn_into_super(s21_decimal *src1, super_decimal *src2) {
  src2->bits[0] = src1->bits[0];
  src2->bits[1] = src1->bits[1];
  src2->bits[2] = src1->bits[2];
  src2->bits[6] = src1->bits[3];
  src2->value_type = src1->value_type;
}

void s21_turn_into_normal(super_decimal *src1, s21_decimal *src2) {
  src2->bits[0] = src1->bits[0];
  src2->bits[1] = src1->bits[1];
  src2->bits[2] = src1->bits[2];
  src2->bits[3] = src1->bits[6];
  src2->value_type = src1->value_type;
}

void s21_converting_decimals(s21_decimal *src1, s21_decimal *src2,
                             super_decimal *dst1, super_decimal *dst2) {
  s21_init_sdecimal(dst1);
  s21_init_sdecimal(dst2);
  s21_turn_into_super(src1, dst1);
  s21_turn_into_super(src2, dst2);
}

int s21_check_exp(float src) {
  char buffer[33];
  int result = 0;
  snprintf(buffer, sizeof(buffer), "%e", src);
  for (int i = 0; buffer[i] != '\0'; i++) {
    if (buffer[i] == '+' || buffer[i] == '-') {
      i++;
      for (int j = i; buffer[j] != '\0'; j++) {
        result += buffer[j] + '0' - 96;
        if (buffer[j + 1] != '\0') result *= 10;
      }
      break;
    }
  }
  return result;
}

int s21_check_neg_exp(float src) {
  char buffer[33];
  int flag = 0;
  snprintf(buffer, sizeof(buffer), "%e", src);
  for (int i = 0; buffer[i] != '\0'; i++) {
    if (buffer[i] == '-') {
      flag = 1;
    }
  }
  return flag;
}

int s21_check_empty(s21_decimal src) {
  int flag = 0;
  if (src.bits[2]) flag = 1;
  if (src.bits[1]) flag = 1;
  if (src.bits[0]) flag = 1;
  return flag;
}

long long s21_pow(int src1, int src2) {
  long long z = 1;
  while (src2) {
    z = z * src1;
    src2--;
  }
  return z;
}

super_decimal s21_add_for_super(super_decimal src1, super_decimal src2) {
  int v_ume = 0;
  super_decimal result;
  s21_init_sdecimal(&result);

  for (int i = 0; i < 192; i++) {
    if (s21_get_bit(src1, i) && s21_get_bit(src2, i)) {
      if (v_ume == 0) {
        s21_set_bit(&result, i, 0);
      } else {
        s21_set_bit(&result, i, 1);
      }
      v_ume = 1;
    } else if (s21_get_bit(src1, i) || s21_get_bit(src2, i)) {
      if (v_ume == 1) {
        s21_set_bit(&result, i, 0);
      } else {
        s21_set_bit(&result, i, 1);
      }
    } else {
      if (v_ume == 1) {
        s21_set_bit(&result, i, 1);
        v_ume = 0;
      } else {
        s21_set_bit(&result, i, 0);
      }
    }
  }
  return result;
}

super_decimal s21_sub_for_super(super_decimal src1, super_decimal src2) {
  super_decimal result;
  s21_init_sdecimal(&result);
  int v_ume = 0;
  for (int i = 0; i < 192; i++) {
    if (s21_get_bit(src1, i) && s21_get_bit(src2, i)) {
      if (v_ume == 1) s21_set_bit(&src1, i, 0);
    } else if (s21_get_bit(src1, i) && !s21_get_bit(src2, i)) {
      if (v_ume == 1) {
        s21_set_bit(&src1, i, 0);
        v_ume = 0;
      }
    } else if (!s21_get_bit(src1, i) && s21_get_bit(src2, i)) {
      if (v_ume == 1) s21_set_bit(&src1, i, 1);
      v_ume = 1;
    } else if (!s21_get_bit(src1, i) && !s21_get_bit(src2, i)) {
      if (v_ume == 1) s21_set_bit(&src1, i, 1);
    }
  }
  for (int i = 0; i < 192; i++) {
    if (s21_get_bit(src1, i) && s21_get_bit(src2, i)) {
      s21_set_bit(&result, i, 0);
    } else if (s21_get_bit(src1, i) && !s21_get_bit(src2, i)) {
      s21_set_bit(&result, i, 1);
    } else if (!s21_get_bit(src1, i) && s21_get_bit(src2, i)) {
      s21_set_bit(&result, i, 1);
    } else if (!s21_get_bit(src1, i) && !s21_get_bit(src2, i)) {
      s21_set_bit(&result, i, 0);
    }
  }
  return result;
}

super_decimal s21_mul_for_super(super_decimal src1, super_decimal src2) {
  super_decimal result, sub;
  s21_init_sdecimal(&result);
  s21_init_sdecimal(&sub);

  for (int i = 0; i < 192; i++) {
    int score = i;
    for (int j = 0; j < 192; j++) {
      if (s21_get_bit(src1, j) && s21_get_bit(src2, i)) {
        s21_set_bit(&sub, j, 1);
      } else {
        s21_set_bit(&sub, j, 0);
      }
    }
    if (i > 0) {
      while (score) {
        s21_great_shift(&sub);
        score--;
      }
    }
    result = s21_add_for_super(result, sub);
    s21_init_sdecimal(&sub);
  }
  return result;
}

super_decimal s21_div_for_super(super_decimal src1, super_decimal src2) {
  super_decimal result, one_plus;
  s21_init_sdecimal(&one_plus);
  s21_init_sdecimal(&result);
  one_plus.bits[0] = 1;
  int score = 0;

  while (s21_check_empty_super(src1) && score < 1000) {
    while (!s21_is_less_for_super(src1, src2)) {
      s21_shift_mul_ten(&src1);
      s21_shift_mul_ten(&result);
      score++;
    }
    src1 = s21_sub_for_super(src1, src2);
    result = s21_add_for_super(result, one_plus);
  }
  result.bits[6] = score;
  return result;
}

super_decimal s21_mod_for_super(super_decimal src1, super_decimal src2) {
  src2 = s21_normal_div(src1, src2);
  unsigned int digit = src2.bits[6];
  if (digit) {
    for (unsigned int i = 0; i != digit + 1; i++) {
      while (!s21_is_greater_or_equal_for_super(src1, src2)) {
        src1 = s21_sub_for_super(src1, src2);
      }
      if (i != digit) s21_shift_mul_ten(&src1);
    }
  } else {
    while (!s21_is_greater_or_equal_for_super(src1, src2)) {
      src1 = s21_sub_for_super(src1, src2);
    }
  }
  super_decimal divider = s21_one_div(src1);
  if (divider.bits[6]) {
    unsigned int digit_2 = divider.bits[6] - digit;
    if (digit) {
      src2 = s21_one_div(src1);
      src1 = s21_div_for_super(src1, src2);
    }
    divider = s21_one_div(src1);
    digit = divider.bits[6];
    while (digit != digit_2) {
      s21_shift_mul_ten(&src1);
      digit++;
    }
  }
  return src1;
}

int s21_is_less_for_super(super_decimal src1, super_decimal src2) {
  int flag = 1;
  int num_bits;

  num_bits = s21_check_bits_super(&src1, &src2);

  if (num_bits >= 0) {
    if (src1.bits[num_bits] < src2.bits[num_bits]) {
      flag = 0;
    }
  }
  return flag;
}

int s21_is_greater_for_super(super_decimal src1, super_decimal src2) {
  int flag = 1, num_bits;

  num_bits = s21_check_bits_super(&src1, &src2);

  if (num_bits >= 0) {
    if (src1.bits[num_bits] > src2.bits[num_bits]) {
      flag = 0;
    }
  }
  return flag;
}

int s21_is_greater_or_equal_for_super(super_decimal src1, super_decimal src2) {
  int flag = 0, num_bits;

  num_bits = s21_check_bits_super(&src1, &src2);

  if (num_bits >= 0) {
    if (src1.bits[num_bits] > src2.bits[num_bits]) {
      flag = 0;
    } else {
      flag = 1;
    }
  }
  return flag;
}

void s21_init_sdecimal(super_decimal *src) {
  for (int i = 0; i < 7; i++) src->bits[i] = 0;
  src->value_type = s21_NORMAL_VALUE;
}

int s21_get_bit(super_decimal src, const int n) {
  return (src.bits[n / 32] & (1 << (n % 32))) != 0;
}

int s21_get_sscale(super_decimal src) {
  return ((src.bits[6] & 0xFF0000) >> 16);
}

super_decimal s21_set_sscale(super_decimal src, int scale) {
  s21_init_sdecimal(&src);
  src.bits[0] = 1;
  while (scale) {
    s21_shift_mul_ten(&src);
    scale--;
  }
  return src;
}

void s21_set_bit(super_decimal *src, const int n, int bit) {
  if (src != NULL) {
    unsigned int mask = 1u << (n % 32);
    if (bit == 0)
      src->bits[n / 32] &= ~mask;
    else
      src->bits[n / 32] |= mask;
  }
}

void s21_set_scale_for_add(super_decimal *src1, super_decimal *src2,
                           super_decimal *result) {
  int scl_1 = s21_get_sscale(*src1), scl_2 = s21_get_sscale(*src2);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  super_decimal scale_dec;
  s21_init_sdecimal(&scale_dec);
  scale_dec = s21_set_sscale(scale_dec, scale);
  if (scl_1 > scl_2) {
    *src2 = s21_mul_for_super(*src2, scale_dec);
    result->bits[6] = scl_1;
  } else {
    *src1 = s21_mul_for_super(*src1, scale_dec);
    result->bits[6] = scl_2;
  }
}

void s21_set_scale_for_div(super_decimal *src1, super_decimal *src2) {
  int scl_1 = s21_get_sscale(*src1), scl_2 = s21_get_sscale(*src2);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  super_decimal scale_dec;
  s21_init_sdecimal(&scale_dec);
  scale_dec = s21_set_sscale(scale_dec, scale);

  if (scl_1 > scl_2) {
    *src2 = s21_mul_for_super(*src2, scale_dec);
  } else {
    *src1 = s21_mul_for_super(*src1, scale_dec);
  }
}

void s21_set_scale_for_sub(super_decimal *src1, super_decimal *src2,
                           super_decimal *result) {
  int scl_1 = s21_get_sscale(*src1), scl_2 = s21_get_sscale(*src2);
  int scale = (scl_1 > scl_2) ? (scl_1 - scl_2) : (scl_2 - scl_1);
  super_decimal scale_dec;
  s21_init_sdecimal(&scale_dec);
  scale_dec = s21_set_sscale(scale_dec, scale);

  if (scl_1 > scl_2) {
    *src2 = s21_mul_for_super(*src2, scale_dec);
    result->bits[6] = scl_1;
  } else {
    *src1 = s21_mul_for_super(*src1, scale_dec);
    result->bits[6] = scl_2;
  }
}

int s21_get_type_for_add(s21_decimal src1, s21_decimal src2) {
  int result = 0;
  if (!src1.value_type && !src2.value_type) {
    result = 0;
  } else if ((!src1.value_type && src2.value_type == 1) ||
             (src1.value_type == 1 && !src2.value_type) ||
             (src1.value_type == 1 && src2.value_type == 1)) {
    result = 1;
  } else if ((!src1.value_type && src2.value_type == 2) ||
             (src1.value_type == 2 && !src2.value_type) ||
             (src1.value_type == 2 && src2.value_type == 2)) {
    result = 2;
  } else if ((!src1.value_type && src2.value_type == 3) ||
             (src1.value_type == 3 && !src2.value_type) ||
             (src1.value_type == 3 && src2.value_type == 3) ||
             (src1.value_type == 1 && src2.value_type == 2) ||
             (src1.value_type == 2 && src2.value_type == 1)) {
    result = 3;
  }
  return result;
}

int s21_get_type_for_sub(s21_decimal src1, s21_decimal src2) {
  int result;
  if (!src1.value_type && !src2.value_type) {
    result = 0;
  } else if ((!src1.value_type && src2.value_type == 2) ||
             (src1.value_type == 1 && !src2.value_type) ||
             (src1.value_type == 1 && src2.value_type == 2)) {
    result = 1;
  } else if ((!src1.value_type && src2.value_type == 1) ||
             (src1.value_type == 2 && !src2.value_type)) {
    result = 2;
  } else {
    result = 3;
  }
  return result;
}

int s21_get_type_for_less(s21_decimal src1, s21_decimal src2) {
  int result = 0;
  if (!src1.value_type && !src2.value_type) {
    result = -1;
  } else if ((!src1.value_type && src2.value_type == 1) ||
             (src1.value_type == 2 && !src2.value_type) ||
             (src1.value_type == 2 && src2.value_type == 1)) {
    result = 0;
  } else if ((!src1.value_type && src2.value_type == 2) ||
             (src1.value_type == 1 && !src2.value_type) ||
             (src1.value_type == 1 && src2.value_type == 2) ||
             (src1.value_type == 3 && src2.value_type == 3) ||
             (src1.value_type == 1 && src2.value_type == 3) ||
             (src1.value_type == 1 && src2.value_type == 1) ||
             (src1.value_type == 2 && src2.value_type == 2) ||
             (src1.value_type == 3 && src2.value_type == 2)) {
    result = 1;
  }
  return result;
}

int s21_get_type_for_great(s21_decimal src1, s21_decimal src2) {
  int result = 1;
  if (!src1.value_type && !src2.value_type) {
    result = -1;
  } else if ((!src1.value_type && src2.value_type == 2) ||
             (src1.value_type == 1 && !src2.value_type) ||
             (src1.value_type == 1 && src2.value_type == 2)) {
    result = 0;
  }
  return result;
}

int s21_get_type_for_eq(s21_decimal src1, s21_decimal src2) {
  int result;
  if (!src1.value_type && !src2.value_type) {
    result = 0;
  } else if ((src1.value_type == 1 && src2.value_type == 1) ||
             (src1.value_type == 2 && src2.value_type == 2)) {
    result = 0;
  } else {
    result = 1;
  }
  return result;
}

int s21_get_type_for_div(s21_decimal src1, s21_decimal src2, int sign_1,
                         int sign_2) {
  int result = 0;
  if (!src1.value_type && !s21_check_empty(src1) && !src2.value_type &&
      s21_check_empty(src2)) {
    result = 6;
  } else if (!src1.value_type && !src2.value_type && s21_check_empty(src2)) {
    result = 0;
  } else if (!src1.value_type && !src2.value_type) {
    if (sign_2)
      result = 2;
    else
      result = 1;
  } else if (src1.value_type == 1 && !src2.value_type) {
    if (sign_2)
      result = 2;
    else
      result = 1;
  } else if (src1.value_type == 2 && !src2.value_type) {
    if (sign_2)
      result = 1;
    else
      result = 2;
  } else if (!src1.value_type && src2.value_type == 1) {
    if (!sign_1)
      result = 4;
    else
      result = 5;
  } else if (!src1.value_type && src2.value_type == 2) {
    if (!sign_1)
      result = 5;
    else
      result = 4;
  } else {
    result = 3;
  }
  return result;
}

int s21_get_type_for_mul(s21_decimal src1, s21_decimal src2) {
  int result = 0;
  if (!src1.value_type && !src2.value_type) {
    result = 0;
  } else if ((!src1.value_type && src2.value_type == 1) ||
             (src1.value_type == 1 && !src2.value_type)) {
    result = 1;
  } else if ((!src1.value_type && src2.value_type == 2) ||
             (src1.value_type == 2 && !src2.value_type)) {
    result = 2;
  } else if ((src1.value_type == 1 && src2.value_type == 1) ||
             (src1.value_type == 2 && src2.value_type == 2)) {
    result = 1;
  } else if ((src1.value_type == 3 && !src2.value_type) ||
             (src1.value_type == 2 && src2.value_type == 1) ||
             (src1.value_type == 1 && src2.value_type == 2) ||
             (!src1.value_type && src2.value_type == 3)) {
    result = 3;
  }
  return result;
}

int s21_get_type_for_mod(s21_decimal src1, s21_decimal src2) {
  int result;
  if (!src1.value_type && !src2.value_type && s21_check_empty(src2)) {
    result = 0;
  } else if (!src1.value_type && !s21_is_one(src1) &&
             (src2.value_type == 1 || src2.value_type == 2)) {
    result = 4;
  } else if (!src1.value_type &&
             (src2.value_type == 1 || src2.value_type == 2)) {
    result = 2;
  } else {
    result = 3;
  }
  return result;
}

int s21_get_type_for_not_eq(s21_decimal src1, s21_decimal src2) {
  int result;
  if (!src1.value_type && !src2.value_type) {
    result = 2;
  } else if ((src1.value_type == 1 && src2.value_type == 1) ||
             (src1.value_type == 2 && src2.value_type == 2)) {
    result = 1;
  } else {
    result = 0;
  }
  return result;
}

int s21_is_one(s21_decimal src) {
  int flag = 0;
  if (src.bits[2]) flag = 1;
  if (src.bits[1]) flag = 1;
  if (src.bits[0] != 1) flag = 1;
  return flag;
}

int s21_check_bits_super(super_decimal *src1, super_decimal *src2) {
  int flag = -1;
  if (src1->bits[5] != src2->bits[5]) {
    flag = 5;
  } else if (src1->bits[4] != src2->bits[4]) {
    flag = 4;
  } else if (src1->bits[3] != src2->bits[3]) {
    flag = 3;
  } else if (src1->bits[2] != src2->bits[2]) {
    flag = 2;
  } else if (src1->bits[1] != src2->bits[1]) {
    flag = 1;
  } else if (src1->bits[0] != src2->bits[0]) {
    flag = 0;
  } else {
    flag = -1;
  }
  return flag;
}

int s21_check_empty_super(super_decimal src) {
  int flag = 0;
  if (src.bits[5]) flag = 1;
  if (src.bits[4]) flag = 1;
  if (src.bits[3]) flag = 1;
  if (src.bits[2]) flag = 1;
  if (src.bits[1]) flag = 1;
  if (src.bits[0]) flag = 1;
  return flag;
}

int s21_check_overflow(super_decimal src) {
  int flag = 0;
  if (src.bits[5]) flag = 1;
  if (src.bits[4]) flag = 1;
  if (src.bits[3]) flag = 1;
  return flag;
}

void s21_super_truncate(super_decimal *src, int scale) {
  super_decimal buf, divider = s21_one_div(*src);
  s21_init_sdecimal(&buf);
  buf = *src;
  scale = divider.bits[6] - scale;
  unsigned int digit = scale;

  while (scale > 0) {
    while (!s21_is_greater_or_equal_for_super(buf, divider)) {
      buf = s21_sub_for_super(buf, divider);
    }
    if (s21_check_empty_super(buf)) {
      while (!s21_is_less_for_super(buf, divider) && scale > 0) {
        s21_shift_mul_ten(&buf);
        scale--;
      }
    }
  }

  buf = s21_div_for_super(buf, divider);
  *src = s21_sub_for_super(*src, buf);
  divider = s21_one_div(*src);
  *src = s21_div_for_super(*src, divider);
  divider = s21_one_div(*src);
  while (divider.bits[6] < digit) {
    s21_shift_mul_ten(src);
    divider.bits[6]++;
  }
}

void s21_super_round_scale(super_decimal *src) {
  super_decimal one_plus, buf, divider = s21_one_div(*src);
  s21_init_sdecimal(&buf);
  s21_init_sdecimal(&one_plus);
  one_plus.bits[0] = 10;
  buf = *src;
  unsigned int score = divider.bits[6] - 1;
  for (unsigned int i = 0; i < score; i++) {
    while (!s21_is_greater_or_equal_for_super(buf, divider)) {
      buf = s21_sub_for_super(buf, divider);
    }
    s21_shift_mul_ten(&buf);
  }
  buf = s21_div_for_super(buf, divider);
  if (buf.bits[0] >= 5) *src = s21_add_for_super(*src, one_plus);
  *src = s21_sub_for_super(*src, buf);
  *src = s21_div_for_super(*src, divider);
  divider = s21_one_div(*src);
  while (divider.bits[6] != score) {
    s21_shift_mul_ten(src);
    divider.bits[6]++;
  }
}

void s21_shift_mul_ten(super_decimal *src) {
  super_decimal first = *src, second = *src;
  for (int i = 0; i < 3; i++) {
    s21_great_shift(&first);
  }
  s21_great_shift(&second);
  *src = s21_add_for_super(first, second);
}

void s21_great_shift(super_decimal *src) {
  int bts0 = 0, bts1 = 0, bts2 = 0, bts3 = 0, bts4 = 0;
  if (s21_get_bit(*src, 31)) bts0 = 1;
  if (s21_get_bit(*src, 63)) bts1 = 1;
  if (s21_get_bit(*src, 95)) bts2 = 1;
  if (s21_get_bit(*src, 127)) bts3 = 1;
  if (s21_get_bit(*src, 159)) bts4 = 1;
  for (int i = 0; i < 6; i++) {
    src->bits[i] <<= 1;
  }
  if (bts0) s21_set_bit(src, 32, 1);
  if (bts1) s21_set_bit(src, 64, 1);
  if (bts2) s21_set_bit(src, 96, 1);
  if (bts3) s21_set_bit(src, 128, 1);
  if (bts4) s21_set_bit(src, 160, 1);
}

void s21_normal_scale(super_decimal *src, int *scale) {
  super_decimal buf, divider = s21_one_div(*src);
  unsigned int digit = divider.bits[6];
  s21_init_sdecimal(&buf);
  buf = *src;
  buf = s21_div_for_super(buf, divider);

  if (s21_check_bits_super(&buf, src) != -1) {
    while (*scale) {
      divider = s21_one_div(*src);
      *src = s21_div_for_super(*src, divider);
      divider = s21_one_div(*src);
      digit--;

      if (divider.bits[6] == digit) {
        *scale -= 1;
        break;
      }
      while (divider.bits[6] < digit) {
        s21_shift_mul_ten(src);
        divider.bits[6]++;
      }
      *scale -= 1;
    }
  }
}

void s21_reduction_sdecimal(super_decimal *src, int *scale) {
  super_decimal one_plus, mod, divider = s21_one_div(*src);
  s21_init_sdecimal(&mod);
  s21_init_sdecimal(&one_plus);
  one_plus.bits[0] = 10;
  unsigned int digit, score = 30;
  digit = divider.bits[6] - score;
  *scale = *scale - digit;
  unsigned int flag = 0;
  while ((s21_check_overflow(*src) && *scale > 0) || flag) {
    if (flag) {
      flag = 0;
      *scale -= 1;
      score--;
    }
    mod = *src;
    digit = score;
    for (unsigned int i = 0; i < score; i++) {
      while (!s21_is_greater_or_equal_for_super(mod, divider)) {
        mod = s21_sub_for_super(mod, divider);
      }
      s21_shift_mul_ten(&mod);
    }
    unsigned int divider_digit = divider.bits[6] - digit;
    mod = s21_div_for_super(mod, divider);
    if (mod.bits[6] > 999) {
      *scale = -1;
      break;
    }
    divider = s21_one_div(mod);
    while (divider.bits[6] != divider_digit) {
      s21_shift_mul_ten(&mod);
      divider.bits[6]++;
    }
    if (mod.bits[0] >= 5 && mod.bits[0] < 10)
      *src = s21_add_for_super(*src, one_plus);
    *src = s21_sub_for_super(*src, mod);
    divider = s21_one_div(*src);
    *src = s21_div_for_super(*src, divider);
    divider = s21_one_div(*src);
    while (divider.bits[6] < digit) {
      s21_shift_mul_ten(src);
      divider.bits[6]++;
    }
    if (s21_check_overflow(*src) && *scale > 0) {
      score--;
      *scale -= 1;
    }
    if (!s21_check_overflow(*src) && *scale > 28) {
      flag = 1;
    }
  }
}

super_decimal s21_one_div(super_decimal src) {
  super_decimal divider;
  s21_init_sdecimal(&divider);
  divider.bits[0] = 1;
  int score = 0, save_score = 0;

  while (!s21_is_greater_or_equal_for_super(src, divider)) {
    s21_shift_mul_ten(&divider);
    score++;
  }
  save_score = score;
  if (divider.bits[0] != 1) {
    s21_init_sdecimal(&divider);
    divider.bits[0] = 1;
    score--;
    for (int i = 0; i != score; i++) {
      s21_shift_mul_ten(&divider);
    }
  }
  divider.bits[6] = save_score;
  return divider;
}

super_decimal s21_normal_div(super_decimal src1, super_decimal src2) {
  super_decimal buf = src2;
  int score = 0;
  while (!s21_is_greater_for_super(src1, buf)) {
    s21_shift_mul_ten(&buf);
    score++;
  }
  if (score) score--;
  for (int i = 0; i < score; i++) s21_shift_mul_ten(&src2);
  src2.bits[6] = score;
  return src2;
}
