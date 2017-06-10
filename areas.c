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

d_areas *delete_area_data(d_areas *list_areas, char key[SIZE]) {

    // Percorre todos os arrays desde 0 até ao numero de areas inseridas
    for (int i = 0; i < list_areas->size; i++) {
        // Se encontrar
        if (strcmp(key, list_areas->area[i].identifier) == 0) {
            for (int j = i; j < list_areas->size - 1; j++) {
                list_areas->area[j] = list_areas->area[j + 1];
            }
            // Como foi removido decrementa 1 ao size
            list_areas->size--;
            // Realocar memória
            list_areas->area = realloc(list_areas->area, sizeof (t_areas) * list_areas->size);
            printf("\n\tEliminado!\n\n");
            PressEnterToContinue();
            return list_areas;
        }
    }
    printf("\n\tNenhum resultado encontrado!\n\n");
    PressEnterToContinue();
    return list_areas;
}

// Função que insere os dados recebidos na estrutura de dados

d_areas *insert_area_data(d_areas *list_areas, char identifier[SIZE], float capacity,
        int nr_adjacent_areas, char adjacent_areas[][SIZE]) {

    int x;

    if (list_areas->area == NULL) {
        // allocar memoria para o array
        list_areas = (d_areas *) malloc(sizeof (d_areas));
        list_areas->area = malloc(sizeof (t_areas));
        list_areas->size = 0;
    }

    list_areas->size++;
    // Alocar memória para a posição atual
    if (list_areas->size > 1) {
        list_areas->area = realloc(list_areas->area, sizeof (t_areas) * list_areas->size);
    }
    int index = list_areas->size - 1;

    // Copiar os dados recebidos para a estrutura
    strncpy(list_areas->area[index].identifier, identifier, strlen(identifier) + 1);
    list_areas->area[index].capacity = capacity;
    list_areas->area[index].nr_adjacent_areas = nr_adjacent_areas;
    for (x = 0; x < nr_adjacent_areas; x++) {
        strncpy(list_areas->area[index].adjacent_areas[x], adjacent_areas[x],
                strlen(adjacent_areas[x]) + 1);
    }

    return list_areas;
}

// Verificar se a capacidade da área é excedida

int check_capacity(d_areas *list_areas, node_animals *list_animals,
        char identifier[SIZE], float weight) {

    node_animals *data_animals = list_animals;

    float area_capacity, total_animals_weight;

    // Verificar a capacidade de determinada area
    for (int i = 0; i < list_areas->size; i++) {
        if (strcmp(identifier, list_areas->area[i].identifier) == 0) {
            area_capacity = list_areas->area[i].capacity;
            break;
        }
    }
    /*
        Verificar todos os animais de determinada localização e contabilizar 
        o seu peso
     */
    for (; data_animals != NULL; data_animals = data_animals->prox) {
        if (strcmp(identifier, data_animals->location) == 0) {
            total_animals_weight = total_animals_weight + data_animals->weight;
        }
    }
    /*
         Se o total de peso dos animais mais o peso do animal inserido for maior
        que a capacidade da àrea não é suportado
     */
    if ((total_animals_weight + weight) <= area_capacity) {
        // A área suporta o animal
        return 1;
    } else {
        return 0;
    }
}

// Verificar se as áreas são adjacentes

int check_adjacent(d_areas *list_areas, char old_area[SIZE], char new_area[SIZE]) {

    // Percorrer as áreas
    for (int i = 0; i < list_areas->size; i++) {
        // Encontrar a area que o animal tem
        if (strcmp(old_area, list_areas->area[i].identifier) == 0) {
            for (int x = 0; x < list_areas->area[i].nr_adjacent_areas; x++) {
                // Verificar se a nova àrea consta nas adjacentes
                if (strcmp(new_area, list_areas->area[i].adjacent_areas[x]) == 0) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }

}

// Verificar se a área existe

int check_if_area_exists(d_areas *list_areas, char location[SIZE]) {

    for (int i = 0; i < list_areas->size; i++) {
        if (strcmp(location, list_areas->area[i].identifier) == 0) {
            return 1;
        }
    }
    return 0;
}

// Função para criar uma nova área

d_areas *create_area(d_areas *list_areas) {

    t_areas load;

    int x;

    clearScreen();
    header();

    // Evitar o registo de uma area ja existente
    do {
        printf("\n\tDigite o identificador da area : ");
        scanf(" %49[^\n]", load.identifier);
    } while (check_if_area_exists(list_areas, load.identifier) == 1 &&
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
                check_if_area_exists(list_areas, load.adjacent_areas[x]) == 0 &&
                printf(
                "\n\t%s nao e uma area existente! Insira uma area existente...\n",
                load.adjacent_areas[x]));
    }

    // Enviar dados recebidos para a função que copia os dados para a Estrutura
    list_areas = insert_area_data(list_areas, load.identifier, load.capacity,
            load.nr_adjacent_areas, load.adjacent_areas);

    printf("\n\tArea creada com sucesso!\n\n");
    PressEnterToContinue();
    return list_areas;
}

// Função para definir a area a apagar

d_areas *delete_area(d_areas *list_areas, node_animals *list_animals) {

    node_animals *data_animals = list_animals;

    char key[SIZE];
    int found = 0;

    clearScreen();
    header();

    printf("\n\tDigite o nome da area a remover : ");
    scanf(" %49[^\n]", key);
    for (; data_animals != NULL; data_animals = data_animals->prox) {
        if (strcmp(key, data_animals->location) == 0) {
            printf("\n\tNome = %s\n", data_animals->name);
            found++;
        }
    }
    // Verificar se nenhum animal se encontra na area antes de a remover
    if (found == 0) {
        // Se nao, enviar a area a apagar para a funçao que apaga
        list_areas = delete_area_data(list_areas, key);
    } else {
        printf("\n\tNao e possivel remover areas que contenham animais!\n\n");
        PressEnterToContinue();
    }
    return list_areas;
}

// Função que carrega os dados relativos às áreas no inicio do programa

d_areas *load_areas_data(d_areas *list_areas) {

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
        // Ler o identificador, capacidade e numero de areas adjacentes
        while (fscanf(file, "%s %f %d", load.identifier, &load.capacity,
                &load.nr_adjacent_areas) == 3) {
            /*
                        ver o numero de areas adjacentes e ler areas adjacentes mediante
                        esse numero
             */
            for (int i = 0; i < load.nr_adjacent_areas; i++) {
                fscanf(file, "%s", load.adjacent_areas[i]);
            }
            // Registar os dados
            list_areas =
                    insert_area_data(list_areas, load.identifier, load.capacity,
                    load.nr_adjacent_areas, load.adjacent_areas);
        }
    }
    fclose(file);
    return list_areas;
}

// Menu areas

void menu_areas(d_areas *list_areas, node_animals *list_animals) {
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
            list_areas = create_area(list_areas);
            break;
        case 2:
            clearScreen();
            list_areas = delete_area(list_areas, list_animals);
            break;
        case 3:
            clearScreen();
            show_areas(list_areas);
            break;
        case 0:
            clearScreen();
            menu(list_animals, list_areas);
            break;
        default:
            printf("\n\t# Opcao invalida\n");
            PressEnterToContinue();
            clearScreen();
            goto menu;
    }

}

// Função que guarda os dados relativos às áreas ao terminar o programa

void save_areas_data(d_areas *list_areas) {

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
        if (list_areas != NULL) {
            // Escrever dados linha por linha ate ao numero de areas inseridas
            for (int i = 0; i < list_areas->size; i++) {
                fprintf(file, "%s %.2f %d ", list_areas->area[i].identifier,
                        list_areas->area[i].capacity, list_areas->area[i].nr_adjacent_areas);
                for (int j = 0; j < list_areas->area[i].nr_adjacent_areas; j++) {
                    fprintf(file, "%s ", list_areas->area[i].adjacent_areas[j]);
                }
                fprintf(file, "\n");
            }
        }
    }
    fclose(file);
}

// // Função para ver áreas

void show_areas(d_areas *list_areas) {

    int x;
    // Se não estiver vazio, mostra os dados
    if (!verify_list_areas(list_areas)) {
        clearScreen();
        header();
        for (int i = 0; i < list_areas->size; i++) {
            printf("\n\tIdentificador = %s\n", list_areas->area[i].identifier);
            printf("\n\tCapacidade = %f\n", list_areas->area[i].capacity);
            printf("\n\tNumero de areas adjacentes = %d\n",
                    list_areas->area[i].nr_adjacent_areas);
            printf("\n\tAreas adjacentes = ");
            for (x = 0; x < list_areas->area[i].nr_adjacent_areas; x++) {
                printf("%s ", list_areas->area[i].adjacent_areas[x]);
            }
            printf("\n\n\t-----------------\n");
        }
        printf("\n");
        PressEnterToContinue();
    }
    return;
}

// Função que verifica se a lista areas está vazia

int verify_list_areas(d_areas *list_areas) {
    // Se a lista estiver vazia
    if (list_areas == NULL) {
        clearScreen();
        header();
        printf("\n\tA lista esta vazia!\n\n");
        PressEnterToContinue();
        return 1;
    } else
        return 0;
}
