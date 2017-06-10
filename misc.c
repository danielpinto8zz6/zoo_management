/*
 * Copyright 2017 daniel.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   misc.c
 * Author: daniel
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 
 */

// Verificar se o ficheiro está vazio

bool check_empty(FILE *file) {
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0) {
        return true;
    }

    fseek(file, savedOffset, SEEK_SET);
    return false;
}

// Função para limpar o buffer

void clear_newlines(void) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

// Função para limpar ecrã, funciona de acordo com o SO

void clearScreen(void) {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

// Cabeçalho do programa

void header(void) {
    printf("\n\n");
    printf("\t############################################################\n");
    printf("\t###################                       ##################\n");
    printf("\t###################  Zoo Vale das Flores  ##################\n");
    printf("\t###################                       ##################\n");
    printf("\t############################################################\n");
}

// Pressionar enter para continuar, alternativa ao getch()

void PressEnterToContinue(void) {
    printf("\t# Pressione ENTER para continuar\n");
    clear_newlines();
    while (getchar() != '\n')
        ;
}

