#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <gmp.h>

mpz_t a;

void load_progress() {
    FILE *file = fopen("progress.txt", "rb");
    if (file == NULL) return;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *) malloc(file_size + 1);
    if (buffer == NULL) {
        fclose(file);
        return;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        fclose(file);
        free(buffer);
        return;
    }

    fclose(file);

    buffer[file_size] = '\0';
    mpz_set_str(a, buffer, 10);

    free(buffer);
}

void quit() {
    FILE *file = fopen("progress.txt", "w");

    if (file == NULL) return;

    char *str = mpz_get_str(NULL, 10, a);

    fprintf(file, "%s", str);
    fclose(file);
    free(str);
    exit(0);
}

int main() {
    mpz_t b;
    mpz_init_set_str(a, "300000000000000000000", 10);
    mpz_init(b);
    load_progress();
    signal(SIGINT, quit);

    main_loop:
    if (mpz_divisible_ui_p(a, 10000000)) {
        gmp_printf("period: %Zd\n", a);
    }
    mpz_set(b, a);

    try_again:
    if (mpz_even_p(b)) {
        mpz_div_ui(b, b, 2);
    } else {
        mpz_mul_ui(b, b, 3);
        mpz_add_ui(b, b, 1);
        mpz_div_ui(b, b, 2);
    }
    switch (mpz_cmp(a, b)) {
        case -1:
            goto try_again;
        case 0:
            gmp_printf("Found it: %Zd\n", a);
            exit(12);
    }

    mpz_add_ui(a, a, 1);
    goto main_loop;
}
