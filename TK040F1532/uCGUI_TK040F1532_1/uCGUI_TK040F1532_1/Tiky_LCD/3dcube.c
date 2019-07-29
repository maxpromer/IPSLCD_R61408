
#include "math.h"               	// include complex math routines
#include "gui.h"

#define OFFSETX 240              	// offset for screen wont change unless
#define OFFSETY 240              	// i use different screen! so its kinda fixed
#define OFFSETZ 40

void cube (void);               	// define the subroutines this one is the actual cube routine

                                	// use const as they are in rom, saving ram

const signed int aa[8]={10,-10,-10,10,   10,-10,-10,10};	// x data for shape vertex
const signed int bb[8]={10,10,-10,-10,   10,10,-10,-10};	// y data for shape vertex
const signed int cc[8]={-10,-10,-10,-10, 10,10,10,10};		// z data for shape vertex

const int ff[12]={1,2,3,4,  5,6,7,8, 1,2,3,4};    		// start vertex for lines
const int gg[12]={2,3,4,1,  6,7,8,5, 5,6,7,8};			// end vertex for lines

int sx,sy,ex,ey;                	// define global vars for calling graphics subroutines

const GUI_POINT aPoints[] = {
{ 10, 10},
{ 20, 10},
{ 20, 20},
{ 10, 20}
};

void cube()                     	// routine to draw and calc 3d cube
 {
 int newx[8];                   	// translated screen x co-ordinates for vertex
 int newy[8];                   	// translated screen y co-ordinates for vertex
 int i,loop;                    	// temp variable for loops
 float xt,yt,zt,x,y,z,sinax,cosax,sinay,cosay,sinaz,cosaz,vertex;  // lots of work variables
 float xpos=0;				// position for object in 3d space, in x
 float ypos=0;				// y
 float zpos=0;				// and z values
 float rotx=0;                  	// starting amount of x rotation
 float roty=0;                 		// starting amount of y rotation
 float rotz=0;                		// starting amount of z rotation
 signed short    face[8];
	GUI_SetBkColor(GUI_BLACK);//设置背景色为黑色
  GUI_Clear();//清屏

 for (loop=0; loop<=100; loop++)	// rotate the cube 100 times
  {
  xpos=xpos+0.0;			// move the object
  ypos=ypos+0.0;			// it would wander off screen
  zpos=zpos+0.0;			// really quick, so leave it centered

  rotx=rotx+0.1;                	// rotate the cube on X axis
  roty=roty+0.1;                	// and on its y axis
  rotz=rotz+0.0;                	// dont bother with z or it gets confusing

  sinax=sin(rotx);			// precalculate the sin and cos values
  cosax=cos(rotx);			// for the rotation as this saves a 
  
  sinay=sin(roty);			// little time when running as we
  cosay=cos(roty);			// call sin and cos less often
  
  sinaz=sin(rotz);			// they are slow routines
  cosaz=cos(rotz);			// and we dont want slow!
	
	
  for (i=0; i<8; i++)           	// translate 3d vertex position to 2d screen position
        {
        x=aa[i];                	// get x for vertex i
        y=bb[i];                	// get y for vertex i
        z=cc[i];                	// get z for vertex i

        yt = y * cosax - z * sinax;	// rotate around the x axis
        zt = y * sinax + z * cosax;	// using the Y and Z for the rotation
        y = yt;
        z = zt;

        xt = x * cosay - z * sinay;	// rotate around the Y axis
        zt = x * sinay + z * cosay;	// using X and Z
        x = xt;
        z = zt;

        xt = x * cosaz - y * sinaz;	// finaly rotate around the Z axis
        yt = x * sinaz + y * cosaz;	// using X and Y
        x = xt;
        y = yt;

        x=x+xpos;			// add the object position offset
        y=y+ypos;			// for both x and y
        z=z+OFFSETZ-zpos;		// as well as Z

        newx[i]=(x*256/z)+OFFSETX;	// translate 3d to 2d coordinates for screen
        newy[i]=(y*256/z)+OFFSETY;	// drawing so we can see the cube
        }

  
  
	
	GUI_SetColor(GUI_GREEN);
  for (i=0; i<12; i++)		// draw the lines that make up the object
		{
			vertex=ff[i]-1;         // temp = start vertex for this line
			sx=newx[(int)vertex];        // set line start x to vertex[i] x position
			sy=newy[(int)vertex];        // set line start y to vertex[i] y position
			vertex=gg[i]-1;         // temp = end vertex for this line
			ex=newx[(int)vertex];	// set line end x to vertex[i+1] x position
			ey=newy[(int)vertex];	// set line end y to vertex[i+1] y position	
//			if(i==0)GUI_SetColor(GUI_RED);//设置前景色
//			if(i==1)GUI_SetColor(GUI_GREEN);//设置前景色
//			if(i==2)GUI_SetColor(GUI_BLUE);//设置前景色
//			if(i==3)GUI_SetColor(GUI_YELLOW);//设置前景色
//			if(i==4)GUI_SetColor(GUI_WHITE);//设置前景色
			if(i<4)
			{
				face[(i%4)*2]=sx;
				face[(i%4)*2+1]=sy;
			}
			GUI_DrawLine(sx, sy, ex, ey);// draw the line between these 2 vertex
		}
		
		GUI_SetColor(GUI_RED);		
		GUI_FillPolygon((const GUI_POINT *)&face,4,0, 0); //填充一面
		
		GUI_Delay(10);
		GUI_SetColor(GUI_BLACK);//设置前景色
		for (i=0; i<12; i++)		// draw the lines that make up the object
		{
			vertex=ff[i]-1;         // temp = start vertex for this line
			sx=newx[(int)vertex];        // set line start x to vertex[i] x position
			sy=newy[(int)vertex];        // set line start y to vertex[i] y position
			vertex=gg[i]-1;         // temp = end vertex for this line
			ex=newx[(int)vertex];	// set line end x to vertex[i+1] x position
			ey=newy[(int)vertex];	// set line end y to vertex[i+1] y position	
			GUI_DrawLine(sx, sy, ex, ey);// draw the line between these 2 vertex
		}
		GUI_SetColor(GUI_BLACK);		
		GUI_FillPolygon((const GUI_POINT *)&face,4,0, 0); //填充一面
  }
 }

