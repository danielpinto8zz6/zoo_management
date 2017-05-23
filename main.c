/*
* File:   main.c
* Author: daniel
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50
#define INCOMPATIBILITY false;

// Estrutura animais
typedef struct animals {
  char name[SIZE];
  char species[SIZE];
  char location[SIZE];
  char family[SIZE];
  float weight;
  struct animals *prox;
} list_animals;

// Estrutura areas
typedef struct areas {
  char identifier[SIZE];
  char adjacent_areas[SIZE];
  int nr_adjacent_areas;
  float capacity;
  struct areas *prox;
} list_areas;

// Definir funções previamente para evitar erros
void header(void);
void clearScreen(void);
void clear_newlines(void);
void PressEnterToContinue(void);
void load_animals_data(list_areas *data);
void save_animals_data(list_animals *data);
void save_areas_data(list_areas *data);
void load_areas_data(void);
list_areas *insert_area_data(list_areas *data, char *identifier, float capacity,
                             int nr_adjacent_areas, char *adjacent_areas);
void delete_area(list_animals *animals_data);
list_areas *delete_area_data(list_areas *data, char *key);
void create_area(void);
void menu_areas(void);
void load_animals(list_areas *data);
bool check_empty(FILE *file);
void born_animal(void);
list_animals *insert_animal_data(list_animals *data, char species[SIZE],
                                 char name[SIZE], float weight,
                                 char location[SIZE]);
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

void transfer_animal_data(list_animals *data, char *key, char *area) {
  int find = 0;

  // Quando encontrar o animal a transferir terminar o ciclo e efetuar a
  // transferência
  for (; data != NULL; data = data->prox) {
    if (strcmp(key, data->name) == 0) {
      find = 1;
      strncpy(data->location, area, strlen(area) + 1);
      break;
    }
  }

  // Se encontrou a procura
  if (find == 1) {
    printf("\n\tTransferido!\n\n");
    PressEnterToContinue();
  } else {
    printf("\n\tNenhum resultado encontrado!\n\n");
    PressEnterToContinue();
  }

  return;
}

void transfer_animal() {
  char key[SIZE], area[SIZE];

  // Se a lista não estiver vazia
  if (!verify_list_animals(start_animals)) {
    clearScreen();
    header();
    // Ler a chave a procurar
    printf("\n\tInsira o nome do animal a transferir : ");
    scanf(" %49[^\n]", key);
    printf("\n\tInsira a area para qual o deseja transferir : ");
    scanf(" %49[^\n]", area);
    // chamando a função que irá procurar a chave
    transfer_animal_data(start_animals, key, area);
  }
}

// Função para carregar dados
void load_animals_data(list_areas *data) {
  char line[256];
  char species[SIZE], name[SIZE], location[SIZE];
  float weight;
  int found = 0;

  FILE *file;
  file = fopen("animals.dat", "rb");

  // Verificar se é possivel abrir o ficheiro
  if (file == NULL) {
    clearScreen();
    header();
    printf("\n\tNao foi possivel carregar dados dos animais do zoo!\n\n");
    PressEnterToContinue();
    return;
    // Assumir que o zoo ainda não tem animais
  } else {
    // Ler o ficheiro linha por linha
    while (fgets(line, sizeof(line), file)) {
      // Em cada linha retirar a seguinte informação e enviar para a função que
      // insere os dados na estrutura
      sscanf(line, "%s %s %f %s", species, name, &weight, location);
      // Apenas aceitar o registo se não ultrapassar a capacidade do local ou se
      // a area não existir

      for (; data != NULL; data = data->prox) {
        if (strcmp(location, data->identifier) == 0) {
          found++;
        }
      }
      // Se o animal não estiver numa área valida será ignorado
      if (found != 0) {
        // Enviar dados recebidos para a função que copia os dados para
        // a estrutura
        start_animals =
            insert_animal_data(start_animals, species, name, weight, location);
      }
    }
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
    for (; data != NULL; data = data->prox) {
      fprintf(file, "%s %s %.2f %s\n", data->species, data->name, data->weight,
              data->location);
    }
  }
  fclose(file);
}

// // Função para ver áreas (não usada no programa, apenas para testes)
void show_areas(list_areas *data) {
  // Se não estiver vazio, mostra os dados
  if (!verify_list_areas(start_areas)) {
    clearScreen();
    header();
    for (; data != NULL; data = data->prox) {
      printf("\n\tIdentificador = %s\n", data->identifier);
      printf("\n\tCapacidade = %f\n", data->capacity);
      printf("\n\tNumero de areas adjacentes = %d\n", data->nr_adjacent_areas);
      printf("\n\tAreas adjacentes = %s\n", data->adjacent_areas);
      printf("\n\t-----------------\n");
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
      fprintf(file, "%s %.2f %d %s\n", data->identifier, data->capacity,
              data->nr_adjacent_areas, data->adjacent_areas);
    }
  }
  fclose(file);
}

void load_areas_data(void) {
  char line[256];
  char identifier[SIZE], adjacent_areas[SIZE];
  float capacity;
  int nr_adjacent_areas;

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
    // Ler o ficheiro linha por linha
    while (fgets(line, sizeof(line), file)) {
      // Em cada linha retirar a seguinte informação e enviar para a função que
      // insere os dados na estrutura
      sscanf(line, "%s %f %d %s", identifier, &capacity, &nr_adjacent_areas,
             adjacent_areas);
      // Não esquecer de implementar condições para o programa apenas aceitar o
      // registo se não ultrapassar a capacidade do local ou se a area não
      // existir

      // Enviar dados recebidos para a função que copia os dados para a
      // Estrutura
      start_areas = insert_area_data(start_areas, identifier, capacity,
                                     nr_adjacent_areas, adjacent_areas);
    }
  }
  fclose(file);
}

// Função que insere os dados recebidos na estrutura de dados
list_areas *insert_area_data(list_areas *data, char *identifier, float capacity,
                             int nr_adjacent_areas, char *adjacent_areas) {
  list_areas *start_areas;
  // Alocar memória para a posição atual
  start_areas = (list_areas *)malloc(sizeof(list_areas));
  // Copiar os dados recebidos para a estrutura
  strncpy(start_areas->identifier, identifier, strlen(identifier) + 1);
  start_areas->capacity = capacity;
  start_areas->nr_adjacent_areas = nr_adjacent_areas;
  strncpy(start_areas->adjacent_areas, adjacent_areas,
          strlen(adjacent_areas) + 1);

  // Se os dados forem inseridos no start do programa aponta para a proxima
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
  char identifier[SIZE], adjacent_areas[SIZE];
  int nr_adjacent_areas = 0, x;
  float capacity;

  clearScreen();
  header();

  printf("\n\tDigite o identificador da area : ");
  scanf(" %49[^\n]", identifier);
  printf("\n\tDigite a capacidade da area : ");
  scanf("%f", &capacity);
  do {
    printf("\n\tDigite o numero de areas adjacentes [max : 3] : ");
    scanf("%d", &nr_adjacent_areas);
  } while (nr_adjacent_areas < 0 || nr_adjacent_areas > 3);
  for (x = 0; x < nr_adjacent_areas; x++) {
    printf("\n\tDigite a area adjacente %d : ", x + 1);
    scanf(" %49[^\n]", adjacent_areas);
  }

  // Enviar dados recebidos para a função que copia os dados para a Estrutura
  start_areas = insert_area_data(start_areas, identifier, capacity,
                                 nr_adjacent_areas, adjacent_areas);

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

// Função para carregar animais a partir de um ficheiro de texto
void load_animals(list_areas *data) {
  char fileName[20], line[256];
  char species[SIZE], name[SIZE], location[SIZE];
  float weight;
  int found = 0, x = 0;

  FILE *file;

  clearScreen();
  header();
  printf("\n\t* Para carregar  = 20animais atraves de um ficheiro de texto "
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
      // Ler o ficheiro linha por linha
      while (fgets(line, sizeof(line), file)) {
        // Em cada linha retirar a seguinte informação e enviar para a função
        // que
        // insere os dados na estrutura
        sscanf(line, "%s %s %f %s", species, name, &weight, location);
        // Não esquecer de implementar condições para o programa apenas aceitar
        // o
        // registo se não ultrapassar a capacidade do local ou se a area não
        // existir

        // Enviar dados recebidos para a função que copia os dados para a
        // Estrutura
        for (; data != NULL; data = data->prox) {
          if (strcmp(location, data->identifier) == 0) {
            found++;
          }
        }
        // Se o animal tiver uma area valida os dados serão registados
        if (found == 0) {
          x++;
        } else { // Enviar dados recebidos para a função que copia os dados para
                 // a
          // Estrutura
          start_animals = insert_animal_data(start_animals, species, name,
                                             weight, location);
        }
      }
    }
  }

  fclose(file);
  if (found == 0) {
    printf("\n\tForam ignorados %d animais por nao se encontrarem numa "
           "area valida\n\n",
           x);
    PressEnterToContinue();
  } else {
    printf("\n\tDados carregados com sucesso!\n\n");
    PressEnterToContinue();
  }
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

// Inserir animal (Função que recebe os dados do animal)
void born_animal(void) {
  char name[SIZE], species[SIZE], location[SIZE];
  float weight;

  clearScreen();
  header();

  printf("\n\tDigite a especie : ");
  scanf(" %49[^\n]", species);
  printf("\n\tDigite o nome : ");
  scanf(" %49[^\n]", name);
  printf("\n\tDigite o peso : ");
  scanf("%f", &weight);
  printf("\n\tDigite a localizacao : ");
  scanf(" %49[^\n]", location);

  // Enviar dados recebidos para a função que copia os dados para a Estrutura
  start_animals =
      insert_animal_data(start_animals, species, name, weight, location);

  printf("\n\tAnimal inserido com sucesso!\n\n");
  PressEnterToContinue();
  menu_animals();
}

// Inserir dados recebidos
list_animals *insert_animal_data(list_animals *data, char species[SIZE],
                                 char name[SIZE], float weight,
                                 char location[SIZE]) {
  list_animals *start_animals;
  // Alocar memória para a posição atual
  start_animals = (list_animals *)malloc(sizeof(list_animals));
  // Copiar os dados recebidos para a estrutura
  strncpy(start_animals->species, species, strlen(species) + 1);
  strncpy(start_animals->name, name, strlen(name) + 1);
  start_animals->weight = weight;
  strncpy(start_animals->location, location, strlen(location) + 1);

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
    printf("\n\tDigite o nome do animal a remover : ");
    scanf(" %49[^\n]", key);
    start_animals = delete_animal_data(start_animals, key);
  }
}

// Função que apaga o animal definido
list_animals *delete_animal_data(list_animals *data, char *key) {
  int find = 0, cont = 0;
  list_animals *join, *aux, *fresh = data;

  // Correr a lista e verificar se encontra a string procurada, se sim,
  // aumentar
  // o contador e setar a variável de procura
  for (; fresh != NULL; fresh = fresh->prox) {
    if (strcmp(key, fresh->name) == 0) {
      find = 1;
      cont++;
    }
  }

  // Se encontrou a procura
  if (find == 1) {
    int ind = 0;
    // Percorrer a lista
    for (ind = 0; ind < cont; ind++) {
      // Se encontrou no primeira casa apaga a primeira casa
      if (strcmp(key, data->name) == 0) {
        aux = data;
        data = data->prox;
        free(aux);
      }
      // Senão, procura até encontrar
      else {
        aux = data;
        // Posiciona na frente do encontro para exclusão
        while (strcmp(key, aux->name) != 0) {
          aux = aux->prox;
        }

        join = data;
        // Enquanto o auxiliar juntou for diferente do posicionado para
        // exclusão
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
    // chamando a função que irá procurar a chave
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
    transfer_animal();
    break;
  case 7:
    clearScreen();
    born_animal();
  case 8:
    clearScreen();
    load_animals(start_areas);
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
    load_animals_data(start_areas);
    load_areas_data();
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
