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

    d_areas *delete_area_data(d_areas *data, char *key);
    d_areas *insert_area_data(d_areas *data, char identifier[SIZE], float capacity, int nr_adjacent_areas, char adjacent_areas[][SIZE]);
    int check_capacity(d_areas *areas_data, list_animals *animals_data, char *identifier, float weight);
    int check_if_area_exists(d_areas *data, char *location);
    void create_area(void);
    void create_areas_list(void);
    void delete_area(list_animals *animals_data);
    void load_areas_data(void);
    void menu_areas(void);
    void save_areas_data(d_areas *data);
    void show_areas(d_areas *data);
    int verify_list_areas(d_areas *data);

#ifdef __cplusplus
}
#endif

#endif /* AREAS_H */

