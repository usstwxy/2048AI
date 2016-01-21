#include <stdio.h>
#include <stdlib.h>
#include "lib2048.h"

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
			
			nextbd.A[pos/4][pos%4] = 2;
			bd_search(nextbd, depth-1, &s);
			score2 += s;
			
			nextbd.A[pos/4][pos%4] = 4;
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



unsigned long long bd_encode(struct board *bd) {
	static int log2[1<<16] = {0};
	
	if (log2[2] == 0) {
		int i, t=1;
		for (i=1; i<16; i++) {
			t = t * 2;
			log2[t] = i;
		}
	}
	
	unsigned long long code = 0;
	int i;
	for (i=0; i<16; i++) {
		int k = log2[bd->A[i/4][i%4]];
		unsigned long long t = k & 0xF;
		t = t<<(i*4);
		code = code | t;
	}
	
	return code;
}

void bd_decode(struct board *bd, unsigned long long code) {
	static int pow2[16] = {0};
	if (pow2[0] == 0) {
		int i, t=1;
		for (i=1; i<16; i++) {
			t *= 2;
			pow2[i] = t;
		}
	}
	
	int i;
	for (i=0; i<16; i++) {
		int k = (code>>(i*4)) & 0xF;
		bd->A[i/4][i%4] = pow2[k];
	}
}




int main(int argc, char *argv[]) {
	int user_input = 0;
	
	unsigned long long codes[10240];
	int codes_cnt = 0;
	
	srand(time(0));
	struct board* bd = bd_new();
	bd_init(bd);
	
	bd_print(bd);
	while (!bd_dead(bd)) {
		printf(">");
		
		codes[codes_cnt++] = bd_encode(bd);
		
		int move = -1;
		if (user_input) {
			char buf[128];
			scanf("%s", buf);
		
			if (buf[0] == 'a') move = MOVE_LEFT;
			if (buf[0] == 's') move = MOVE_DOWN;
			if (buf[0] == 'd') move = MOVE_RIGHT;
			if (buf[0] == 'w') move = MOVE_UP;
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
	
	printf("%d codes\n", codes_cnt);
	
	return 0;
}
