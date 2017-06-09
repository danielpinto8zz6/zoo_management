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
 * File:   animals.c
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


// Função que apaga o animal mediante o id recebido

list_animals *delete_animal_data(list_animals *data, char *key) {
    int find = 0;
    list_animals *aux, *fresh = data;

    char specie_id[SIZE + 5];
    char id[5];

    // Correr a lista e verificar se encontra a string procurada
    // aumentar
    if (fresh != NULL) {
        strcpy(specie_id, fresh->species);
        sprintf(id, "%04d", fresh->id);
        strcat(specie_id, id);
        if (strcmp(key, specie_id) == 0) {
            aux = fresh;
            fresh = fresh->prox;
            free(aux);
            data = fresh;
            find = 1;
        }
    }
    if (find == 0) { // Senão, procura até encontrar
        // Posiciona na frente do encontro para exclusão
        for (; fresh->prox != NULL; fresh = fresh->prox) {
            strcpy(specie_id, fresh->prox->species);
            sprintf(id, "%04d", fresh->prox->id);
            strcat(specie_id, id);
            if (strcmp(key, specie_id) == 0) {

                aux = fresh->prox;
                // Aponta para o próximo valor válido
                fresh->prox = aux->prox;
                find = 1;
                free(aux);
                break;
            }
        }
    }
    // Se encontrou a procura
    if (find == 1) {
        // Apagar relações familiares
        delete_family_relationships(start_animals, key);
        printf("\n\tEliminado!\n\n");
        PressEnterToContinue();
    } else {
        printf("\n\tNenhum resultado encontrado!\n\n");
        PressEnterToContinue();
    }

    return data;
}

// Inserir dados do animal na lista

list_animals *insert_animal_data(list_animals *data, char species[SIZE],
        char name[SIZE], float weight,
        char location[SIZE], int id, int nr_parents,
        char parents[][SIZE], int nr_descendants,
        char descendants[][SIZE]) {
    int x;
    list_animals *start_animals;
    list_animals *aux = data;
    //------------------------------------------------------------------------
    // Alocar memória para a posição atual
    start_animals = (list_animals *) malloc(sizeof (list_animals));
    // Copiar os dados recebidos para a estrutura
    strncpy(start_animals->species, species, strlen(species) + 1);
    strncpy(start_animals->name, name, strlen(name) + 1);
    start_animals->weight = weight;
    strncpy(start_animals->location, location, strlen(location) + 1);
    start_animals->id = id;
    start_animals->nr_parents = nr_parents;
    for (x = 0; x < nr_parents; x++) {
        strncpy(start_animals->parents[x], parents[x], strlen(parents[x]) + 1);
    }
    start_animals->nr_descendants = nr_descendants;
    for (x = 0; x < nr_descendants; x++) {
        strncpy(start_animals->descendants[x], descendants[x],
                strlen(descendants[x]) + 1);
    }

    // start_animals->id =
    //     generate_animal_id(start_animals, start_animals->species, id);
    // printf("%d", start_animals->id);
    // PressEnterToContinue();

    // Procurar por espécie
    // Percorrendo todas as posições
    if (start_animals->id == 0) { // is a new animal
        int id_max = 0;
        for (; aux != NULL; aux = aux->prox) {
            // Se encontrar, compare the id
            if (strcmp(species, aux->species) == 0) {
                if (aux->id > id_max)
                    id_max = aux->id;
            }
        }
        start_animals->id = id_max + 1;
    }

    // //-----------------------------------------------------------------------------
    // Se os dados forem inseridos no inicio do programa aponta para a
    // proxima posição da lista
    // Caso contrário aponta para a lista ja existente
    if (data == NULL) {
        // Apontando para a próxima posição da lista
        start_animals->prox = NULL;
    } else {
        // O próximo valor aponta para a lista já existente
        start_animals->prox = data;
    }

    return start_animals;
}

// Recebe informações acerca do nascimento de um animal

void born_animal(list_animals *data) {

    list_animals *aux = data;
    list_animals load;

    char specie_id[SIZE + 5];
    char id[5];

    clearScreen();
    header();

    printf("\n\tDigite o nome do animal : ");
    scanf(" %49[^\n]", load.name);

    do {
        printf("\n\tVai inserir 1 ou 2 projenitores : ");
        scanf("%d", &load.nr_parents);
    } while (load.nr_parents != 1 && load.nr_parents != 2);

    printf("\n\tDigite o id do projenitor : ");
    scanf(" %49[^\n]", load.parents[0]);
    if (check_if_animal_exists(start_animals, load.parents[0]) == 0) {
        printf("\n\tNao existe nenhum projenitor com o id %s, a terminar!\n",
                load.parents[0]);
        PressEnterToContinue();
        return;
    } else {
        for (; data != NULL; data = data->prox) {
            strcpy(specie_id, data->species);
            sprintf(id, "%04d", data->id);
            strcat(specie_id, id);

            if (strcmp(load.parents[0], specie_id) == 0) {
                strncpy(load.species, data->species, strlen(data->species) + 1);
                load.weight = data->weight * 0.20;
                strncpy(load.location, data->location, strlen(data->location) + 1);
                break;
            }
        }
    }
    if (load.nr_parents == 2) {
        printf("\n\tDigite o id do projenitor 2 : ");
        scanf(" %49[^\n]", load.parents[1]);
        if (check_if_animal_exists(start_animals, load.parents[1]) == 0) {
            printf("\n\tNao existe nenhum projenitor com o id %s, a terminar!\n",
                    load.parents[1]);
            PressEnterToContinue();
            return;
        } else {
            // aux porque data ja foi percorrido, está na posição prox, o aux
            // funciona como auxiliar
            for (; aux != NULL; aux = aux->prox) {

                strcpy(specie_id, data->species);
                sprintf(id, "%04d", data->id);
                strcat(specie_id, id);

                if (strcmp(load.parents[1], specie_id) == 0) {
                    // Certificar que os projenitores são da mesma espécie
                    if (strcmp(load.species, aux->species) == 0) {
                        // Certificar que os projenitores estão na mesma localização
                        if (strcmp(load.location, aux->location) == 0) {
                            load.weight = load.weight + (aux->weight * 0.20);
                        } else {
                            printf("\n\tOs projenitores nao se encontram na mesma area! A "
                                    "terminar...\n");
                            PressEnterToContinue();
                            menu_animals();
                        }
                    } else {
                        printf("\n\tOs projenitores nao sao da mesma especie! A "
                                "terminar...\n");
                        PressEnterToContinue();
                        menu_animals();
                    }
                    break;
                }
            }
        }
    }

    // Se não exceder o total, iserir o animal
    if (check_capacity(start_areas, start_animals, load.location, load.weight) ==
            1) {
        // Inserir animal
        start_animals = insert_animal_data(start_animals, load.species, load.name,
                load.weight, load.location, 0,
                load.nr_parents, load.parents, 0, 0);

        char descendant_id[SIZE + 5], id[5];
        strcpy(descendant_id, load.species);
        sprintf(id, "%04d", generate_animal_id(start_animals, load.species, 0));
        strcat(descendant_id, id);
        // Inserir filho no/s projetinor/s
        insert_descendant_data(start_animals, load.nr_parents, load.parents,
                descendant_id);

        printf("\n\t%s registado com sucesso!\n", load.name);
    } else {
        printf("\n\t%s excede a capacidade total da area, a ignorar\n", load.name);
    }
    printf("\n");
    PressEnterToContinue();
    return;
}

// Verifica se determinado animal existe

int check_if_animal_exists(list_animals *data, char *key) {
    char specie_id[SIZE + 5];
    char id[5];

    for (; data != NULL; data = data->prox) {
        strcpy(specie_id, data->species);
        sprintf(id, "%04d", data->id);
        strcat(specie_id, id);

        if (strcmp(key, specie_id) == 0) {
            return 1;
            break;
        }
    }

    return 0;
}

// Funcao que define a lista como vazia.

void create_animals_list(void) {
    start_animals = NULL;
}

// Função que recebe o id do animal a remover

void delete_animal(void) {
    char key[SIZE];
    // Se não estiver vazio
    if (!verify_list_animals(start_animals)) {
        clearScreen();
        header();
        printf("\n\tDigite o id do animal a remover : ");
        scanf(" %49[^\n]", key);
        start_animals = delete_animal_data(start_animals, key);
    }
}

// Apagar relações familiares 

void delete_family_relationships(list_animals *data, char key[SIZE]) {
    int x, i;

    for (; data != NULL; data = data->prox) {
        // Apagar de projenitores
        for (x = 0; x < data->nr_parents; x++) {
            if (strcmp(key, data->parents[x]) == 0) {
                // If delete position is invalid
                for (i = x - 1; i < data->nr_parents - 1; i++) {
                    // Copy next element value to current element
                    strcpy(data->parents[i], data->parents[i + 1]);
                }
                data->nr_parents--;
            }
        }
        // Apagar de descendentes
        for (x = 0; x < data->nr_descendants; x++) {
            if (strcmp(key, data->descendants[x]) == 0) {
                // If delete position is invalid
                for (i = x - 1; i < data->nr_descendants - 1; i++) {
                    // Copy next element value to current element
                    strcpy(data->descendants[i], data->descendants[i + 1]);
                }
                data->nr_descendants--;
            }
        }
    }
}

// Gerar id para um novo animal

int generate_animal_id(list_animals *data, char species[SIZE], int id) {

    int id_max = 0;

    if (id == 0) { // é um novo animal
        for (; data != NULL; data = data->prox) {
            // Se encontrar, comparar o id
            if (strcmp(species, data->species) == 0) {
                if (data->id > id_max)
                    id_max = data->id;
            }
        }
        id = id_max;
    }
    return id;
}

// Inserir descendência em determinado animal

void insert_descendant_data(list_animals *data, int nr_parents,
        char parents[][SIZE], char descendant[SIZE]) {

    char specie_id[SIZE + 5], id[5];
    int x;

    for (; data != NULL; data = data->prox) {
        strcpy(specie_id, data->species);
        sprintf(id, "%04d", data->id);
        strcat(specie_id, id);
        for (x = 0; x < nr_parents; x++) {
            if (strcmp(parents[x], specie_id) == 0) {
                strncpy(data->descendants[data->nr_descendants++], descendant,
                        strlen(descendant) + 1);
            }
        }
    }
    return;
}

// Carregar dados de animais no inicio do programa

void load_animals_data() {

    // uma variável list_amimal para armazenar os dados lidos
    list_animals load;

    FILE *file;
    file = fopen("animals.dat", "rb");

    clearScreen();
    header();

    // Verificar se é possivel abrir o ficheiro
    if (file == NULL || check_empty(file) == true) {
        printf("\n\tNao foi possivel carregar dados dos animais do zoo!\n\n");
        PressEnterToContinue();
        return;
        // Assumir que o zoo ainda não tem animais
    } else {
        free(start_animals);
        // Os dados são armazenados directamente na variável espécie
        while (fread(&load, sizeof (list_animals), 1, file)) {
            if (check_if_area_exists(start_areas, load.location) == 0) {
                printf("\n\t%s nao tem uma area valida! A ignorar...\n", load.name);
            } else {
                // Se não exceder o total, iserir o animal
                if (check_capacity(start_areas, start_animals, load.location,
                        load.weight) == 1) {
                    start_animals = insert_animal_data(
                            start_animals, load.species, load.name, load.weight,
                            load.location, load.id, load.nr_parents, load.parents,
                            load.nr_descendants, load.descendants);
                    printf("\n\t%s carregado com sucesso!\n", load.name);
                } else {
                    printf("\n\t%s excede a capacidade total da area, a ignorar\n",
                            load.name);
                }
            }
        }
        printf("\n");
        PressEnterToContinue();
    }

    fclose(file);
}

// Função para inserir animais a partir de um ficheiro de texto

void load_animals(void) {
    char fileName[20];

    list_animals load;

    FILE *file;

    clearScreen();
    header();
    printf("\n\t* Para carregar animais atraves de um ficheiro de texto "
            "deve\n "
            "\tinserir 1 animal por linha com a seguinte sintaxe :\n\n"
            "\t\tEspecie Nome Localizacao Peso\n");
    printf("\n\tDigite o nome do ficheiro a carregar : ");
    scanf("%s", fileName);

    file = fopen(fileName, "r");

    // Verificar se é possivel abrir o ficheiro
    if (file == NULL) {
        clearScreen();
        header();
        printf("\n\tErro ao carregar ficheiro, verifica se o mesmo existe e se\n"
                "\tencontra no diretorio do programa!\n\n");
        PressEnterToContinue();
        return;
    } else {

        if (check_empty(file) == true) {
            clearScreen();
            header();
            printf("\n\tO ficheiro esta vazio, nada a carregar!\n\n");
            PressEnterToContinue();
            return;
        } else {
            while (fscanf(file, "%s %s %f %s", load.species, load.name, &load.weight,
                    load.location) == 4) {
                if (check_if_area_exists(start_areas, load.location) == 0) {
                    printf("\n\t%s nao tem uma area valida! A ignorar...\n", load.name);
                } else {
                    // Se não exceder o total, iserir o animal
                    if (check_capacity(start_areas, start_animals, load.location,
                            load.weight) == 1) {
                        start_animals =
                                insert_animal_data(start_animals, load.species, load.name,
                                load.weight, load.location, 0, 0, 0, 0, 0);
                        printf("\n\t%s carregado com sucesso!\n", load.name);
                    } else {
                        printf("\n\t%s excede a capacidade total da area, a ignorar\n",
                                load.name);
                    }
                }
            }
        }
        printf("\n");
        PressEnterToContinue();
    }

    fclose(file);
}

// Menu animais

void menu_animals(void) {

    int op;

menu:

    clearScreen();

    header();

    printf("\n\t# Listagem de Informacao");
    printf("\n\t -> 1 - Listar todos os animais do Zoo");
    printf("\n\t -> 2 - Listar animais por localização");
    printf("\n\t -> 3 - Listar animais por species");
    printf("\n\t -> 4 - Listar um determinado animal");
    printf("\n\t# 5 - Eliminar animal");
    printf("\n\t# 6 - Transferir animal");
    printf("\n\t# 7 - Nascimento de um animal");
    printf("\n\t# 8 - Carregar animais");
    printf("\n\t# 0 - Voltar ao menu principal\n\n");

    printf("\t### : ");
    scanf("%d", &op);

    switch (op) {
        case 1:
            clearScreen();
            show_animals(start_animals);
            break;
        case 2:
            clearScreen();
            search_animals(1);
            break;
        case 3:
            clearScreen();
            search_animals(0);
            break;

        case 4:
            clearScreen();
            search_animals(2);
            break;
        case 5:
            clearScreen();
            delete_animal();
            break;
        case 6:
            clearScreen();
            transfer_animal(start_animals);
            break;
        case 7:
            clearScreen();
            born_animal(start_animals);
            break;
        case 8:
            clearScreen();
            load_animals();
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

// Guardar dados dos animais na lista ao fechar o programa

void save_animals_data(list_animals *data) {

    FILE *file;
    file = fopen("animals.dat", "wb");
    // Verificar se é possivel abrir o ficheiro
    if (file == NULL) {
        clearScreen();
        header();
        printf("\n\tNao foi possivel guardar dados de animais\n\n");
        PressEnterToContinue();
        exit(0);
    } else {
        // Antes de guardar os dados vamos limpar o ficheiro para os mesmo dados nao
        // serem inseridos duas vezes
        save_list_into(data, file);
        /*
        for (; data != NULL; data = data->prox) {
      fwrite(data, sizeof(list_animals), 1, file);
    }
         */
    }
    fclose(file);
}

// Função auxiliar para save_animals_data

void save_list_into(list_animals *data, FILE *file) {
    if (data == NULL)
        return;
    else
        save_list_into(data->prox, file);
    fwrite(data, sizeof (list_animals), 1, file);
    return;
}

// Procura animais mediante id, localização ou espécie

void search_animals_data(list_animals *data, char *key, int filter) {
    int found = 0;
    clearScreen();
    header();

    // Filtro a 0, procurar por espécie
    if (filter == 0) {
        // Percorrendo todas as posições
        for (; data != NULL; data = data->prox) {
            // Se encontrar, mostra os dados
            if (strcmp(key, data->species) == 0) {
                printf("\n\tId = %s%04d\n", data->species, data->id);
                printf("\n\tNome = %s\n", data->name);
                printf("\n\t-----------------\n");
                found++;
            }
        }
    }// Filtro a 1, procurar por localização
    else if (filter == 1) {
        // Percorrendo todas as posições
        for (; data != NULL; data = data->prox) {
            // Se encontrar, mostra os dados
            if (strcmp(key, data->location) == 0) {
                printf("\n\tId = %s%04d\n", data->species, data->id);
                printf("\n\tNome = %s\n", data->name);
                printf("\n\t-----------------\n");
                found++;
            }
        }
    }// Filtro a 2, procurar por nome
    else if (filter == 2) {
        // Percorrendo todas as posições
        for (; data != NULL; data = data->prox) {
            // Se encontrar, mostra os dados
            if (strcmp(key, data->name) == 0) {
                printf("\n\tId = %s%04d\n", data->species, data->id);
                printf("\n\tEspecie = %s\n", data->species);
                printf("\n\tNome = %s\n", data->name);
                printf("\n\tLocalizacao = %s\n", data->location);
                printf("\n\tPeso = %.2f\n", data->weight);
                printf("\n\t-----------------\n");
                found++;
            }
        }
    }

    // Mostrar o resultado da pesquisa
    if (found == 0) {
        printf("\n\tNenhum resultado encontrado.\n\n");
        PressEnterToContinue();
    } else {
        printf("\n\tForam encontrado(s) %d resultado(s).\n\n", found);
        PressEnterToContinue();
    }
}

// Função que recebe o termo a procurar

void search_animals(int filter) {
    char key[SIZE];
    // Se a lista não estiver vazia
    if (!verify_list_animals(start_animals)) {
        clearScreen();
        header();
        // Ler a chave a procurar
        printf("\n\tDigite a procura : ");
        scanf(" %49[^\n]", key);
        // chamando a função que ir?? procurar a chave
        search_animals_data(start_animals, key, filter);
    }
}

// Mostra animais

void show_animals(list_animals *data) {
    int x;
    // Se não estiver vazio, mostra os dados
    if (!verify_list_animals(start_animals)) {
        clearScreen();
        header();
        for (; data != NULL; data = data->prox) {
            printf("\n\tId = %s%04d\n", data->species, data->id);
            printf("\n\tEspecie = %s\n", data->species);
            printf("\n\tNome = %s\n", data->name);
            printf("\n\tLocalizacao = %s\n", data->location);
            printf("\n\tPeso = %.2f\n", data->weight);
            for (x = 0; x < data->nr_parents; x++) {
                printf("\n\tProjenitor %d = %s\n", x + 1, data->parents[x]);
            }
            for (x = 0; x < data->nr_descendants; x++) {
                printf("\n\tDescendente %d = %s\n", x + 1, data->descendants[x]);
            }
            printf("\n\t-----------------\n");
        }
        printf("\n");
        PressEnterToContinue();
    }
}

// Transfere animal de localização

void transfer_animal_data(list_animals *data, char *key, char *area) {

    char specie_id[SIZE + 5];
    char id[5];
    // Quando encontrar o animal a transferir terminar o ciclo e efetuar a
    // transferência
    for (; data != NULL; data = data->prox) {
        strcpy(specie_id, data->species);
        sprintf(id, "%04d", data->id);
        strcat(specie_id, id);
        if (strcmp(key, specie_id) == 0) {
            strncpy(data->location, area, strlen(area) + 1);
            break;
        }
    }
    return;
}

// Recebe dados acerca da transferência do animal

void transfer_animal(list_animals *data) {
    list_animals load;
    char key[SIZE];
    char specie_id[SIZE + 5];
    char id[5];

    // Se a lista não estiver vazia
    if (!verify_list_animals(start_animals)) {
        clearScreen();
        header();
        // Ler a chave a procurar
        printf("\n\tInsira o id do animal a transferir : ");
        scanf(" %49[^\n]", key);
        if (check_if_animal_exists(start_animals, key) == 0) {
            printf("\n\tNao existe nenhum animal com o id %s, a terminar!\n", key);
        } else {
            printf("\n\tInsira a area para qual o deseja transferir : ");
            scanf(" %49[^\n]", load.location);
            if (check_if_area_exists(start_areas, load.location) == 0) {
                printf("\n\t%s nao exite! A terminar...\n", load.location);
            } else {
                // Verificar peso do animal
                for (; data != NULL; data = data->prox) {

                    strcpy(specie_id, data->species);
                    sprintf(id, "%04d", data->id);
                    strcat(specie_id, id);

                    if (strcmp(key, specie_id) == 0) {
                        load.weight = data->weight;
                        break;
                    }
                }
                // Verificar se o animal excede o peso da nova area
                if (check_capacity(start_areas, start_animals, load.location,
                        load.weight) == 1) {
                    // Se não exceder, transferir
                    transfer_animal_data(start_animals, key, load.location);
                    printf("\n\t%s transferido com sucesso!\n", key);
                } else {
                    printf("\n\t%s excede a capacidade total da area, a ignorar\n", key);
                }
            }
        }
        printf("\n");
        PressEnterToContinue();
        return;
    }
}

// Função que verifica se a lista animais está vazia

int verify_list_animals(list_animals *data) {
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









