
//#include <stdio.h>
//#include <string.h>
//#include <assert.h>
//#include <malloc.h>
//#include <stdlib.h>
//#include <fcntl.h>

#define  	MIN_MEM_ALLOC 128
#define 	MIN_BLOCK_SIZE sizeof(long)

static long MEM_START;
static long MEM_SIZE;

typedef struct buddy {
	long offset;
	long   size;
	struct buddy *parent;
	struct buddy *sibling;
	struct buddy *lchild, *rchild;
}buddy;

buddy *root;

inline long max(int a, int b)
{
	return (a>b?a:b);
}

inline long abs(long value)
{
   if (value < 0)
       value = -value;
   return value;
}
#ifdef DEBUG
int height_tree(buddy *n)
{
	if (n)
		return max(height_tree(n->lchild), height_tree(n->rchild)) + 1;
	return 0;
}


void print(buddy *n, int lvl)
{
	if (!n)
		return;

	if (lvl == 1) {
		printf(" %d ", n->size);	
	}
	else 
	if (lvl > 1) {
		print(n->lchild, lvl-1);
		print(n->rchild, lvl-1);
	}
	

}
void print_tree(buddy *n)
{
#if 1
	if (n) {
		printf("node: %x, offset = %d, parent = %x, left = %x, \
			right = %x, value = %d \n", n, n->offset, n->parent, n->lchild, n->rchild, n->size); 
		print_tree(n->lchild);
		print_tree(n->rchild);
	}		 
	return;
#endif
	int h = height_tree(n);
	int i;
	for (i = 1; i <= h; i++){
		print(n, i);
		printf("\n");
	}
	
	
}
#else
void print_tree(buddy *n)
{
}
#endif

void init_node(buddy *n, long size, long offset, void *parent, void *sibling, void *lchild, void *rchild)
{
//	printf("offset = 0x%x\n", offset);
	n->size = size;
	n->offset = offset;
	n->parent = parent;
	n->sibling = sibling;
	n->lchild = lchild;
	n->rchild = rchild;
	
}

void mem_init(long start, long size)
{
	MEM_START = start;
	root = (buddy *)MEM_START;
	MEM_SIZE = size - sizeof(buddy);
	init_node(root, MEM_SIZE, MEM_START + sizeof(buddy), root, root, 0, 0);
}


buddy *split_node(buddy *n)
{
	n->size   = n->size >> 1;

	n->lchild = (buddy *)(n->offset);
	n->rchild = (buddy *)(n->offset + n->size);

	init_node(n->lchild, n->size - sizeof(buddy), n->offset + sizeof(buddy) , n, n->rchild, 0, 0);
	init_node(n->rchild, n->size - sizeof(buddy), n->offset + n->size + sizeof(buddy), n, n->lchild, 0, 0);

	n->size = n->lchild->size;
	return n->lchild; 
}

void update_parent(buddy *n)
{
	// go to the top and update parent size, comparing with siblings
	while (n->parent != n) {	//go upto root node
		n = n->parent;
		n->size = max(n->lchild->size, n->rchild->size);
	}	
	if (n->lchild)
		n->size = max(n->lchild->size, n->rchild->size);

}


void *kmalloc(long sz)
{

	/* find the block which satisfies our request 
 	 * we maintain block header and data after header
 	   ------------
 	  |header| data|
          /------------\
         /              \
	/                \
       /                  \
   ------------        ------------
  |header| data|       |header| data|
   ------------         ------------
	
This helps us in not maintaining separate data structure for headers (self sufficient).

Allocation
==========
When allocating, we make sure that "
	sizeof(header) + sizeof(data) room is availble and allocate space for it and return pointer to data.

Deallocation
============
When freeing, the data pointer is passed, then we use that to get the header and merge with sibblings if they are
free and go up to the top.


	 * 1. max-free memory is always availble at the top (parent) node,i.e its reflects the max available memory
	 		at the child nodes.

	 * 2. When we split nodes we will always do it into two equal parts, and the parent will always maintain the 
			max of left, and right child.
	 */	

	long size = sz; 
	buddy *cn = root;

	if (size < MIN_BLOCK_SIZE)
		size = MIN_BLOCK_SIZE;

	while (size <= cn->size) {

		/* request can be served either with left block or right block */
		if (!cn->lchild) {			// left and right exist together, so, node is single
			if ( 2 * (size + sizeof(buddy)) > cn->size) {	// no use of splitting, return the full size
				cn->size = 0;
				update_parent(cn);
				return (void *)(cn->offset); 
			}
			split_node(cn);		// now, split the node and take left
			cn = cn->lchild;
			continue;
		}	
		else {				//node is already split, check for left size, we go left
			if (size <= cn->lchild->size)
				cn = cn->lchild;
			else
				cn = cn->rchild;
		}
	
	}
	return 0;	
		
}

void kfree(void *addr)
{
	buddy *n = (buddy *)(addr - sizeof(buddy));
	buddy *p;
	
	if (n->parent == n) {
		n->size = MEM_SIZE;
		return;
	}
	n->size = abs(n->offset - n->sibling->offset) - sizeof(buddy);
	/* if sibling is not allocated, merge two children and add to parent */
	while (n->size == n->sibling->size) {	
		p = n->parent;
		//printf("merging nodes of node = %x, l = %x, r = %x\n", p, p->lchild, p->rchild);
		p->size = (n->size + sizeof(buddy)) << 1;
		/* this order is important */
		p->lchild = 0;
		p->rchild = 0;
		update_parent(p);
		n = p;
		if (p->parent == p)
			break;
	}
}
#if 0
int main(int argc, char **argv)
{

	long mem = atoi(argv[1]);

	assert(mem >= MIN_MEM_ALLOC);

	MEM_START = (long)malloc(mem);
	printf("mem_start  %x\n", MEM_START);

	init_mem(mem);
	print_tree(root);
	
	FILE *fp = fopen(argv[2], "rw");	
	if (fp == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	long st, end;
	int type;
	while (scanf("%d %d", &type, &mem) != EOF) {
		switch (type) {
		case 1:
			//allocation
			if (st = (long)alloc_mem(mem)) {
				end = st + mem - 1;
				printf("allocated memory: size = %d, st = %d, st = 0x%x, end = 0x%x\n", mem, st, st, end);
				fprintf(fp, "%d", st);
				print_tree(root);
			}
			else {
				printf("Mem insufficient\n");
			}
			break;

		case 2:
			//deallocation
			free_mem(mem);
			print_tree(root);
			break;

		case 3:
			exit(0);
		}
	}
	fflush(fp);	
	fclose(fp);
	fp = fopen(argv[2], "r");
	long addr;	
	while (!feof(fp)) {
		fscanf(fp, "%d", &addr);
	//	assert(addr >= MEM_START && addr < MEM_START + mem);
	//	free_mem(mem);	
		printf("%d\n", addr);
	}
}
#endif
