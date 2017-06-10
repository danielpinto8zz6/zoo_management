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
 * File:   animals.h
 * Author: daniel
 *
 */

#ifndef ANIMALS_H
#define ANIMALS_H

#ifdef __cplusplus
extern "C" {
#endif

    node_animals *delete_animal_data(node_animals *list_animals, char key[SIZE]);
    node_animals *insert_animal_data(node_animals *list_animals, char species[50], char name[50], float weight, char location[50], int id, int nr_parents, char parents[][50], int nr_descendants, char descendants[][50]);
    node_animals *born_animal(node_animals *list_animals);
    int check_if_animal_exists(node_animals *list_animals, char key[SIZE]);
    node_animals *delete_animal(node_animals *list_animals);
    void delete_family_relationships(node_animals *list_animals, char key[50]);
    int generate_animal_id(node_animals *list_animals, char species[50], int id);
    void insert_descendant_data(node_animals *list_animals, int nr_parents, char parents[][50], char descendant[50]);
    node_animals *load_animals_data(node_animals *list_animals, d_areas *list_areas);
    node_animals *load_animals(node_animals *list_animals, d_areas *list_areas);
    void menu_animals(node_animals *list_animals, d_areas *list_areas);
    void save_animals_data(node_animals *list_animals);
    void save_list_into(node_animals *list_animals, FILE *file);
    void search_animals_data(node_animals *list_animals, char key[SIZE], int filter);
    void search_animals(node_animals *list_animals, int filter);
    void show_animals(node_animals *list_animals);
    node_animals *transfer_animal_data(node_animals *list_animals, char key[SIZE], char area[SIZE]);
    node_animals *transfer_animal(node_animals *list_animals, d_areas *list_areas);
    int verify_list_animals(node_animals *list_animals);

#ifdef __cplusplus
}
#endif

#endif /* ANIMALS_H */

