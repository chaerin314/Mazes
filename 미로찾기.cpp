#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAZE_SIZE 15
int treasure_num = 0, trap_num = 0;	//보물, 함정의 갯수를 셀 변수
int treasure_score = 0, trap_score = 0;	//보물점수, 손실점수를 계산할 변수

//=====스택코드 시작=====
#define MAX_STACK_SIZE 100

typedef struct {
	short r;
	short c;
} element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;

//스택 초기화 함수
void init_stack(StackType* s) {
	s->top = -1;
}

//공백 상태 검출 함수
int is_empty(StackType* s) {
	return (s->top == -1);
}

//포화 상태 검출 함수
int is_full(StackType* s) {
	return (s->top == (MAX_STACK_SIZE - 1));
}

//삽입함수
void push(StackType* s, element item) {
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

//삭제함수
element pop(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

//피크함수
element peek(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top];
}
//=====스택 코드의 끝=====

element here = { 1,0 }, entry = { 1,0 };

char maze[MAZE_SIZE][MAZE_SIZE] = {
	{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
	{'e','0','0','0','0','1','c','1','1','1','0','1','1','C','1'},
	{'1','1','1','1','0','1','0','0','1','0','0','b','0','0','1'},
	{'1','0','0','c','0','1','1','0','1','1','1','1','0','1','1'},
	{'1','0','1','1','0','1','1','0','0','0','0','0','0','A','1'},
	{'1','0','C','1','0','1','C','0','1','1','1','0','1','1','1'},
	{'1','0','1','1','0','0','0','1','1','0','1','0','a','1','1'},
	{'1','0','0','0','b','1','1','1','c','0','0','B','0','0','1'},
	{'1','b','1','0','1','0','1','0','1','0','1','1','0','1','1'},
	{'1','1','1','0','0','0','a','0','1','0','1','0','0','0','x'},
	{'1','0','0','0','1','0','1','0','1','0','0','C','1','1','1'},
	{'1','0','1','c','1','0','1','0','B','0','1','0','0','1','1'},
	{'1','0','0','0','0','B','1','0','1','0','1','1','0','1','1'},
	{'1','A','1','1','1','0','1','0','1','0','c','1','C','0','1'},
	{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
};

//위치를 스택에 삽입
void push_loc(StackType* s, int r, int c) {
	if (r < 0 || c < 0) return;
	if (maze[r][c] != '1' && maze[r][c] != '.') {
		element tmp;
		tmp.r = r;
		tmp.c = c;
		push(s, tmp);
	}
}

// 미로를 화면에 출력한다.
void maze_print(char maze[MAZE_SIZE][MAZE_SIZE]) {
	for (int r = 0; r < MAZE_SIZE; r++) {
		for (int c = 0; c < MAZE_SIZE; c++) {
			printf(" %c", maze[r][c]);
		}
		printf("\n");
	}
	printf("\n");
}

//보물을 만나면 만난 보물의 수를 늘리고, 보물점수에 점수를 합산한다.
void  encounter_treasure(char n) {
	(treasure_num)++;

	if (n == 'C') {	//만난 보물이 C일 경우 1점을 더한다.
		treasure_score += 1;
		printf("보물!! 1점 획득\n");
	}
	if (n == 'B') {	//만난 보물이 B일 경우 2점을 더한다.
		treasure_score += 2;
		printf("보물!! 2점 획득\n");
	}
	if (n == 'A') {	//만난 보물이 A일 경우 3점을 더한다.
		treasure_score += 3;
		printf("보물!! 3점 획득\n");
	}

}

//함정을 만나면 만난 함정의 수를 늘리고, 손실점수에 점수를 합산한다.
void  encounter_trap(char n) {
	(trap_num)++;

	if (n == 'c') {	//만난 함정이 c일 경우 1점을 더한다.
		trap_score += 1;
		printf("함정!! 1점 감점\n");
	}
	if (n == 'b') {	//만난 함정이 b일 경우 2점을 더한다.
		trap_score += 2;
		printf("함정!! 2점 감점\n");
	}
	if (n == 'a') {	//만난 함정이 a일 경우 3점을 더한다.
		trap_score += 3;
		printf("함정!! 3점 감점\n");
	}
}

int main(void) {
	int r, c;
	StackType s;
	init_stack(&s);
	here = entry;

	while (maze[here.r][here.c] != 'x') {	//x에 도달할 때까지 반복
		r = here.r;
		c = here.c;

		char n = maze[r][c];	//현재 위치의 문자를 받는다.
		if (n == 'C' || n == 'B' || n == 'A') {	//현재 위치가 보물인 A,B,C일 경우
			encounter_treasure(n);
		}
		if (n == 'c' || n == 'b' || n == 'a') {	//현재 위치가 함정인 a,b,c일 경우
			encounter_trap(n);
		}

		maze[r][c] = '.';	//지나온 길은 .으로 표시한다.
		maze_print(maze);	//미로 그리기

		push_loc(&s, r - 1, c);	//상,하,좌,우 칸을 검사하고 스택에 넣는다.
		push_loc(&s, r + 1, c);
		push_loc(&s, r, c - 1);
		push_loc(&s, r, c + 1);

		if (is_empty(&s)) {
			printf("실패\n");
			return -1;
		}
		else {
			here = pop(&s);
		}
	}
	printf("성공\n");
	printf("만난 보물 갯수: %d, 만난 함정 갯수: %d\n", treasure_num, trap_num);
	printf("보물 점수: %d, 손실 점수: %d\n", treasure_score, trap_score);
	printf("총점: %d\n", (treasure_score - trap_score));	//총점==보물점수-손실점수

	return 0;
}