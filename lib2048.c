#include "lib2048.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct board* bd_new() {
	struct board *bd = (struct board *)malloc(sizeof *bd);
	bd->score = 0;
	memset(bd->A, 0, sizeof bd->A);
	return bd;
}

void bd_delete(struct board* bd) {
	free(bd);
}

static int line_move(int R[], int a, int b, int c, int d) {
	if (a+b+c+d == 0) return 0;
	while (a == 0) {
		a = b; b = c; c = d; d = 0;
	}
	while (b == 0 && b+c+d != 0) {
		b = c; c = d; d = 0;
	}
	while (c == 0 && c+d != 0) {
		c = d; d = 0;
	}
	
	int n = 0;
	int s = 0;
	if (a == b && a > 0) { // XX??
		R[n++] = a + b;
		s += a + b;
		if (c == d && c > 0) {
			R[n++] = c + d;
			s += c+d;
		}else {
			if (c > 0) R[n++] = c;
			if (d > 0) R[n++] = d;
		}
	}else { // O????
		if (a > 0) R[n++] = a;
			
		if (b == c && b > 0) { // OXX?
			R[n++] = b + c;
			s += b + c;
			if (d > 0) R[n++] = d;
		}else { // OX??
			if (b > 0) R[n++] = b;
			
			if (c == d && c > 0) { // OX##
				R[n++] = c + d;
				s += c + d;
			}else { // OX#$
				if (c > 0) R[n++] = c;
				if (d > 0) R[n++] = d;
			}
		}
	}
	while (n < 4) R[n++] = 0;
	
	return s;
}

static int line_move_ex(int *a, int *b, int *c, int *d) {
	int A[4] = {0};
	int s = line_move(A, *a, *b, *c, *d);
	*a = A[0];
	*b = A[1];
	*c = A[2];
	*d = A[3];
	return s;
}

int bd_same(struct board *a, struct board *b) {
	int i, j;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			if (a->A[i][j] != b->A[i][j]) return 0;
	return a->score == b->score;
}

int bd_move(struct board *bd, int arrow) {
	struct board save = *bd;
	
	int i, s=0;
	switch (arrow) {
		case MOVE_DOWN: {
			for (i=0; i<4; i++) {                                                                                                              
				s += line_move_ex(&bd->A[3][i], &bd->A[2][i], &bd->A[1][i], &bd->A[0][i]);
			}
		}break;
		
		case MOVE_UP:{
			for (i=0; i<4; i++) {                                                                                                                   
				s += line_move_ex(&bd->A[0][i], &bd->A[1][i], &bd->A[2][i], &bd->A[3][i]);
			}
		}break;
		
		case MOVE_LEFT: {
			for (i=0; i<4; i++) {
				s += line_move_ex(&bd->A[i][0], &bd->A[i][1], &bd->A[i][2], &bd->A[i][3]);
			}
		}break;
			
		case MOVE_RIGHT: {
			for (i=0; i<4; i++) {
				s += line_move_ex(&bd->A[i][3], &bd->A[i][2], &bd->A[i][1], &bd->A[i][0]);
			}
		}break;
	}
	
	bd->score += s;
	
	if (s != 0) return s;
	
	return bd_same(&save, bd) ? -1 : 0;
}

void bd_add(struct board *bd) {
	int x = rand() % 16;
	while (bd->A[x/4][x%4] != 0)
		x = rand() % 16;
	int v = rand() % 10;
	v = (v==0) ? 4 : 2;
	bd->A[x/4][x%4] = v;
}


int bd_dead(struct board *bd) {
	int i, j;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			if (bd->A[i][j] == 0) return 0;
			
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			if (i+1<4 && bd->A[i][j] == bd->A[i+1][j] ||
				j+1<4 && bd->A[i][j] == bd->A[i][j+1])
				return 0;
	return 1;
}

void bd_init(struct board *bd) {
	memset(bd->A, 0, sizeof bd->A);
	
	int x = rand() % 16;
	while (bd->A[x/4][x%4] != 0)
		x = rand() % 16;
	bd->A[x/4][x%4] = 2;
	
	while (bd->A[x/4][x%4] != 0)
		x = rand() % 16;
	bd->A[x/4][x%4] = 2;
	
	bd->score = 0;
}


void bd_print(struct board *bd) {
	printf("2048!!!!\n\n");
	int i, j;
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++)
			if (bd->A[i][j] > 0)
				printf("%6d", bd->A[i][j]);
			else
				printf("     .");
		printf("\n");
	}
	printf("\nscore: %d\n\n", bd->score);
}

