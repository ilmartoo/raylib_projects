#include "float16.h"
#include "types.h"

#define F16_SIGNUM_MASK    0x8000
#define F16_EXP_MASK       0x7C00
#define F16_NO_SIGNUM_MASK 0x7FFF
#define F16_NAN_VALUE      F16_NO_SIGNUM_MASK

#define F16_MANTISSA(x)         (((x) & 1023) | (((x) & F16_EXP_MASK) == 0 ? 0 : 1024))
#define F16_EXPONENT(x)         (((x) & F16_EXP_MASK) >> 10)
#define F16_SIGNUM(x)           ((x) & F16_SIGNUM_MASK)
#define F16_UNSIGNED_VALUE(x)   ((x) & F16_NO_SIGNUM_MASK)
#define F16_SIGNED_INF_VALUE(x) (((x) & F16_SIGNUM_MASK) | F16_EXP_MASK)

#define F16_IS_ZERO(x)    (!F16_UNSIGNED_VALUE(x))
#define F16_IS_INVALID(x) (((x) & F16_EXP_MASK) == F16_EXP_MASK)
#define F16_IS_NAN(x)     (F16_UNSIGNED_VALUE(x) > F16_EXP_MASK)
#define F16_IS_INF(x)     (F16_UNSIGNED_VALUE(x) == F16_EXP_MASK)

// Arithmetic operations //

f16 f16_sub(f16 ain, f16 bin) {
    u16 a = ain;
    u16 b = bin;
    if (F16_SIGNUM(a ^ b)) { return f16_add(a, b ^ F16_SIGNUM_MASK); }
    u16 sign = F16_SIGNUM(a);
    a = a << 1;
    b = b << 1;
    if (a < b) {
        u16 x = a;
        a = b;
        b = x;
        sign ^= F16_SIGNUM_MASK;
    }
    u16 ax = a & 0xF800;
    u16 bx = b & 0xF800;
    if (a >= 0xf800 || b >= 0xf800) {
        if (a > 0xF800 || b > 0xF800 || a == b) { return F16_NO_SIGNUM_MASK; }
        u16 res = sign | F16_EXP_MASK;
        if (a == 0xf800) {
            return res;
        } else {
            return res ^ F16_SIGNUM_MASK;
        }
    }
    int exp_diff = ax - bx;
    u16 exp_part = ax;
    if (exp_diff != 0) {
        int shift = exp_diff >> 11;
        if (bx != 0) {
            b = ((b & 2047) | 2048) >> shift;
        } else {
            b >>= (shift - 1);
        }
    } else {
        if (bx == 0) {
            u16 res = (a - b) >> 1;
            if (res == 0) { return res; }
            return res | sign;
        } else {
            b = (b & 2047) | 2048;
        }
    }
    u16 r = a - b;
    if ((r & 0xF800) == exp_part) { return (r >> 1) | sign; }
    u16 am = (a & 2047) | 2048;
    u16 new_m = am - b;
    if (new_m == 0) { return 0; }
    while (exp_part != 0 && !(new_m & (2048))) {
        exp_part -= 0x800;
        if (exp_part != 0) { new_m <<= 1; }
    }
    return (((new_m & 2047) | exp_part) >> 1) | sign;
}

f16 f16_add(f16 a, f16 b) {
    if (F16_SIGNUM(a ^ b)) { return f16_sub(a, b ^ F16_SIGNUM_MASK); }
    i16 sign = F16_SIGNUM(a);
    a &= F16_NO_SIGNUM_MASK;
    b &= F16_NO_SIGNUM_MASK;
    if (a < b) {
        i16 x = a;
        a = b;
        b = x;
    }
    if (a >= F16_EXP_MASK || b >= F16_EXP_MASK) {
        if (a > F16_EXP_MASK || b > F16_EXP_MASK) { return F16_NAN_VALUE; }
        return F16_EXP_MASK | sign;
    }
    i16 ax = F16_EXPONENT(a);
    i16 bx = F16_EXPONENT(b);
    i16 exp_diff = ax - bx;
    i16 exp_part = ax;
    if (exp_diff != 0) {
        int shift = exp_diff >> 10;
        if (bx != 0) {
            b = ((b & 1023) | 1024) >> shift;
        } else {
            b >>= (shift - 1);
        }
    } else {
        if (bx == 0) {
            return (a + b) | sign;
        } else {
            b = (b & 1023) | 1024;
        }
    }
    i16 r = a + b;
    if (F16_EXPONENT(r) != exp_part) {
        u16 am = (a & 1023) | 1024;
        u16 new_m = (am + b) >> 1;
        r = (exp_part + 0x400) | (1023 & new_m);
    }
    if ((u16)r >= 0x7C00u) { return sign | F16_EXP_MASK; }
    return r | sign;
}

f16 f16_mul(f16 a, f16 b) {
    int sign = F16_SIGNUM(a ^ b);

    if (F16_IS_INVALID(a) || F16_IS_INVALID(b)) {
        if (F16_IS_NAN(a) || F16_IS_NAN(b) || F16_IS_ZERO(a) || F16_IS_ZERO(b)) { return F16_NAN_VALUE; }
        return sign | F16_EXP_MASK;
    }

    if (F16_IS_ZERO(a) || F16_IS_ZERO(b)) { return 0; }
    u16 m1 = F16_MANTISSA(a);
    u16 m2 = F16_MANTISSA(b);

    i32 v = m1;
    v *= m2;
    int ax = F16_EXPONENT(a);
    int bx = F16_EXPONENT(b);
    ax += (ax == 0);
    bx += (bx == 0);
    int new_exp = ax + bx - 15;

    if (v & ((i32)1 << 21)) {
        v >>= 11;
        new_exp++;
    } else if (v & ((i32)1 << 20)) {
        v >>= 10;
    } else {  // denormal
        new_exp -= 10;
        while (v >= 2048) {
            v >>= 1;
            new_exp++;
        }
    }
    if (new_exp <= 0) {
        v >>= (-new_exp + 1);
        new_exp = 0;
    } else if (new_exp >= 31) {
        return F16_SIGNED_INF_VALUE(sign);
    }
    return sign | (new_exp << 10) | (v & 1023);
}

f16 f16_div(f16 a, f16 b) {
    i16 sign = F16_SIGNUM(a ^ b);
    if (F16_IS_NAN(a) || F16_IS_NAN(b) || (F16_IS_INVALID(a) && F16_IS_INVALID(b)) || (F16_IS_ZERO(a) && F16_IS_ZERO(b))) {
        return F16_NAN_VALUE;
    }
    if (F16_IS_INVALID(a) || F16_IS_ZERO(b)) { return sign | F16_EXP_MASK; }
    if (F16_IS_INVALID(b) || F16_IS_ZERO(a)) { return 0; }

    u16 m1 = F16_MANTISSA(a);
    u16 m2 = F16_MANTISSA(b);
    i32 m1_shifted = m1;
    m1_shifted <<= 10;
    i32 v = m1_shifted / m2;
    u16 rem = m1_shifted % m2;

    int ax = F16_EXPONENT(a);
    int bx = F16_EXPONENT(b);
    ax += (ax == 0);
    bx += (bx == 0);
    int new_exp = ax - bx + 15;

    if (v == 0 && rem == 0) { return 0; }

    while (v < 1024 && new_exp > 0) {
        v <<= 1;
        rem <<= 1;
        if (rem >= m2) {
            v++;
            rem -= m2;
        }
        new_exp--;
    }
    while (v >= 2048) {
        v >>= 1;
        new_exp++;
    }

    if (new_exp <= 0) {
        v >>= (-new_exp + 1);
        new_exp = 0;
    } else if (new_exp >= 31) {
        return F16_SIGNED_INF_VALUE(sign);
    }
    return sign | (v & 1023) | (new_exp << 10);
}

f16 f16_neg(f16 v) { return F16_SIGNUM_MASK ^ v; }

// Integer transformations //

f16 itof16(i32 sv) {
    i32 v;
    int sig = 0;
    if (sv < 0) {
        v = -sv;
        sig = 1;
    } else {
        v = sv;
    }
    if (v == 0) { return 0; }
    int e = 25;
    while (v >= 2048) {
        v >>= 1;
        e++;
    }
    while (v < 1024) {
        v <<= 1;
        e--;
    }
    if (e >= 31) { return F16_SIGNED_INF_VALUE(sig << 15); }
    return (sig << 15) | (e << 10) | (v & 1023);
}

i32 f16toi(f16 a) {
    u16 value = F16_MANTISSA(a);
    i16 shift = F16_EXPONENT(a) - 25;
    if (shift > 0) {
        value <<= shift;
    } else if (shift < 0) {
        value >>= -shift;
    }
    if (F16_SIGNUM(a)) { return -(i32)(value); }
    return value;
}

// Single-precision floating point transformations //

f16 ftof16(f32 f) {
    u32 bits = *(u32 *)&f;
    u16 sign = (bits >> 16) & 0x8000;
    u32 exp = (bits >> 23) & 0xFF;
    u32 mant = bits & 0x7FFFFF;

    if (exp == 255) {  // NaN or Inf
        if (mant != 0) {
            u16 nan_mant = mant >> 13;
            if (nan_mant == 0) nan_mant = 1;
            return sign | 0x7C00 | nan_mant;  // NaN
        } else {
            return sign | 0x7C00;  // Infinity
        }
    }

    if (exp < 113) {  // Too small for normalized half
        if (exp < 103) {
            return sign;  // Underflow to zero
        }
        // Subnormal number
        u32 shift = 113 - exp;
        mant |= 0x800000;  // Add implicit 1
        u16 submant = mant >> (shift + 13);
        // Rounding
        if (((mant >> (shift + 12)) & 1) && ((mant >> (shift + 13)) & 1 || (mant & ((1 << (shift + 12)) - 1)))) { submant += 1; }
        return sign | submant;
    }

    // Normal case
    u16 hexp = exp - 112;
    u32 mant_rounded = mant + 0x1000;
    if (mant_rounded & 0x800000) {
        mant_rounded = 0;
        hexp += 1;
    }

    if (hexp >= 0x1F) {
        return sign | 0x7C00;  // Overflow to infinity
    }

    return sign | (hexp << 10) | (mant_rounded >> 13);
}

f32 f16tof(f16 h) {
    u32 sign = (h & 0x8000) << 16;
    u32 exp = (h & 0x7C00) >> 10;
    u32 mant = (h & 0x03FF);

    if (exp == 0) {
        if (mant == 0) {
            // Zero
            return *(f32 *)&sign;
        } else {
            // Subnormal number
            u32 shift = 0;
            while ((mant & 0x200) == 0) {
                mant <<= 1;
                shift++;
            }
            mant &= 0x3FF;  // Remove leading 1
            exp = 1;
            exp = 127 - 15 - shift;  // Adjust exponent for float32
            u32 fbits = sign | (exp << 23) | (mant << 13);
            return *(f32 *)&fbits;
        }
    } else if (exp == 0x1F) {
        // Inf or NaN
        if (mant == 0) return *(f32 *)&(u32){sign | 0x7F800000};                  // Inf
        else return *(f32 *)&(u32){sign | 0x7F800000 | (mant << 13) | 0x400000};  // NaN (qNaN)
    }

    // Normalized float16 to float32
    u32 fexp = exp + (127 - 15);
    u32 fbits = sign | (fexp << 23) | (mant << 13);
    return *(f32 *)&fbits;
}

// Boolean operations //

bool f16_gte(f16 a, f16 b) {
    if (F16_IS_ZERO(a) && F16_IS_ZERO(b)) { return true; }
    if (F16_IS_NAN(a) || F16_IS_NAN(b)) { return false; }
    if (!F16_SIGNUM(a)) {
        if (F16_SIGNUM(b)) { return true; }
        return a >= b;
    } else {
        if (!F16_SIGNUM(b)) { return false; }
        return F16_UNSIGNED_VALUE(a) <= F16_UNSIGNED_VALUE(b);
    }
}

bool f16_gt(f16 a, f16 b) {
    if (F16_IS_NAN(a) || F16_IS_NAN(b)) { return false; }
    if (F16_IS_ZERO(a) && F16_IS_ZERO(b)) { return false; }
    if (!F16_SIGNUM(a)) {
        if (F16_SIGNUM(b)) { return true; }
        return a > b;
    } else {
        if (!F16_SIGNUM(b)) { return false; }
        return F16_UNSIGNED_VALUE(a) < F16_UNSIGNED_VALUE(b);
    }
}

bool f16_eq(f16 a, f16 b) {
    if (F16_IS_NAN(a) || F16_IS_NAN(b)) { return false; }
    if (F16_IS_ZERO(a) && F16_IS_ZERO(b)) { return true; }
    return a == b;
}

bool f16_lte(f16 a, f16 b) {
    if (F16_IS_NAN(a) || F16_IS_NAN(b)) { return false; }
    return f16_gte(b, a);
}

bool f16_lt(f16 a, f16 b) {
    if (F16_IS_NAN(a) || F16_IS_NAN(b)) { return false; }
    return f16_gt(b, a);
}

bool f16_neq(f16 a, f16 b) { return !f16_eq(a, b); }
