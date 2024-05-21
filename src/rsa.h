#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef RSA_H
#define RSA_H

unsigned long long int modpow(int base, int power, int mod) {
    int i;
    unsigned long long int result = 1;
    for (i = 0; i < power; i++) {
        result = (result * base) % mod;
    }
    return result;
}

int inverse(int a, int mod) {
    int aprev, iprev, i = 1, atemp, itemp;

    aprev = mod, iprev = mod;
    while (a != 1) {
        atemp = a;
        itemp = i;
        a = aprev - aprev / atemp * a;
        i = iprev - aprev / atemp * i;
        aprev = atemp;
        iprev = itemp;
        while (i < 0)
            i += mod;
    }

    return i;
}

char* encrypt(char* input_str) {
    int m, n, e;
    unsigned long long int c;

    FILE* inp = fopen("public.txt", "r");
    fscanf(inp, "%d %d", &n, &e);
    fclose(inp);

    int len = strlen(input_str);
    char* ciphertext = (char*)malloc((len * 20 + 1) * sizeof(char));
    int ciphertext_index = 0;

    for (int i = 0; input_str[i] != '\0'; i++) {
        c = modpow(input_str[i], e, n);
        ciphertext_index += sprintf(ciphertext + ciphertext_index, "%llu ", c);
    }
    ciphertext[ciphertext_index - 1] = '\0';

    return ciphertext;
}

char* decrypt(char* ciphertext) {
    int d, n, p, q, h, m, qInv, m1m2;
    unsigned long long int c, dP, dQ, m1, m2;
    FILE* inp;

    inp = fopen("private.txt", "r");
    fscanf(inp, "%d %d", &n, &d);
    fclose(inp);

    inp = fopen("pq.txt", "r");
    fscanf(inp, "%d %d", &p, &q);
    fclose(inp);

    char* message = (char*)malloc(strlen(ciphertext) + 1);
    int message_index = 0;

    char* token = strtok(ciphertext, " ");
    while (token != NULL) {
        c = strtoull(token, NULL, 10);
        dP = d % (p - 1);
        dQ = d % (q - 1);
        qInv = inverse(q, p);
        m1 = modpow(c, dP, p);
        m2 = modpow(c, dQ, q);
        m1m2 = m1 - m2;
        if (m1m2 < 0)
            m1m2 += p;
        h = (qInv * m1m2) % p;
        m = m2 + h * q;
        message[message_index++] = (char)m;
        token = strtok(NULL, " ");
    }
    message[message_index] = '\0';

    return message;
}

#endif