#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MOVE_DOWN 	0
#define MOVE_LEFT 	1
#define MOVE_UP		2
#define MOVE_RIGHT 	3


struct board {
	int A[4][4];
	int score;
};





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
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
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
			for (int i=0; i<4; i++) {
				s += line_move_ex(&bd->A[i][3], &bd->A[i][2], &bd->A[i][1], &bd->A[i][0]);
			}
		}break;
	}
	
	bd->score += s;
	
	if (s != 0) return s;
	
	return bd_same(&save, bd) ? -1 : 0;
}

void bd_add_ex(struct board *bd, int pos, int v) {
	bd->A[pos/4][pos%4] = v;
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




int bd_search(struct board nowbd, int depth, int *score) {
	if (depth == 0) {
		if (bd_dead(&nowbd)) {
			*score = 0;
		}else {
			*score = nowbd.score;
		}
		return -1;
	}
	
	int arrow, results[4]={0};
	
	for (arrow=0; arrow<4; arrow++) {
		struct board nextbd = nowbd;
		if (bd_move(&nextbd, arrow) == -1) continue;
		
		int pos, cnt=0, score2=0, score4=0;
		for (pos=0; pos<16; pos++) {
			if (nextbd.A[pos/4][pos%4] != 0) continue;
			int s;
			
			bd_add_ex(&nextbd, pos, 2);
			bd_search(nextbd, depth-1, &s);
			score2 += s;
			
			bd_add_ex(&nextbd, pos, 4);
			bd_search(nextbd, depth-1, &s);
			score4 += s;
			
			cnt++;
		}
		
		results[arrow] = 0.9*score2 + 0.1*score4;
	}
	
	int ans = 0;
	for (arrow=1; arrow<4; arrow++)
		if (results[arrow] > results[ans])
			ans = arrow;
	
	*score = results[ans];
	
	return ans;
}


int bd_search_ex(struct board *bd) {
	struct board nowbd = *bd;
	int score;
	int depth = 3;
	
	
//	int i, j, cnt = 0, maxn=0;
//	for (i=0; i<4; i++)
//		for (j=0; j<4; j++)
//			if (bd->A[i][j] != 0) {
//				cnt++;
//				if (bd->A[i][j] > maxn)
//					maxn = bd->A[i][j];
//			}
//	
//	if (cnt <= 8 && maxn <= 64) {
//		int arrow;
//		for (arrow=0; arrow<4; arrow++)
//			if (bd_move(&nowbd, arrow) != -1) {
//				return arrow;
//			}
//	}
//	
	return bd_search(nowbd, depth, &score);
}



int main()
{
	int user_input = 0;
	
	srand(time(0));
	struct board* bd = bd_new();
	bd_init(bd);
	
	bd_print(bd);
	while (!bd_dead(bd)) {
		printf(">");
		
		int move = -1;
		if (user_input) {
			char buf[128];
			scanf("%s", buf);
		
			if (buf[0] == 'a')
				move = MOVE_LEFT;
			if (buf[0] == 's')
				move = MOVE_DOWN;
			if (buf[0] == 'd')
				move = MOVE_RIGHT;
			if (buf[0] == 'w')
				move = MOVE_UP;
		}else {
			move = bd_search_ex(bd);
		}
		
		if (bd_move(bd, move) != -1) {
			bd_add(bd);
			system("cls");
			bd_print(bd);
		}else {
			printf("Can't move, please retry other arrow\n");
			//break;
		}
	}
	
	printf("GAME OVER!!!!!\n\n");
	
	printf("Your score: %d\n", bd->score);
	
	return 0;
}
