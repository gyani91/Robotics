/* Relative Coordinates & Directions
   0 ->        (0,-1)
               (-1,0)
               (0,1)
               (1,0)
   
   1 ->        (1,-1)   -1 ->       (1,-1)
               (-1,1)               (-1,1)
               (1,1)                (-1,-1)
               
   2 ->        (0,2)    -2 ->       (0,-2)
               (2,0)                (-2,0)
               (1,0)               (-1,0)
               (0,1)               (0,-1)
               
   3 ->        (1,1)    -3 ->       (-1,-1)  */

#include stdlib.h

int data[3],count=1;
void trans(void);
void wl_config(void);
void avoid(void);

// configure wireless settings

void wl_config(void)
{
	transmit_char('<');
	transmit_char(SID);
	transmit_char(CID);
	transmit_char('>');
}

//transmission of coordinates
void trans(void)
{
     transmit_char(RID);
     transmit_char(X);
     transmit_char(Y);
     transmit_char(direct);
}

// receiving and conversion of data
// collision detection

SIGNAL(SIG_USART_RECV)
{               
        int dx,dy,dd;
        if(count<=3)
        {
            data[count]=atoi(UDR); // Conversion from ASCII to integer
            lcd_goto(count,1);
            write_data(data);
            count++;
        }
        else // collision detection algorithm
        {
            count=1;
            dx=data[count]-X;
            dy=data[count+1]-Y;
            dd=data[count+2]-direct;
            if(dd==1)
            {
                     if((dx==1 && (dy==-1 || dy==1)) || (dx==-1 && dy==1))
                     {
                               avoid_side();
                     }
            }
            else if(dd==-1)
            {
                     if((dx==1 && dy==-1) || (dx==-1 && (dy==1 || dy==-1)))
                     {
                               avoid_side();
                     }
            }
            else if(dd==2)
            {
                     if((dx==0 && (dy==2 || dy==1)) || ((dx==2 || dx==1) && dy==0))
                     {
                               avoid_headon();
                     }
            }
            else if(dd==-2)
            {
                     if((dx==0 && (dy==-2 || dy==-1)) || ((dx==-2 || dx==-1) && dy==0))
                     {
                               avoid_headon();
                     }
            }
            else if(dd==3)
            {
                     if((dx==1 && dy==1))
                     {
                               avoid_side();
                     }
            }
            else if(dd==-3)
            {
                     if((dx==-1 && dy==-1))
                     {
                               avoid_side();
                     }
            }
            else
            {
                     if((dx==0 && (dy==1 || dy==-1)) || ((dx==1 || dx==-1) && dy==0))
                     {
                     }
            }
            
        }
}

// collision avoidance
void avoid_side(void)
{
     REV;
       while(!(s1_clr && s2_clr && s3_clr && s4_clr));
       STOP;
       direct=direct-2;
       xy();
       direct=direct+2;
       i--;
       while(count!=4);
       continue;
}
void avoid_headon(void)
{
     
}

