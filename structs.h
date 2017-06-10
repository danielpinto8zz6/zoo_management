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
 * File:   structs.h
 * Author: daniel
 *
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

    // Estrutura animais

    typedef struct animals {
        char name[SIZE];
        char species[SIZE];
        char location[SIZE];
        int nr_parents;
        char parents[2][SIZE];
        int nr_descendants;
        char descendants[SIZE][SIZE];
        int id;
        float weight;
        struct animals *prox;
    } node_animals;

    // Estrutura areas

    typedef struct areas {
        char identifier[SIZE];
        char adjacent_areas[3][SIZE];
        int nr_adjacent_areas;
        float capacity;
    } t_areas;

    // dynamic arrays of areas

    typedef struct dynamic_array {
        t_areas *area;
        int size;
    } d_areas;

#ifdef __cplusplus
}
#endif

#endif /* STRUCTS_H */

