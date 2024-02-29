#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdbool.h>

typedef struct {
        unsigned size;
        mpz_t* elements;
} GmpSet;

GmpSet set_create() {
        GmpSet set;
        set.size = 0;
        return set;
}

bool set_includes(GmpSet set, mpz_t element) {
        for(unsigned i = 0; i < set.size; i++) {
                if(set.elements[i] == element) return true;
        }
        return false;
}

void set_push(GmpSet set, mpz_t element) {
        //if(!set_includes(set, element)) {
                set.size++;
                mpz_t* old = set.elements;
                mpz_t* new = (mpz_t*)malloc(set.size * sizeof(mpz_t));
                for(unsigned i = 0; i < set.size - 1; i++) {
                        mpz_init_set(new[i], old[i]);
                }
                set.elements = new;
                mpz_init_set(new[set.size - 1], element);
                free(old);
        //}
}

void collatz(mpz_t n) {
        if(mpz_even_p(n)) {
                mpz_div_ui(n, n, 2);
                return;
        }
        mpz_mul_ui(n, n, 3);
        mpz_add_ui(n, n, 1);
}

int main() {
        //GmpSet set = set_create();

        mpz_t a, b;
        mpz_init(a);
        mpz_init(b);
        mpz_set_str(a, "300000000000000000000", 10);

        while(true) {
                if(mpz_divisible_ui_p(a, 1000000)) {
                        gmp_printf("period: %Zd\n", a);
                }
            mpz_set(b, a);
            while(true) {
                collatz(b);
                int cmp = mpz_cmp(a, b);
                if(cmp > 0) break;
                if(cmp == 0) {
                        gmp_printf("Cracked. %Zd\n", a);
                        return 12;
                }
                //if(set_includes(set, b)) break;
                //set_push(set, b);
            }
            mpz_add_ui(a, a, 1);
        }

        mpz_clear(a);
        mpz_clear(b);
        return 0;
}
