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

    list_animals *delete_animal_data(list_animals *data, char *key);
    list_animals *insert_animal_data(list_animals *data, char species[SIZE], char name[SIZE], float weight, char location[SIZE], int id, int nr_parents, char parents[][SIZE], int nr_descendants, char descendants[][SIZE]);
    void born_animal(list_animals *data);
    int check_if_animal_exists(list_animals *data, char *key);
    void create_animals_list(void);
    void delete_animal(void);
    void delete_family_relationships(list_animals *data, char key[SIZE]);
    int generate_animal_id(list_animals *data, char species[SIZE], int id);
    void insert_descendant_data(list_animals *data, int nr_parents, char parents[][SIZE], char descendant[SIZE]);
    void load_animals_data();
    void load_animals(void);
    void menu_animals(void);
    void save_animals_data(list_animals *data);
    void save_list_into(list_animals *data, FILE *file);
    void search_animals_data(list_animals *data, char *key, int filter);
    void search_animals(int filter);
    void show_animals(list_animals *data);
    void transfer_animal_data(list_animals *data, char *key, char *area);
    void transfer_animal(list_animals *data);
    int verify_list_animals(list_animals *data);

#ifdef __cplusplus
}
#endif

#endif /* ANIMALS_H */

