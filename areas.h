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
 * File:   areas.h
 * Author: daniel
 *
 */

#ifndef AREAS_H
#define AREAS_H

#ifdef __cplusplus
extern "C" {
#endif

    d_areas *delete_area_data(d_areas *list_areas, char key[SIZE]);
    d_areas *insert_area_data(d_areas *list_areas, char identifier[50], float capacity, int nr_adjacent_areas, char adjacent_areas[][50]);
    int check_capacity(d_areas *list_areas, node_animals *list_animals, char identifier[SIZE], float weight);
    int check_adjacent(d_areas *list_areas, char old_area[SIZE], char new_area[SIZE]);
    int check_if_area_exists(d_areas *list_areas, char location[SIZE]);
    d_areas *create_area(d_areas *list_areas);
    d_areas *delete_area(d_areas *list_areas, node_animals *list_animals);
    d_areas *load_areas_data(d_areas *list_areas);
    void menu_areas(d_areas *list_areas, node_animals *list_animals);
    void save_areas_data(d_areas *list_areas);
    void show_areas(d_areas *list_areas);
    int verify_list_areas(d_areas *list_areas);


#ifdef __cplusplus
}
#endif

#endif /* AREAS_H */

