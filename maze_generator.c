#include "maze_generator.h"

int main() {

    printf("Width: ");  scanf("%d", &width);
    printf("Height: "); scanf("%d", &height);

    char maze[2 * height + 1][2 * width + 1];
    
    createMaze(maze);
    printMaze(maze);
    printmazefile(maze, width, height);

    return 0;
}

/***********************************************************
 *  DisjointSet 자료 구조를 생성하고 초기화한다.
 *  input  : (int) 자료 구조의 크기
 *  return : (DisjointSet*) 초기화된 유니온-파인드 자료 구조의 포인터
 ***********************************************************/
DisjointSet* createDisjointSet(int size) {
    DisjointSet *ds = (DisjointSet*)malloc(size * sizeof(DisjointSet));
    for (int i = 0; i < size; i++) {
        ds[i].parent = i;   // 부모는 자기 자신
        ds[i].rank = 0;     // 랭크(트리의 높이)는 0 
    }
    return ds;
}

/***********************************************************
 *  요소 i를 포함하는 집합의 루트를 찾는다.
 *  input  : (DisjointSet*) 유니온-파인드 자료 구조의 포인터
 *           (int) 찾고자 하는 요소의 인덱스
 *  return : (int) 요소 i를 포함하는 집합의 루트 인덱스
 ***********************************************************/
int find(DisjointSet* ds, int i) {  
    if (ds[i].parent != i)  // i가 루트가 아니라면
        ds[i].parent = find(ds, ds[i].parent);   // 경로 압축
    
    return ds[i].parent;
}

/***********************************************************
 *  요소 i와 j를 포함하는 두 집합을 합친다.
 *  input  : (DisjointSet*) 유니온-파인드 자료 구조의 포인터
 *           (int) 합칠 첫 번째 요소의 인덱스
 *           (int) 합칠 두 번째 요소의 인덱스
 *  return : 없음
 ***********************************************************/
void unionSets(DisjointSet* ds, int i, int j) {
    int rootI = find(ds, i);    // 1번 집합의 루트(구분자, 대표원소)
    int rootJ = find(ds, j);    // 2번 집합의 루트(구분자, 대표원소)
    if (rootI == rootJ) return; // 이미 같은 집합에 속해있음

    if (ds[rootI].rank > ds[rootJ].rank) {  // 만약 1번 집합이 더 길면
        ds[rootJ].parent = rootI;           // 2번 집합을 1번 집합에 붙인다
    } else if (ds[rootJ].rank > ds[rootI].rank) {    // 만약 2번 집합이 더 길면
        ds[rootI].parent = rootJ;           // 1번 집합을 2번 집합에 붙인다
    } else {                            // 만약 길이가 같다면
        ds[rootJ].parent = rootI;           // 2번 집합을 1번 집합에 붙인다
        ds[rootI].rank++;                   // 1번 집합의 랭크를 증가시킨다
    }
}

/***********************************************************
 *  벽 배열을 랜덤하게 섞는다.
 *  input  : (Wall*) 섞을 벽 배열의 포인터
 *           (int) 배열의 크기
 *  return : 없음
 ***********************************************************/
void shuffle(Wall* array, int n) {  // 랜덤섞기
    srand(time(NULL)); // 시드 초기화
    for (int i = n - 1; i > 0; i--) {   // 배열의 끝부터 시작하여
        int j = rand() % (i + 1);   // 0부터 i까지의 랜덤한 수를 뽑은 뒤
        Wall temp = array[i];   // array[i] 와 array[j]를 바꾼다
        array[i] = array[j];
        array[j] = temp;
    }
}

/***********************************************************
 *  크루스칼 알고리즘을 사용하여 미로를 생성한다.
 *  input  : (char[2 * height + 1][2 * width + 1]) 미로 배열의 포인터
 *  return : 없음
 ***********************************************************/
void createMaze(char maze[2 * height + 1][2 * width + 1]) {
    // 초기화
    for (int y = 0; y < 2 * height + 1; y++)    // 미로 초기화
    for (int x = 0; x < 2 * width + 1; x++)     // 전체 배열에 대해
        if (y % 2 == 0 && x % 2 == 0) maze[y][x] = '+'; // 교차점    
        else if (y % 2 == 0) maze[y][x] = '-';  // 가로벽
        else if (x % 2 == 0) maze[y][x] = '|';  // 세로벽
        else maze[y][x] = ' ';  // 블록의 중심 (비어있다)

    // 벽 리스트 생성

    // 왜 walls의 크기는 width * height * 2인가?
    // 가로 벽은 (width - 1) * height개, 세로 벽은 width * (height - 1)개가 있으므로
    Wall walls[width * height * 2]; // 벽 리스트 선언
    int wallcount = 0;  // 벽 리스트의 크기
    for (int y = 0; y < height; y++)    // 벽 리스트 초기화
    for (int x = 0; x < width; x++) {
        if (x < width - 1) walls[wallcount++] = (Wall){{x, y}, {x + 1, y}};  // 가로 벽
        if (y < height - 1) walls[wallcount++] = (Wall){{x, y}, {x, y + 1}}; // 세로 벽
    }
    // 벽 리스트를 랜덤하게 섞음
    shuffle(walls, wallcount);

    // 크루스칼 알고리즘을 사용하여 미로 생성 - 벽 리스트를 순서대로 탐색하며 두 셀을 연결(벽을 삭제)한다.
    DisjointSet* ds = createDisjointSet(width * height);

    for (int i = 0; i < wallcount; i++) {
        Cell cell1 = walls[i].cell1;    // 벽의 왼쪽    / 위쪽 셀
        Cell cell2 = walls[i].cell2;    // 벽의 오른쪽  / 아래쪽 셀

        int cell1Index = cell1.y * width + cell1.x;    // 1번 셀의 인덱스
        int cell2Index = cell2.y * width + cell2.x;    // 2번 셀의 인덱스
        // 셀의 인덱스는 y * width + x로 계산할 수 있다 (1차원 배열을 2차원으로 봄).
        if (find(ds, cell1Index) != find(ds, cell2Index)) {    // 두 셀이 같은 집합에 속해있지 않다면 
            unionSets(ds, cell1Index, cell2Index); // 두 셀을 연결힌다
            if (cell1.x == cell2.x) maze[cell1.y * 2 + 2][cell1.x * 2 + 1] = ' '; // 세로 벽을 없앰 (x값이 같으므로)
            else maze[cell1.y * 2 + 1][cell1.x * 2 + 2] = ' ';    // 가로 벽을 없앰 (y값이 같으므로)
        }
    }
    // for (int i = 0; i < width * height; i++) printf("%d %d\n", ds[i].parent, ds[i].rank);    // 디버깅용
    free (ds);

    // 시작점과 끝점 설정
    maze[1][1] = 'S';  // 시작점
    maze[2 * height - 1][2 * width - 1] = 'E';  // 끝점
}

/***********************************************************
 *  생성된 미로를 콘솔에 출력한다.
 *  input  : (char[2 * height + 1][2 * width + 1]) 미로 배열의 포인터
 *  return : 없음
 ***********************************************************/
void printMaze(char maze[2 * height + 1][2 * width + 1]) {
    for (int y = 0; y < 2 * height + 1; y++) {
        for (int x = 0; x < 2 * width + 1; x++) 
            printf("%c", maze[y][x]);
        printf("\n");
    }
}

void printmazefile(char maze[2 * height + 1][2 * width + 1], int width, int height) {
    // 파일 이름 형식은 width_height_maze.txt
    // ex) 10_10_maze.txt
    char filename[20];
    //sprintf(filename, "%d_%d_maze.txt", width, height);
    sprintf(filename, "%d_%d_maze.maz", width, height);
    FILE *fp = fopen(filename, "w");
    for (int y = 0; y < 2 * height + 1; y++) {
        for (int x = 0; x < 2 * width + 1; x++) 
            fprintf(fp, "%c", maze[y][x]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}