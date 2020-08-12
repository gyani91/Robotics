/*				Maze Solving
This is a Graphical User Interface for testing Algorithms that are used
in various Robotics Competitions throughout the world.
This is a RnD work of our team and no part of it is copied from the internet.

The Maze Solving algorithm currently in use is a non-standard algorithm, called
a greedy-approach in technical terms. This is also a RnD work by our team.

14/10/12

*/

#include<graphics.h>
#include<stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<alloc.h>

/*Used for randomization */
#include<time.h>

// Grid Size
#define xmax 8
#define ymax 5

// Initial/Top-Left Node
#define rwmin  120
#define colmin 40

#define intery (200/(ymax-1))
#define interx intery

#define rwmax  (rwmin + (xmax-1)*interx)
#define colmax (colmin + (ymax-1)*intery)

#define nonodes 8

unsigned int xdes,ydes,xcrnt,ycrnt,xsrc,ysrc,xtemp=0,ytemp=0,count=0;
unsigned int n[4],w[4];

void refresh(void);
void grid(void);
void dead(unsigned int,unsigned int,unsigned int);
void highlight(unsigned int,unsigned int,unsigned int,unsigned int);
void path_hl(struct node **start);

void gra_init(void);
void random_nodes(void);
void visit(unsigned int, unsigned int);
unsigned int generate(unsigned int);

void chk_mark(void);
unsigned int encode(unsigned int,unsigned int);
void decode(unsigned int n);

void algo(void);
void next(void);
void neigh(void);
void weigh(unsigned int x,unsigned int y);

struct node
{
	unsigned int data;
	struct node *link;
};

struct node *uv,*path;

unsigned int fetch(struct node *,unsigned int);
void modify(struct node *,unsigned int,unsigned int);
unsigned int status(unsigned int,unsigned int);

void add_end(struct node **start,unsigned int no);
void deleteall(struct node **q);
void del_last(struct node **start);

void display(struct node *start);

//Add A node at the end of the Link-Last
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

//Modify value of a node in a Link-List
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

//Fetch the value of a node in a Link-Link
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

//Fetch the value of a single bit in a byte
unsigned int status(unsigned int no, unsigned int loc)
{
	return((no&(1<<loc))>>loc);
}

//Delete all the nodes to free the memory for further use
void deleteall(struct node **q)
{
	struct node *t;
	while(*q!=NULL)
	{
		t=*q;
		*q=(*q)->link;
		free(t);
	}
}

//Delete Last node
void del_last(struct node **start)
{
	struct node *temp,*old;

	temp=*start;
	/*Traverse list till desired node is reached*/

	while(temp->link!=NULL)
	{
		old=temp;
		temp=temp->link;
		/*if end of link list is encountered*/
	}
	old->link=NULL;
	/*free memory occupied by the node*/
	free(temp);

	/*Making the backtracked Node Current*/
	decode(old->data);
	xcrnt=xtemp;
	ycrnt=ytemp;
	//printf("\t%d\t%d",xtemp,ytemp);
}

// Display the value of nodes in a Link-List
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

//Graphic Module
//Contructing Horizontal and Vertical Lines to form a grid
void grid(void)
{
	unsigned int i,j;

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
}

// To Encirle a dead node, Source and Destination
void dead(unsigned int x,unsigned int y,unsigned int colour)
{
	setcolor(colour);
	circle(rwmin+(x*interx),colmin+(y*intery),2);
}

// To Highlight the Decided Path
void path_hl(struct node **start)
{
	struct node *temp,*old;
	unsigned int a,b;
	temp=*start;
	/*Traverse list till desired node is reached*/

	while(temp->link!=NULL)
	{
		old=temp;
		temp=temp->link;
		decode(old->data);
		a=xtemp;
		b=ytemp;
		decode(temp->data);
		highlight(a,b,xtemp,ytemp);
	}
}

// Basic Module for Highlighting Edge that is being used by path_hl()
void highlight(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	setcolor(50);
	line(rwmin+(x1*interx),colmin+(y1*intery),rwmin+(x2*interx),colmin+(y2*intery));
}
//End of Graphics Module

//Intialization
//Intializing all the screen and backend components and also used for refreshing the screen
void refresh(void)
{
	grid();
	uv=NULL;
	path=NULL;
	gra_init();
}

//Intializing all the bits on the Unvisited Set as 1
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

/* Randomization of nodes and ensuring that they are not repeated*/
void random_nodes(void)
{
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

		dead(x,y,255);
		//Visiting a Dead Node will remove it from the Unvisited set
		//making it invisible when being consider for the path
		visit(x,y);
	}
}

//Removing a Node from the Unvisited Set
void visit(unsigned int x, unsigned int y)
{
	unsigned int temp;

	temp = fetch(uv,y);
	//printf("\t%d",temp);
	temp &= generate(x);
	modify(uv,y,temp);
}

//Generate a Number of the form 11110111, where location of the 0 is variable
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

//Check if the Source & Destination Entered valid or not
void chk_mark(void)
{
	if(xsrc==xdes && ysrc==ydes)
	{
		printf("Source and Destination are same, way the hell do you need a path");
		return;
	}
	if(!(xsrc>=0 && xdes>=0 && ysrc>=0 && ydes>=0 && xsrc<xmax && xdes<xmax && ysrc<ymax && ydes<ymax))
	{
		printf("Source or Destination are out of the limit of the existing grid");
		return;
	}
	if(!(status(fetch(uv,ysrc),xsrc) && status(fetch(uv,ydes),xdes)))
	{
		printf("Source or Destination is a dead node, not connected");
		return;
	}
	//Highlighting Them
	dead(xsrc,ysrc,125);
	dead(xdes,ydes,50);
	//Making them the current node
	xcrnt=xsrc;
	ycrnt=ysrc;
	visit(xcrnt,ycrnt);
	add_end(&path,encode(xcrnt,ycrnt));

}
//End of Initialization

//Encoding the coordinate into a single byte
unsigned int encode(unsigned int x,unsigned int y)
{
	unsigned int i=0;
	i |= x;
	i=(i<<4);
	i |= y;
	//printf("\t\t%d",i);
	return(i);
}

//Decoding a single byte into two coordinates
void decode(unsigned int n)
{
	xtemp=(n>>4);
	ytemp=((n<<12)>>12);
}

//The Actual Maze Solving Algorithm
void algo(void)
{
	//int i;
	//for(i=0;i<50;i++)
	while(1)
	{
		/*Destination Reached*/
		if(xcrnt==xdes && ycrnt==ydes)
			break;
		neigh();

		/*Perfect position to check whether the node is not reachable*/

		if(xcrnt==xsrc && ycrnt == ysrc && count==0)
		{
			printf("Destination not reachable");
			break;
		}
		next();
		/*Every important to check, as if count = 0, then double backtracking is not possible as
		the statement in this block will add the current node to the Link-List
		getting into an infinite loop of add and backtrack*/
		if(count!=0)
		{
			visit(xcrnt,ycrnt);
			add_end(&path,encode(xcrnt,ycrnt));
		}
	}
	path_hl(&path);
}

//Deciding which node should be the next node and whether to backtrack
void next(void)
{
	unsigned small=255,ch,i;
	if(count==0)
	{
		del_last(&path);
		return;
	}
	for(i=0;i<count;i++)
	{
		if(w[i]<small)
		{
			ch=i;
			small=w[i];
		}
	}
	decode(n[ch]);
	//printf("\n%d\t%d",xtemp,ytemp);
	xcrnt=xtemp;
	ycrnt=ytemp;
}

//Telling weigh(), who all are the possible neighbours of the current node
void neigh(void)
{
	count=0;
	weigh(xcrnt,ycrnt-1);
	weigh(xcrnt,ycrnt+1);
	weigh(xcrnt+1,ycrnt);
	weigh(xcrnt-1,ycrnt);
}

//Judging if the neighbours are alive and unvisited and calculate their distance from the Destination
void weigh(unsigned int x,unsigned int y)
{
	//Checking if the inputted src n des are not out of bound
	if(x>=0 && x<xmax && y>=0 && y<ymax)

		/*Checking if they are not already visited
		  Note: this is to be done after we've checked the bounds, or the function might
		  request program to fetch non-existing values from the Link-List */
		if(status(fetch(uv,y),x))
		{
				//Calculate the distace between this neighbour and the Destination
				w[count]=abs(x-xdes)+abs(y-ydes);
				n[count]=encode(x,y);
				//printf("\n%d\t%d\t%d\t%d",x,y,n[count],w[count]);
				count++;
		}
}
// End of the Algorithm

void main(void)
{
   /* request auto detection */
   int gdriver = DETECT, gmode, errorcode;
   unsigned int ch,a,b;
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
   refresh();
   while(1)
   {
	gotoxy(12,16);
	printf("\n\t1: Randomize Nodes");
	printf("\n\t2: Enter a Node Manually");
	printf("\n\t3: Enter the Source and Destination");
	printf("\n\t4: Find Path");
	printf("\n\t5: Refresh");
	printf("\n\t6: Exit");
	printf("\n\n\tPlease Enter your choice: ");
	scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				random_nodes();
				break;
			case 2:
				printf("\nPlease enter the X-coodinate : ");
				scanf("%d",&a);
				printf("\nPlease enter the Y-coodinate : ");
				scanf("%d",&b);
				if(!(a>=0 && b>=0 && a<xmax && b<ymax))
				{
					printf("\nThis is not a valid node");
					break;
				}
				visit(a,b);
				dead(a,b,255);
				break;
			case 3:
				printf("\nPlease  enter  the  Source  :  ");
				scanf("%d%d",&xsrc,&ysrc);
				printf("\nPlease enter the Destination : ");
				scanf("%d%d",&xdes,&ydes);
				chk_mark();
				break;
			case 4:
				printf("\n\t\t                                       ");
				printf("\n\t\t                                       ");
				printf("\n\t\t                                       ");
				printf("\n\t\t                                       ");
				algo();
				getch();
				break;
			case 5:
				deleteall(&path);
				deleteall(&uv);
				initgraph(&gdriver, &gmode, "c:\\turboc3\\bgi");refresh();
				break;
			case 6:
				deleteall(&path);
				deleteall(&uv);
				closegraph();
				exit(0);
			default:
				printf("\nPlease enter a correct choice  ");
				getch();
		}
	}

}
