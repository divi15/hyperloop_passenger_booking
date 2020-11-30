#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<string.h>
typedef struct {
    int vertex;
    int weight;
} edge_t;

typedef struct {
    edge_t **edges;
    int edges_len;
    int edges_size;
    int dist;
    int prev;
    int visited;
} vertex_t;

typedef struct {
    vertex_t **vertices;
    int vertices_len;
    int vertices_size;
} graph_t;

typedef struct {
    int *data;
    int *prio;
    int *index;
    int len;
    int size;
} heap_t;


struct passenger
{
  int age;
  char name[10];
  char dest;
  struct passenger * link;
};
void add_vertex (graph_t *g, int i) {
    if (g->vertices_size < i + 1) {
        int size = g->vertices_size * 2 > i ? g->vertices_size * 2 : i + 4;
        g->vertices = realloc(g->vertices, size * sizeof (vertex_t *));
        for (int j = g->vertices_size; j < size; j++)
            g->vertices[j] = NULL;
        g->vertices_size = size;
    }
    if (!g->vertices[i]) {
        g->vertices[i] = calloc(1, sizeof (vertex_t));
        g->vertices_len++;
    }
}

void add_edge (graph_t *g, int a, int b, int w) {
    a = a - 'a';
    b = b - 'a';
    add_vertex(g, a);
    add_vertex(g, b);
    vertex_t *v = g->vertices[a];
    if (v->edges_len >= v->edges_size) {
        v->edges_size = v->edges_size ? v->edges_size * 2 : 4;
        v->edges = realloc(v->edges, v->edges_size * sizeof (edge_t *));
    }
    edge_t *e = calloc(1, sizeof (edge_t));
    e->vertex = b;
    e->weight = w;
    v->edges[v->edges_len++] = e;
}

heap_t *create_heap (int n) {
    heap_t *h = calloc(1, sizeof (heap_t));
    h->data = calloc(n + 1, sizeof (int));
    h->prio = calloc(n + 1, sizeof (int));
    h->index = calloc(n, sizeof (int));
    return h;
}

void push_heap (heap_t *h, int v, int p) {
    int i = h->index[v] == 0 ? ++h->len : h->index[v];
    int j = i / 2;
    while (i > 1) {
        if (h->prio[j] < p)
            break;
        h->data[i] = h->data[j];
        h->prio[i] = h->prio[j];
        h->index[h->data[i]] = i;
        i = j;
        j = j / 2;
    }
    h->data[i] = v;
    h->prio[i] = p;
    h->index[v] = i;
}

int min (heap_t *h, int i, int j, int k) {
    int m = i;
    if (j <= h->len && h->prio[j] < h->prio[m])
        m = j;
    if (k <= h->len && h->prio[k] < h->prio[m])
        m = k;
    return m;
}

int pop_heap (heap_t *h) {
    int v = h->data[1];
    int i = 1;
    while (1) {
        int j = min(h, h->len, 2 * i, 2 * i + 1);
        if (j == h->len)
            break;
        h->data[i] = h->data[j];
        h->prio[i] = h->prio[j];
        h->index[h->data[i]] = i;
        i = j;
    }
    h->data[i] = h->data[h->len];
    h->prio[i] = h->prio[h->len];
    h->index[h->data[i]] = i;
    h->len--;
    return v;
}

void dijkstra (graph_t *g, int a, int b) {
    int i, j;
    a = a - 'a';
    b = b - 'a';
    for (i = 0; i < g->vertices_len; i++) {
        vertex_t *v = g->vertices[i];
        v->dist = INT_MAX;
        v->prev = 0;
        v->visited = 0;
    }
    vertex_t *v = g->vertices[a];
    v->dist = 0;
    heap_t *h = create_heap(g->vertices_len);
    push_heap(h, a, v->dist);
    while (h->len) {
        i = pop_heap(h);
        if (i == b)
            break;
        v = g->vertices[i];
        v->visited = 1;
        for (j = 0; j < v->edges_len; j++) {
            edge_t *e = v->edges[j];
            vertex_t *u = g->vertices[e->vertex];
            if (!u->visited && v->dist + e->weight <= u->dist) {
                u->prev = i;
                u->dist = v->dist + e->weight;
                push_heap(h, e->vertex, u->dist);
            }
        }
    }
}

void print_path (graph_t *g, int i) {
    int n, j;
    vertex_t *v, *u;
    i = i - 'a';
    v = g->vertices[i];
    if (v->dist == INT_MAX) {
        printf("no path\n");
        return;
    }
    for (n = 1, u = v; u->dist; u = g->vertices[u->prev], n++)
        ;
    char *path = malloc(n);
    path[n - 1] = 'a' + i;
    for (j = 0, u = v; u->dist; u = g->vertices[u->prev], j++)
        path[n - j - 2] = 'a' + u->prev;
    printf(" %.*s\n",n, path);
}

void append ( struct passenger **q, struct passenger s ) {
	struct passenger *nn;
	nn=(struct passenger*)malloc(sizeof(struct passenger));
	nn->link=NULL;
	nn->age=s.age;
	strcpy(nn->name,s.name);
	strcpy(nn->dest,s.dest);
	if(*q==NULL)
	{
	    *q=nn;
	}
	else
	{
	    struct passenger *temp=*q;
	    while(temp->link!=NULL)
	    {
	        temp=temp->link;
	    }
	    temp->link=nn;
	}
  }

void display ( struct passenger *q ) {
  struct passenger * temp=q;
  while(temp!=NULL)
  {
    printf("%s ",temp->name);
    printf("%d ",temp->age);
    temp=temp->link;
  }
}
int findmax ( struct passenger *q,graph_t *g ) {
    struct passenger *temp=q;
    int max=0;
    char a;
   while(temp!=NULL)
    {
        if(max<temp->age)
        {
            max=temp->age;
            a=temp->dest;
        }
        temp=temp->link;
    }
    dijkstra(g, 'a', a);
    print_path(g, a);
    return max;
}
void delete ( struct passenger **q, int num ) {
	struct passenger* temp=*q,*temp3=*q,*temp4=*q;
	struct passenger* temp1=*q;
	struct passenger* temp2=*q;
	int count=0,flag=0,i,index;
	while(temp!=NULL)
	{
	    count++;
	    temp=temp->link;
	}
	for(i=1;i<=count;i++)
	{
	    if(temp1->age==num)
	    {
	        index=i;
	        flag=1;
	    }
	    temp1=temp1->link;
	}
	if(flag==1)
	{
	if(index==1)
	{
	    *q=temp2->link;
	    temp2->link=NULL;
	    free(temp2);
	}
	else if(index>1)
	{
	    for(i=1;i<index-1;i++)
	    {
	        temp3=temp3->link;
	    }
	    for(i=1;i<index;i++)
	    {
	        temp4=temp4->link;
	    }
	    temp3->link=temp4->link;
	    temp4->link=NULL;
	    free(temp4);
	}
	}
	else
	{
	    printf ( "Element %d not found\n", num ) ;
	}
}
int count(struct passenger *q)
{
    int count=0;
    while(q!=NULL)
    {
        count++;
        q=q->link;
    }
    return count;
}
int main () {
    struct passenger *p ;
	p=NULL;
	struct passenger s;
  	char ch[10];
  	do
    {
  	printf("1: add_passenger\n");
  	printf("2: start pod\n");
  	printf("3: print_path\n");
  	printf("Enter your choice\n ");
  	int n;
  	scanf("%d",&n);
  	switch(n)
  	{
  	case 1:
	    scanf("%s",s.name); 
		scanf("%d",&s.age);
		scanf("%c",&s.dest);
		s.link=NULL;
		append(&p,s);
		break;
	case 2:
	display(p);
    graph_t *g = calloc(7, sizeof (graph_t));
    add_edge(g, 'a', 'b', 1);
    add_edge(g, 'a', 'c', 1);
    add_edge(g, 'b', 'd', 1);
    add_edge(g, 'b', 'c', 1);
    add_edge(g, 'b', 'e', 1);
    add_edge(g, 'c', 'e', 1);
    add_edge(g, 'd', 'e', 1);
    int num = findmax(p,g);
    delete(&p,num);
    break;
    case 3:
    printf("%d\n",count(p));
    display(p);
    break;
  	}
  	printf("Do you want to add another node? Type Yes/No\n");
    scanf("%s",ch);
    }while(!strcmp(ch,"Yes"));
    return 0;
}
