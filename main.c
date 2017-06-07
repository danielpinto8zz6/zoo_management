/*
 * File:   main.c
 * Author: daniel
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50

// Estrutura animais
typedef struct animals {
  char name[SIZE];
  char species[SIZE];
  char location[SIZE];
  char family[SIZE];
  int id;
  float weight;
  struct animals *prox;
} list_animals;

// Estrutura areas
typedef struct areas {
  char identifier[SIZE];
  char adjacent_areas[3][SIZE];
  int nr_adjacent_areas;
  float capacity;
  struct areas *prox;
} list_areas;

// Definir funções previamente para evitar erros
void header(void);
void clearScreen(void);
void clear_newlines(void);
void PressEnterToContinue(void);
int check_capacity(list_areas *areas_data, list_animals *animals_data,
                   char *identifier, float weight);
void transfer_animal_data(list_animals *data, char *key, char *area);
void transfer_animal(list_animals *data);
void load_animals_data(void);
void save_animals_data(list_animals *data);
void save_list_into(list_animals *data, FILE *file);
void show_areas(list_areas *data);
void save_areas_data(list_areas *data);
void load_areas_data(void);
list_areas *insert_area_data(list_areas *data, char identifier[SIZE],
                             float capacity, int nr_adjacent_areas,
                             char adjacent_areas[][SIZE]);
void delete_area(list_animals *animals_data);
list_areas *delete_area_data(list_areas *data, char *key);
void create_area(void);
void menu_areas(void);
int check_if_area_exists(list_areas *data, char *location);
void load_animals(void);
_Bool check_empty(FILE *file);
int check_if_animal_exists(list_animals *data, char *animal);
void born_animal(list_animals *data, list_animals *data_2);
list_animals *insert_animal_data(list_animals *data, char species[SIZE],
                                 char name[SIZE], float weight,
                                 char location[SIZE], int id);
void show_animals(list_animals *data);
int verify_list_animals(list_animals *data);
int verify_list_areas(list_areas *data);
void delete_animal(void);

list_animals *delete_animal_data(list_animals *data, char *key);

void search_animals_data(list_animals *data, char *key, int filter);
void search_animals(int filter);
void menu_animals(void);
int main(void);

// Iniciar os dados das listas
list_animals *start_animals = NULL;
list_areas *start_areas = NULL;

// Cabeçalho do programa
void header() {
  printf("\n\n");
  printf("\t############################################################\n");
  printf("\t###################                       ##################\n");
  printf("\t###################  Zoo Vale das Flores  ##################\n");
  printf("\t###################                       ##################\n");
  printf("\t############################################################\n");
}

// Função para limpar ecrã, funciona de acordo com o SO
void clearScreen() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
  system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
  system("cls");
#endif
}

// Função para limpar o buffer
void clear_newlines(void) {
  int c;
  do {
    c = getchar();
  } while (c != '\n' && c != EOF);
}

// Pressionar enter para continuar, alternativa ao getch()
void PressEnterToContinue(void) {
  printf("\t# Pressione ENTER para continuar\n");
  clear_newlines();
  while (getchar() != '\n')
    ;
}

int check_capacity(list_areas *areas_data, list_animals *animals_data,
                   char *identifier, float weight) {
  float area_capacity, total_animals_weight;
  for (; areas_data != NULL; areas_data = areas_data->prox) {
    if (strcmp(identifier, areas_data->identifier) == 0) {
      area_capacity = areas_data->capacity;
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

// Função para carregar dados
void load_animals_data() {

  // uma variável list_amimal para armazenar os dados lidos
  list_animals load;

  FILE *file;
  file = fopen("animals.dat", "rb");

  clearScreen();
  header();

  // Verificar se é possivel abrir o ficheiro
  if (file == NULL) {
    printf("\n\tNao foi possivel carregar dados dos animais do zoo!\n\n");
    PressEnterToContinue();
    return;
    // Assumir que o zoo ainda não tem animais
  } else {
    free(start_animals);
    // Os dados são armazenados directamente na variável espécie
    while (fread(&load, sizeof(list_animals), 1, file)) {
      if (check_if_area_exists(start_areas, load.location) == 0) {
        printf("\n\t%s nao tem uma area valida! A ignorar...\n", load.name);
      } else {
        // Se não exceder o total, iserir o animal
        if (check_capacity(start_areas, start_animals, load.location,
                           load.weight) == 1) {
          start_animals =
              insert_animal_data(start_animals, load.species, load.name,
                                 load.weight, load.location, load.id);
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

void save_list_into(list_animals *data, FILE *file) {
  if (data == NULL)
    return;
  else
    save_list_into(data->prox, file);
  fwrite(data, sizeof(list_animals), 1, file);
  return;
}

// // Função para ver áreas (não usada no programa, apenas para testes)
void show_areas(list_areas *data) {
  int x;
  // Se não estiver vazio, mostra os dados
  if (!verify_list_areas(start_areas)) {
    clearScreen();
    header();
    for (; data != NULL; data = data->prox) {
      printf("\n\tIdentificador = %s\n", data->identifier);
      printf("\n\tCapacidade = %f\n", data->capacity);
      printf("\n\tNumero de areas adjacentes = %d\n", data->nr_adjacent_areas);
      printf("\n\tAreas adjacentes = ");
      for (x = 0; x < data->nr_adjacent_areas; x++) {
        printf("%s ", data->adjacent_areas[x]);
      }
      printf("\n\n\t-----------------\n");
    }
    printf("\n");
    PressEnterToContinue();
  }
  return;
}

void save_areas_data(list_areas *data) {

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
    for (; data != NULL; data = data->prox) {
      fprintf(file, "%s %.2f %d ", data->identifier, data->capacity,
              data->nr_adjacent_areas);
      for (int i = 0; i < data->nr_adjacent_areas; i++) {
        fprintf(file, "%s ", data->adjacent_areas[i]);
      }
      fprintf(file, "\n");
    }
  }
  fclose(file);
}

void load_areas_data(void) {
  list_areas load;

  FILE *file;
  file = fopen("areas.txt", "r");

  // Verificar se é possivel abrir o ficheiro
  if (file == NULL) {
    clearScreen();
    header();
    printf("\n\tNao foi possivel carregar dados das areas do zoo!\n\n");
    PressEnterToContinue();
    return;
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

// Função que insere os dados recebidos na estrutura de dados
list_areas *insert_area_data(list_areas *data, char identifier[SIZE],
                             float capacity, int nr_adjacent_areas,
                             char adjacent_areas[][SIZE]) {
  int x;
  list_areas *start_areas;
  // Alocar memória para a posição atual
  start_areas = (list_areas *)malloc(sizeof(list_areas));
  // Copiar os dados recebidos para a estrutura
  strncpy(start_areas->identifier, identifier, strlen(identifier) + 1);
  start_areas->capacity = capacity;
  start_areas->nr_adjacent_areas = nr_adjacent_areas;
  for (x = 0; x < nr_adjacent_areas; x++) {
    strncpy(start_areas->adjacent_areas[x], adjacent_areas[x],
            strlen(adjacent_areas[x]) + 1);
  }
  // Se os dados forem inseridos no inicio do programa aponta para a proxima
  // posição da lista
  // Caso contrário aponta para a lista já existente
  if (data == NULL) {
    // Aponta para a próxima posição da lista
    start_areas->prox = NULL;
  } else {
    // O próximo valor aponta para a lista já existente
    start_areas->prox = data;
  }
  return start_areas;
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

// Função que apaga os dados de determinada area
list_areas *delete_area_data(list_areas *data, char *key) {
  int find = 0, cont = 0;
  list_areas *join, *aux, *fresh = data;

  // Correr a lista e verificar se encontrou a string procurada, se sim,
  // aumentar o contador e seta a variável de procura
  for (; fresh != NULL; fresh = fresh->prox) {
    if (strcmp(key, fresh->identifier) == 0) {
      find = 1;
      cont++;
    }
  }

  // Se encontrou a procura
  if (find == 1) {
    int ind = 0;
    // Percorrer a lista
    for (ind = 0; ind < cont; ind++) {
      // Se encontrou na primeira casa apaga a primeira casa
      if (strcmp(key, data->identifier) == 0) {
        aux = data;
        data = data->prox;
        free(aux);
      }
      // Senão, procura até encontrar
      else {
        aux = data;
        // Posiciona na frente para apagar
        while (strcmp(key, aux->identifier) != 0) {
          aux = aux->prox;
        }

        join = data;
        // Enquanto o auxiliar juntou for diferente do posicionado para
        // apagar
        while (join->prox != aux) {
          join = join->prox;
        }
        // Aponta para o próximo valor válido
        join->prox = aux->prox;
        free(aux);
      }
    }
    printf("\n\tEliminado!\n\n");
    PressEnterToContinue();
  } else {
    printf("\n\tNenhum resultado encontrado!\n\n");
    PressEnterToContinue();
  }

  return data;
}

// Função para criar uma nova área
void create_area(void) {
  int x;

  list_areas load;

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

int check_if_area_exists(list_areas *data, char *location) {
  for (; data != NULL; data = data->prox) {
    if (strcmp(location, data->identifier) == 0) {
      return 1;
    }
  }
  return 0;
}

// Função para carregar animais a partir de um ficheiro de texto
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
                                   load.weight, load.location, 0);
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

// Inserir animal (Função que recebe os dados do animal)
void born_animal(list_animals *data, list_animals *data_2) {

  list_animals load;
  char parent_1[SIZE], parent_2[SIZE];
  int nr_parents;

  char specie_id[SIZE + 5];
  char id[5];

  clearScreen();
  header();

  printf("\n\tDigite o nome do animal : ");
  scanf(" %49[^\n]", load.name);

  do {
    printf("\n\tVai inserir 1 ou 2 projenitores : ");
    scanf("%d", &nr_parents);
  } while (nr_parents < 0 && nr_parents > 3);

  printf("\n\tDigite o id do projenitor : ");
  scanf(" %49[^\n]", parent_1);
  if (check_if_animal_exists(start_animals, parent_1) == 0) {
    printf("\n\tNao existe nenhum projenitor com o id %s, a terminar!\n",
           parent_1);
    PressEnterToContinue();
    return;
  } else {
    for (; data != NULL; data = data->prox) {
      strcpy(specie_id, data->species);
      sprintf(id, "%04d", data->id);
      strcat(specie_id, id);

      if (strcmp(parent_1, specie_id) == 0) {
        strncpy(load.species, data->species, strlen(data->species) + 1);
        load.weight = data->weight * 0.20;
        strncpy(load.location, data->location, strlen(data->location) + 1);
        break;
      }
    }
  }
  if (nr_parents == 2) {
    printf("\n\tDigite o id do projenitor 2 : ");
    scanf(" %49[^\n]", parent_2);
    if (check_if_animal_exists(start_animals, parent_2) == 0) {
      printf("\n\tNao existe nenhum projenitor com o id %s, a terminar!\n",
             parent_2);
      PressEnterToContinue();
      return;
    } else {
      // data_2 porque data ja foi percorrido, está na posição prox, o data_2
      // funciona como auxiliar
      for (; data_2 != NULL; data_2 = data_2->prox) {

        strcpy(specie_id, data->species);
        sprintf(id, "%04d", data->id);
        strcat(specie_id, id);

        if (strcmp(parent_2, specie_id) == 0) {
          // Certificar que os projenitores são da mesma espécie
          if (strcmp(load.species, data_2->species) == 0) {
            // Certificar que os projenitores estão na mesma localização
            if (strcmp(load.location, data_2->location) == 0) {
              load.weight = load.weight + (data_2->weight * 0.20);
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
    start_animals = insert_animal_data(
        start_animals, load.species, load.name, load.weight, load.location,
        0); // <--- here we put zero like the code for a new animal
    printf("\n\t%s registado com sucesso!\n", load.name);
  } else {
    printf("\n\t%s excede a capacidade total da area, a ignorar\n", load.name);
  }
  printf("\n");
  PressEnterToContinue();

  return;
}

// Inserir dados recebidos
list_animals *insert_animal_data(list_animals *data, char species[SIZE],
                                 char name[SIZE], float weight,
                                 char location[SIZE], int id) {
  list_animals *start_animals;
  //------------------------------------------------------------------------
  list_animals *aux = data;
  //------------------------------------------------------------------------
  // Alocar memória para a posição atual
  start_animals = (list_animals *)malloc(sizeof(list_animals));
  // Copiar os dados recebidos para a estrutura
  strncpy(start_animals->species, species, strlen(species) + 1);
  strncpy(start_animals->name, name, strlen(name) + 1);
  start_animals->weight = weight;
  strncpy(start_animals->location, location, strlen(location) + 1);
  start_animals->id = id;

  //-----------------------------------------------------------------------------
  //
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

  // sprintf(id, "%04d", start_animals->id);
  // strcat(start_animals->species, id);

  //-----------------------------------------------------------------------------
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

void show_animals(list_animals *data) {
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
      printf("\n\t-----------------\n");
    }
    printf("\n");
    PressEnterToContinue();
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

// Função que verifica se a lista areas está vazia
int verify_list_areas(list_areas *data) {
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

// Função que recebe o nome do animal a remover
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

//-----------------------------------------------------------------------

// Função que apaga o animal definido
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
    printf("\n\tEliminado!\n\n");
    PressEnterToContinue();
  } else {
    printf("\n\tNenhum resultado encontrado!\n\n");
    PressEnterToContinue();
  }

  return data;
}

//-----------------------------------------------------------------------

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
  }
  // Filtro a 1, procurar por localização
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
  }
  // Filtro a 2, procurar por nome
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
    // chamando a função que ir�� procurar a chave
    search_animals_data(start_animals, key, filter);
  }
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
    born_animal(start_animals, start_animals);
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

// Menu principal
int main() {
  int op;

  static bool _isExecutedFirst = false;

  if (!_isExecutedFirst) {
    _isExecutedFirst = true;
    load_areas_data();
    load_animals_data();
  }

menu:

  clearScreen();
  header();

  printf("\n\t# 1 - Areas de Zoo");
  printf("\n\t# 2 - Animais");
  printf("\n\t# 0 - Sair\n\n");

  printf("\t### : ");
  scanf("%d", &op);

  switch (op) {
  case 1:
    clearScreen();
    menu_areas();
    break;
  case 2:
    clearScreen();
    menu_animals();
    break;
  case 0:
    clearScreen();
    save_animals_data(start_animals);
    save_areas_data(start_areas);
    exit(0);
    break;
  default:
    printf("\n\t# Opcao invalida\n");
    PressEnterToContinue();
    clearScreen();
    goto menu;
  }
  goto menu;
}
