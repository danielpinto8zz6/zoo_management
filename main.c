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
 * File:   main.c
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

int main() {

    node_animals *list_animals = NULL;
    //list_animals = malloc(sizeof (node_animals));

    d_areas *list_areas = NULL;
    list_areas = malloc(sizeof (d_areas));
    list_areas->area = NULL;

    static bool _isExecutedFirst = false;

    if (!_isExecutedFirst) {
        _isExecutedFirst = true;
        list_areas = load_areas_data(list_areas);
        list_animals = load_animals_data(list_animals, list_areas);
    }

    menu(list_animals, list_areas);
}

void menu(node_animals *list_animals, d_areas * list_areas) {

    int op;

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
            menu_areas(list_areas, list_animals);
            break;
        case 2:
            clearScreen();
            menu_animals(list_animals, list_areas);
            break;
        case 0:
            clearScreen();
            save_animals_data(list_animals);
            save_areas_data(list_areas);
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
