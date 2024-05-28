#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x, y;
} Cell;

typedef struct {
    Cell cell1, cell2;
} Wall;

typedef struct {
    int parent;
    int rank;
} DisjointSet;

int width, height;

/***********************************************************
 *  유니온-파인드 자료 구조를 생성하고 초기화한다.
 *  input  : (int) 자료 구조의 크기
 *  return : (DisjointSet*) 초기화된 유니온-파인드 자료 구조의 포인터
 ***********************************************************/
DisjointSet* createDisjointSet(int size);

/***********************************************************
 *  요소 i를 포함하는 집합의 루트를 찾는다.
 *  input  : (DisjointSet*) 유니온-파인드 자료 구조의 포인터
 *           (int) 찾고자 하는 요소의 인덱스
 *  return : (int) 요소 i를 포함하는 집합의 루트 인덱스
 ***********************************************************/
int find(DisjointSet* ds, int i);

/***********************************************************
 *  요소 i와 j를 포함하는 두 집합을 합친다.
 *  input  : (DisjointSet*) 유니온-파인드 자료 구조의 포인터
 *           (int) 합칠 첫 번째 요소의 인덱스
 *           (int) 합칠 두 번째 요소의 인덱스
 *  return : 없음
 ***********************************************************/
void unionSets(DisjointSet* ds, int i, int j);

/***********************************************************
 *  벽 배열을 랜덤하게 섞는다.
 *  input  : (Wall*) 섞을 벽 배열의 포인터
 *           (int) 배열의 크기
 *  return : 없음
 ***********************************************************/
void shuffle(Wall* array, int n);

/***********************************************************
 *  크루스칼 알고리즘을 사용하여 미로를 생성한다.
 *  input  : (char[2 * height + 1][2 * width + 1]) 미로 배열의 포인터
 *  return : 없음
 ***********************************************************/
void createMaze(char maze[2 * height + 1][2 * width + 1]);

/***********************************************************
 *  생성된 미로를 콘솔에 출력한다.
 *  input  : (char[2 * height + 1][2 * width + 1]) 미로 배열의 포인터
 *  return : 없음
 ***********************************************************/
void printMaze(char maze[2 * height + 1][2 * width + 1]);

#endif // MAZE_GENERATOR_H
