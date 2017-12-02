typedef struct NODE
	{
		struct NODE *next;
		void *item;	
	}node;

	typedef struct
	{
		node *first;
		node *last;
		node *current;
	}list;
