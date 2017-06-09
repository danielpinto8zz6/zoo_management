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
 * File:   areas.c
 * Author: daniel
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50

#include "structs.h"
#include "misc.h"
#include "animals.h"
#include "areas.h"
#include "main.h"

/*
 * 
 */

// Função que apaga os dados de determinada area

d_areas *delete_area_data(d_areas *data, char *key) {

    for (int i = 0; i < data->size; i++) {
        if (strcmp(key, data->area[i].identifier) == 0) {
            for (int j = i; j < data->size - 1; j++) {
                data->area[j] = data->area[j + 1];
            }
            data->size--;
            data->area = realloc(data->area, sizeof (t_areas) * data->size);
            printf("\n\tEliminado!\n\n");
            PressEnterToContinue();
            return data;
        }
    }

    printf("\n\tNenhum resultado encontrado!\n\n");
    PressEnterToContinue();
    return data;
}

// Função que insere os dados recebidos na estrutura de dados

d_areas *insert_area_data(d_areas *data, char identifier[SIZE], float capacity,
        int nr_adjacent_areas, char adjacent_areas[][SIZE]) {
    int x;
    if (data == NULL) {
        // allocar memoria para o array
        data = (d_areas *) malloc(sizeof (d_areas));
        data->area = malloc(sizeof (t_areas));
        data->size = 0;
    }

    data->size++;
    // Alocar memória para a posição atual
    if (data->size > 1) {
        data->area = realloc(data->area, sizeof (t_areas) * data->size);
    }
    int index = data->size - 1;

    // start_areas = (t_areas *)malloc(sizeof(t_areas));
    // Copiar os dados recebidos para a estrutura
    strncpy(data->area[index].identifier, identifier, strlen(identifier) + 1);
    data->area[index].capacity = capacity;
    data->area[index].nr_adjacent_areas = nr_adjacent_areas;
    for (x = 0; x < nr_adjacent_areas; x++) {
        strncpy(data->area[index].adjacent_areas[x], adjacent_areas[x],
                strlen(adjacent_areas[x]) + 1);
    }

    return data;
}

// Verificar se a capacidade da área é excedida

int check_capacity(d_areas *areas_data, list_animals *animals_data,
        char *identifier, float weight) {
    float area_capacity, total_animals_weight;
    for (int i = 0; i < areas_data->size; i++) {
        if (strcmp(identifier, areas_data->area[i].identifier) == 0) {
            area_capacity = areas_data->area[i].capacity;
        }
    }
    for (; animals_data != NULL; animals_data = animals_data->prox) {
        if (strcmp(identifier, animals_data->location) == 0) {
            total_animals_weight = total_animals_weight + animals_data->weight;
        }
    }
    if ((total_animals_weight + weight) <= area_capacity) {
        // A área suporta o animal
        return 1;
    } else {
        return 0;
    }
}

// Verificar se a área existe

int check_if_area_exists(d_areas *data, char *location) {
    for (int i = 0; i < data->size; i++) {
        if (strcmp(location, data->area[i].identifier) == 0) {
            return 1;
        }
    }
    return 0;
}

// Função para criar uma nova área

void create_area(void) {
    int x;

    t_areas load;

    clearScreen();
    header();

    // Evitar o registo de uma area ja existente
    do {
        printf("\n\tDigite o identificador da area : ");
        scanf(" %49[^\n]", load.identifier);
    } while (check_if_area_exists(start_areas, load.identifier) == 1 &&
            printf("\n\tJa existe uma area com o identificador %s, use outro "
            "identificador\n",
            load.identifier));

    printf("\n\tDigite a capacidade da area : ");
    scanf("%f", &load.capacity);
    do {
        printf("\n\tDigite o numero de areas adjacentes [max : 3] : ");
        scanf("%d", &load.nr_adjacent_areas);
    } while (load.nr_adjacent_areas < 0 || load.nr_adjacent_areas > 3);
    for (x = 0; x < load.nr_adjacent_areas; x++) {
        do {
            printf("\n\tDigite a area adjacente %d : ", x + 1);
            scanf(" %49[^\n]", load.adjacent_areas[x]);
        } while (
                check_if_area_exists(start_areas, load.adjacent_areas[x]) == 0 &&
                printf(
                "\n\t%s nao e uma area existente! Insira uma area existente...\n",
                load.adjacent_areas[x]));
    }

    // Enviar dados recebidos para a função que copia os dados para a Estrutura
    start_areas = insert_area_data(start_areas, load.identifier, load.capacity,
            load.nr_adjacent_areas, load.adjacent_areas);

    printf("\n\tArea creada com sucesso!\n\n");
    PressEnterToContinue();
}

// Funcao que define a lista como vazia.

void create_areas_list(void) {
    start_areas = NULL;
}

// Função para definir a area a apagar

void delete_area(list_animals *animals_data) {
    char key[SIZE];
    int found = 0;
    clearScreen();
    header();

    printf("\n\tDigite o nome da area a remover : ");
    scanf(" %49[^\n]", key);
    for (; animals_data != NULL; animals_data = animals_data->prox) {
        if (strcmp(key, animals_data->location) == 0) {
            printf("\n\tNome = %s\n", animals_data->name);
            found++;
        }
    }
    // Verificar se nenhum animal se encontra na area antes de a remover
    if (found == 0) {
        start_areas = delete_area_data(start_areas, key);
    } else {
        printf("\n\tNao e possivel remover areas que contenham animais!\n\n");
        PressEnterToContinue();
    }
}

// Função que carrega os dados relativos às áreas no inicio do programa

void load_areas_data(void) {
    t_areas load;

    FILE *file;
    file = fopen("areas.txt", "r");

    // Verificar se é possivel abrir o ficheiro
    if (file == NULL) {
        clearScreen();
        header();
        printf("\n\tNao foi possivel carregar dados das areas do zoo! \n\tA fechar programa...\n\n");
        PressEnterToContinue();
        exit(0);
    } else {
        free(start_animals);
        while (fscanf(file, "%s %f %d", load.identifier, &load.capacity,
                &load.nr_adjacent_areas) == 3) {
            for (int i = 0; i < load.nr_adjacent_areas; i++) {
                fscanf(file, "%s", load.adjacent_areas[i]);
            }
            start_areas =
                    insert_area_data(start_areas, load.identifier, load.capacity,
                    load.nr_adjacent_areas, load.adjacent_areas);
        }
    }
    fclose(file);
}

// Menu areas

void menu_areas(void) {
    int op;
menu:
    clearScreen();
    header();

    printf("\n\t# 1 - Criar uma nova area");
    printf("\n\t# 2 - Eliminar uma area existente");
    printf("\n\t# 3 - Ver areas do zoo");
    printf("\n\t# 0 - Voltar ao menu principal\n\n");

    printf("\t### : ");
    scanf("%d", &op);

    switch (op) {
        case 1:
            clearScreen();
            create_area();
            break;
        case 2:
            clearScreen();
            delete_area(start_animals);
            break;
        case 3:
            clearScreen();
            show_areas(start_areas);
            break;
        case 0:
            clearScreen();
            main();
            break;
        default:
            printf("\n\t# Opcao invalida\n");
            PressEnterToContinue();
            clearScreen();
            goto menu;
    }
    goto menu;
}

// Função que guarda os dados relativos às áreas ao terminar o programa

void save_areas_data(d_areas *data) {

    FILE *file;
    file = fopen("areas.txt", "w");
    // Verificar se é possivel abrir o ficheiro
    if (file == NULL) {
        clearScreen();
        header();
        printf("\n\tNao foi possivel guardar dados de areas de zoo!\n\n");
        PressEnterToContinue();
        exit(0);
    } else {
        // Antes de guardar os dados vamos limpar o ficheiro para os mesmo dados nao
        // serem inseridos duas vezes
        for (int i = 0; i < data->size; i++) {
            fprintf(file, "%s %.2f %d ", data->area[i].identifier,
                    data->area[i].capacity, data->area[i].nr_adjacent_areas);
            for (int i = 0; i < data->area[i].nr_adjacent_areas; i++) {
                fprintf(file, "%s ", data->area[i].adjacent_areas[i]);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

// // Função para ver áreas

void show_areas(d_areas *data) {
    int x;
    // Se não estiver vazio, mostra os dados
    if (!verify_list_areas(start_areas)) {
        clearScreen();
        header();
        for (int i = 0; i < data->size; i++) {
            printf("\n\tIdentificador = %s\n", data->area[i].identifier);
            printf("\n\tCapacidade = %f\n", data->area[i].capacity);
            printf("\n\tNumero de areas adjacentes = %d\n",
                    data->area[i].nr_adjacent_areas);
            printf("\n\tAreas adjacentes = ");
            for (x = 0; x < data->area[i].nr_adjacent_areas; x++) {
                printf("%s ", data->area[i].adjacent_areas[x]);
            }
            printf("\n\n\t-----------------\n");
        }
        printf("\n");
        PressEnterToContinue();
    }
    return;
}

// Função que verifica se a lista areas está vazia

int verify_list_areas(d_areas *data) {
    // Se a lista estiver vazia
    if (data == NULL) {
        clearScreen();
        header();
        printf("\n\tA lista esta vazia!\n\n");
        PressEnterToContinue();
        return 1;
    } else
        return 0;
}
