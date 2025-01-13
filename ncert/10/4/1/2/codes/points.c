#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <openssl/rand.h>

typedef double complex compl;

double a = 1;
double b = -4;
double c = 6;

double f(double x) {
    return a * x * x + b * x + c;
}

double f_derivative(double x) {
    return 2 * a * x + b;
}

compl compl_f_der(compl x) {
    return 2 * a * x + b;
}

compl compl_f(compl x) {
    return a * x * x + b * x + c;
}

int run_complex_newton_iterations(int num_iter, double upper_alarm, double lower_alarm, compl init_point) {
    for (int i = 0; i < num_iter; i++) {
        if (cabs(compl_f_der(init_point)) < lower_alarm) {
            printf("Reached extrema\n");
            return -1;
        } else {
            compl save = init_point;
            init_point -= compl_f(init_point) / compl_f_der(init_point);
            if (cabs((init_point - save) / init_point) < lower_alarm) {
                printf("Solution = %lf + %lf i\n", creal(init_point), cimag(init_point));
                return 0;
            }
            if (cabs(init_point) > upper_alarm) {
                printf("x got too big\n");
                return -1;
            }
        }
    }
    return -1;
}

int run_newton_iterations(int num_iter, double upper_alarm, double lower_alarm, double init_point) {
    while(1) {
        if (fabs(f_derivative(init_point)) < lower_alarm) {
            printf("Failure\n");
            return -1;
        } else {
            double save = init_point;
            init_point -= f(init_point) / f_derivative(init_point);
            if (fabs((init_point - save) / init_point) < lower_alarm) {
                printf("Solution = %lf\n", init_point);
                return 0;
            }
            if (fabs(init_point) > upper_alarm) {
                printf("x got too big\n");
                return -1;
            }
        }
    }
    return -1;
}

double g(double x) {
    return (-a / b) * x * x - (c / b);
}

compl compl_g(compl x) {
    return (-a / b) * x * x - (c / b);
}

int run_complex_fixed_point_iterations(compl init_guess, int num_iter, double upper_alarm) {
    for (int i = 0; i < num_iter; i++) {
        if (cabs(init_guess) > upper_alarm) {
            printf("x got too big\n");
            return -1;
        }
        init_guess = compl_g(init_guess);
    }
    printf("Solution: %lf + %lf i\n", creal(init_guess), cimag(init_guess));
    return 0;
}

int run_fixed_point_iterations(double init_guess, int num_iter, double upper_alarm) {
    for (int i = 0; i < num_iter; i++) {
        if (fabs(init_guess) > upper_alarm) {
            printf("x got too big\n");
            return -1;
        }
        init_guess = g(init_guess);
    }    
    printf("Solution: %lf\n", init_guess);
    return 0;
}

double random_double_in_range(double min, double max) {
    if (min > max) {
        fprintf(stderr, "Invalid range: min should be less than or equal to max.\n");
        return -1;
    }
    unsigned char random_bytes[8];
    uint64_t random_value;
    if (RAND_bytes(random_bytes, sizeof(random_bytes)) != 1) {
        fprintf(stderr, "Error generating random bytes\n");
        return -1;
    }
    random_value = *((uint64_t*)random_bytes);
    double normalized = (random_value / (double)(UINT64_MAX));
    double scaled_value = normalized * (max - min) + min;
    return scaled_value;
}

int main() {
    printf("Running Newton iterations:\n");
    int ret_val = run_newton_iterations(1000, 1e5, 1e-5, random_double_in_range(-10, 10));

    if (ret_val == -1) {
        printf("Trying fixed point iterations:\n");
        ret_val = run_fixed_point_iterations(random_double_in_range(-10, 10), 1000, 1e10);

        if (ret_val == -1) {
            printf("Trying complex Newton's iterations:\n");
            compl init_guess = random_double_in_range(-10, 10) + random_double_in_range(-10, 10) * I;
            ret_val = run_complex_newton_iterations(1000, 1e10, 1e-5, init_guess);

            if (ret_val == -1) {
                printf("Trying complex fixed point iterations:\n");
                init_guess = random_double_in_range(-10, 10) + random_double_in_range(-10, 10) * I;
                ret_val = run_complex_fixed_point_iterations(init_guess, 1000, 1e10);

                if (ret_val == -1) {
                    printf("Complete failure, no roots were found.\n");
                    return 0;
                }
            }
        }
    }

    return 0;
}

