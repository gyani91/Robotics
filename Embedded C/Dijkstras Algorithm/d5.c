#define MAX_NODES 1024        /* maximum number of nodes */
#define INFINITY 1000000000      /* a number larger than every maximum path */
int n,dist[MAX_NODES][MAX_NODES];     /*dist[I][j] is the distance from i to j */
void shortest_path(int s,int t,int path[ ])
{struct state {                          /* the path being worked on */
int predecessor ;                     /*previous node */
int length                                /*length from source to this node*/
enum {permanent, tentative} label    /*label state*/
}state[MAX_NODES];
int I, k, min;
struct state *
                     p;
for (p=&state[0];p < &state[n];p++){       /*initialize state*/
p->predecessor=-1
p->length=INFINITY
p->label=tentative;
}
state[t].length=0; state[t].label=permanent ;
k=t ;                                                          /*k is the initial working node */
do{                                                            /* is  the better path from k? */
for I=0; I < n; I++)                                       /*this graph has n nodes */
if (dist[k][I] !=0 && state[I].label==tentative){
            if (state[k].length+dist[k][I] < state[I].length){
		       state[I].predecessor=k;
		       state[I].length=state[k].length + dist[k][I]
		    }
}
/* Find the tentatively labeled node with the smallest label. */
k=0;min=INFINITY;
for (I=0;I < n;I++)
     if(state[I].label==tentative && state[I].length <
min)=state[I].length;
     k=I;
	}
state[k].label=permanent
}while (k!=s);
/*Copy the path into output array*/
I=0;k=0
Do{path[I++]=k;k=state[k].predecessor;} while (k > =0);
}