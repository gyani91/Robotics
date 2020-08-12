#include<graphics.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<stdio.h>
#include<alloc.h>

#define xmax 8
#define ymax 5

#define rwmin  120
#define colmin 140

#define interx 50
#define intery 50

#define rwmax  (rwmin + (xmax-1)*interx)
#define colmax (colmin + (ymax-1)*intery)

#define nonodes 8

void dead(unsigned int,unsigned int);
void highlight(unsigned int,unsigned int,unsigned int,unsigned int);

void random_nodes(void);

void visit(unsigned int, unsigned int);
unsigned int generate(unsigned int);

unsigned int fetch(struct node *,unsigned int);
void modify(struct node *,unsigned int,unsigned int);
unsigned int status(unsigned int,unsigned int);

struct node
{
	unsigned int data;
	struct node *link;
};

struct node *uv;

//void add_beg(struct node **start,int no);
void add_end(struct node **start,unsigned int no);
//void add_bet(struct node *start,int loc,int no);
//void del_ele(struct node **start,int no);
//void del_loc(struct node **start,int no);
//void deleteall(struct node **q);
//int count(struct node *q);
void display(struct node *start);
unsigned int fetch(struct node *start,unsigned int loc);
void modify(struct node *start,unsigned int loc,unsigned int no);

void add_end(struct node **start,unsigned int no)
{
	struct node *temp,*r;

	r=(struct node*) malloc(sizeof(struct node));
	r->data=no;
	r->link=NULL;

	if(*start==NULL)
		*start=r;
	else
	{
		temp=*start;
		while(temp->link!=NULL)
			temp=temp->link;

		temp->link=r;
	}
}
void modify(struct node *start,unsigned int loc,unsigned int no)
{
	struct node *temp,*r;

	unsigned int i;

	temp=start;

	/*Skip to desired position*/
	for(i=0;i<loc;i++)
	{
		/*if end of link list is encountered*/
		if(temp->link==NULL)
		{
			printf("\nThere are less node than %d in the list",loc);
			break;
		}
		temp=temp->link;
	}

	temp->data=no;
}

unsigned int fetch(struct node *start,unsigned int loc)
{
	struct node *temp,*r;

	unsigned int i,no;

	temp=start;

	/*Skip to desired position*/
	for(i=0;i<loc;i++)
	{
		/*if end of link list is encountered*/
		if(temp->link==NULL)
		{
			printf("\nThere are less node than %d in the list",loc);
			break;
		}
		temp=temp->link;
	}

	no=temp->data;
	return(no);
}

void display(struct node *start)
{
	printf("\nLINK LIST:");

	while(start!=NULL)
	{
		printf("\t%d",start->data);
		start=start->link;
	}
	getch();
}

void dead(unsigned int x,unsigned int y)
{
	setcolor(255);
	circle(rwmin+(x*interx),colmin+(y*intery),2);
}

void highlight(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	setcolor(50);
	line(rwmin+(x1*interx),colmin+(y1*intery),rwmin+(x2*interx),colmin+(y2*intery));
}

void random_nodes(void)
{
	/* Randomization of nodes and insuring that they are not repeated*/
	unsigned int i,x,y;
	randomize();
	for(i=0;i<nonodes;i++)
	{
		x=random(xmax);
		y=random(ymax);

		//printf("\n%d\t%d\t%d",i,x,y);
		//printf("\t%d\t%d",status(fetch(uv,y),x),fetch(uv,y));
		if(status(fetch(uv,y),x)==0)
			i--;

		dead(x,y);
		visit(x,y);
	}
}

void visit(unsigned int x, unsigned int y)
{
	unsigned int temp;

	temp = fetch(uv,y);
	//printf("\t%d",temp);
	temp &= generate(x);
	modify(uv,y,temp);
}

unsigned int generate(unsigned int x)
{
	unsigned int com=0,i;
	for(i=0;i<xmax;i++)
	{
		if(i!=x)
			com |= (1<<i);
	}
	return(com);
}

unsigned int status(unsigned int no, unsigned int loc)
{
	return((no&(1<<loc))>>loc);
}

void gra_init(void)
{
	unsigned int b=0,i,j;

	for(j=0;j<ymax;j++)
	{
		for(i=0;i<xmax;i++)
		{
			b |= (1<<i);
		}
		add_end(&uv,b);
	}
}

void main(void)
{
   /* request auto detection */
   int gdriver = DETECT, gmode, errorcode;
   unsigned int i,j;

   /* initialize graphics and local variables */
   initgraph(&gdriver, &gmode, "c:\\turboc3\\bgi");

   /* read result of initialization */
   errorcode = graphresult();
   /* an error occurred */
   if (errorcode != grOk)
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);
   }
   /* draw a maze */
   setcolor(100);

   for(i=rwmin;i<=rwmax;i+=interx)
   {
	line(i,colmin,i,colmax);
   }
   for(j=colmin;j<=colmax;j+=intery)
   {
	line(rwmin,j,rwmax,j);
   }
   highlight(0,0,0,1);
   highlight(6,3,7,3);
   uv=NULL;
   gra_init();
   random_nodes();
   display(uv);

   /* clean up */
   getch();
   closegraph();
}