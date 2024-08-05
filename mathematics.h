#pragma once

#include <vector>
#include <cmath>

template<class T>
inline T circle_idx(T i, T n) {
    if (i < n)
        return i;

    return i - n;
}

vector<int> get_comb_coeffs(int n) {
    if (n == 1)
        return {1};
    if (n == 4)
        return {1, 3, 3, 1}; // самый частый случай при вызове, небольшая оптимизация

    vector<int> coeffs(n);
    coeffs[0] = 1;
    coeffs.back() = 1;
    for (int i = 1; i < n / 2; i++) {
        coeffs[i] = coeffs[i - 1] * (n - i) / i;
        coeffs[n - i - 1] = coeffs[i];
    }
    if (n % 2 != 0) {
        coeffs[n / 2] = coeffs[n / 2 - 1] * (n - n / 2) / (n / 2);
    }

    return coeffs;
}

template<class T>
int sign(T n) {
    if (n > 0)
        return 1;
    else if (n < 0) {
        return -1;
    }

    return 0;
}
