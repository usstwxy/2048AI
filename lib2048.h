#ifndef LIB2048_H
#define LIB2048_H


#define MOVE_DOWN 	0
#define MOVE_LEFT 	1
#define MOVE_UP		2
#define MOVE_RIGHT 	3


struct board {
	int A[4][4];
	int score;
};


extern struct board* bd_new();
extern void bd_delete(struct board* bd);

extern void bd_init(struct board *bd);
extern int bd_dead(struct board *bd);
extern int bd_same(struct board *a, struct board *b);

extern int bd_move(struct board *bd, int arrow);
extern void bd_add(struct board *bd);

extern void bd_print(struct board *bd);




#endif
