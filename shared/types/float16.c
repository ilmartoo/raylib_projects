#include <stdint.h>

#include "types/float16.h"

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

half f16_add(half h1, half h2) { return ftof16(f16tof(h1) + f16tof(h2)); }

half f16_sub(half h1, half h2) { return ftof16(f16tof(h1) - f16tof(h2)); }

half f16_mul(half h1, half h2) { return ftof16(f16tof(h1) * f16tof(h2)); }

half f16_div(half h1, half h2) { return ftof16(f16tof(h1) / f16tof(h2)); }

half f16_neg(half h) { return F16_SIGNUM_MASK ^ h; }

// Integer transformations //

half itof16(int32_t i) {
    int32_t v;
    int sig = 0;
    if (i < 0) {
        v = -i;
        sig = 1;
    } else {
        v = i;
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

int32_t f16toi(half h) {
    uint16_t value = F16_MANTISSA(h);
    int16_t shift = F16_EXPONENT(h) - 25;
    if (shift > 0) {
        value <<= shift;
    } else if (shift < 0) {
        value >>= -shift;
    }
    if (F16_SIGNUM(h)) { return -(int32_t)(value); }
    return value;
}

// Single-precision floating point transformations //

half ftof16(float f) {
    void* bits_ptr = &f;
    uint32_t bits = *(uint32_t*)bits_ptr;
    uint16_t sign = (bits >> 16) & 0x8000;
    uint32_t exp = (bits >> 23) & 0xFF;
    uint32_t mant = bits & 0x7FFFFF;

    if (exp == 255) {  // NaN or Inf
        if (mant != 0) {
            uint16_t nan_mant = mant >> 13;
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
        uint32_t shift = 113 - exp;
        mant |= 0x800000;  // Add implicit 1
        uint16_t submant = mant >> (shift + 13);
        // Rounding
        if (((mant >> (shift + 12)) & 1) && ((mant >> (shift + 13)) & 1 || (mant & ((1 << (shift + 12)) - 1)))) { submant += 1; }
        return sign | submant;
    }

    // Normal case
    uint16_t hexp = exp - 112;
    uint32_t mant_rounded = mant + 0x1000;
    if (mant_rounded & 0x800000) {
        mant_rounded = 0;
        hexp += 1;
    }

    if (hexp >= 0x1F) {
        return sign | 0x7C00;  // Overflow to infinity
    }

    return sign | (hexp << 10) | (mant_rounded >> 13);
}

float f16tof(half h) {
    uint32_t sign = (h & 0x8000) << 16;
    uint32_t exp = (h & 0x7C00) >> 10;
    uint32_t mant = (h & 0x03FF);

    if (exp == 0) {
        if (mant == 0) {
            // Zero
            void* sign_ptr = &sign;
            return *(float*)sign_ptr;
        } else {
            // Subnormal number
            uint32_t shift = 0;
            while ((mant & 0x200) == 0) {
                mant <<= 1;
                shift++;
            }
            mant &= 0x3FF;  // Remove leading 1
            exp = 1;
            exp = 127 - 15 - shift;  // Adjust exponent for float32
            uint32_t fbits = sign | (exp << 23) | (mant << 13);
            void* fbits_ptr = &fbits;
            return *(float*)fbits_ptr;
        }
    } else if (exp == 0x1F) {
        // Inf or NaN
        if (mant == 0) return *(float*)&(uint32_t){sign | 0x7F800000};                  // Inf
        else return *(float*)&(uint32_t){sign | 0x7F800000 | (mant << 13) | 0x400000};  // NaN (qNaN)
    }

    // Normalized float16 to float32
    uint32_t fexp = exp + (127 - 15);
    uint32_t fbits = sign | (fexp << 23) | (mant << 13);
    void* fbits_ptr = &fbits;
    return *(float*)fbits_ptr;
}

// Boolean operations //

int f16_gte(half h1, half h2) {
    if (F16_IS_ZERO(h1) && F16_IS_ZERO(h2)) { return 1; }
    if (F16_IS_NAN(h1) || F16_IS_NAN(h2)) { return 0; }
    if (!F16_SIGNUM(h1)) {
        if (F16_SIGNUM(h2)) { return 1; }
        return h1 >= h2;
    } else {
        if (!F16_SIGNUM(h2)) { return 0; }
        return F16_UNSIGNED_VALUE(h1) <= F16_UNSIGNED_VALUE(h2);
    }
}

int f16_gt(half h1, half h2) {
    if (F16_IS_NAN(h1) || F16_IS_NAN(h2)) { return 0; }
    if (F16_IS_ZERO(h1) && F16_IS_ZERO(h2)) { return 0; }
    if (!F16_SIGNUM(h1)) {
        if (F16_SIGNUM(h2)) { return 1; }
        return h1 > h2;
    } else {
        if (!F16_SIGNUM(h2)) { return 0; }
        return F16_UNSIGNED_VALUE(h1) < F16_UNSIGNED_VALUE(h2);
    }
}

int f16_eq(half h1, half h2) {
    if (F16_IS_NAN(h1) || F16_IS_NAN(h2)) { return 0; }
    if (F16_IS_ZERO(h1) && F16_IS_ZERO(h2)) { return 1; }
    return h1 == h2;
}

int f16_lte(half h1, half h2) {
    if (F16_IS_NAN(h1) || F16_IS_NAN(h2)) { return 0; }
    return f16_gte(h2, h1);
}

int f16_lt(half h1, half h2) {
    if (F16_IS_NAN(h1) || F16_IS_NAN(h2)) { return 0; }
    return f16_gt(h2, h1);
}

int f16_neq(half h1, half h2) { return !f16_eq(h1, h2); }
