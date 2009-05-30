#include "basetypes.h"
#include "forbid.h"
#include "string.h"
#include "stdio.h"
#include "globalconst.h"

#define BOARD(v, x, y) (v->data[x][y])

int forbid(Configuration v, int x, int y) {
  int NearBlack[8];
  int NearEmpty[8];
  int EmptyBlack[8];
  int NextEmpty[8];
  int NextBlack[8];
  int i = 0, j = 0, k = 0;
  //Initialize
  for (k = 0; k < 8; ++k)
    NearBlack[k]=NearEmpty[k]=EmptyBlack[k]=NextEmpty[k]=NextBlack[k]=0;
  
  BOARD(v,x,y)=BLACK;

  //Search Up
  for(i=x-1; i>=0&&BOARD(v,i,y)==BLACK; i--, NearBlack[0]++);
  for(;i>=0&&BOARD(v,i,y)==NONE; i--, NearEmpty[0]++);
  for(;i>=0&&BOARD(v,i,y)==BLACK; i--, EmptyBlack[0]++);
  for(;i>=0&&BOARD(v,i,y)==NONE; i--, NextEmpty[0]++);
  for(;i>=0&&BOARD(v,i,y)==BLACK; i--, NextBlack[0]++);
  
  //Search Right Up
  for(i=x-1,j=y+1; i>=0&&j<15&&BOARD(v,i,j)==BLACK; i--,j++,NearBlack[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==NONE; i--,j++, NearEmpty[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==BLACK; i--,j++, EmptyBlack[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==NONE; i--,j++, NextEmpty[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==BLACK; i--,j++, NextBlack[1]++);

  //Search Right
  for(j=y+1; j<15&&BOARD(v,x,j)==BLACK; j++,NearBlack[2]++);
  for(;j<15&&BOARD(v,x,j)==NONE; j++, NearEmpty[2]++);
  for(;j<15&&BOARD(v,x,j)==BLACK; j++, EmptyBlack[2]++);
  for(;j<15&&BOARD(v,x,j)==NONE; j++, NextEmpty[2]++);
  for(;j<15&&BOARD(v,x,j)==BLACK; j++, NextBlack[2]++);
  
  //Search Right Down
  for(i=x+1,j=y+1; i<15&&j<15&&BOARD(v,i,j)==BLACK; i++,j++,NearBlack[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==NONE; i++,j++, NearEmpty[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==BLACK; i++,j++, EmptyBlack[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==NONE; i++,j++, NextEmpty[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==BLACK; i++,j++, NextBlack[3]++);
  
  //Search Down
  for(i=x+1; i<15&&BOARD(v,i,y)==BLACK; i++,NearBlack[4]++);
  for(;i<15&&BOARD(v,i,y)==NONE; i++, NearEmpty[4]++);
  for(;i<15&&BOARD(v,i,y)==BLACK; i++, EmptyBlack[4]++);
  for(;i<15&&BOARD(v,i,y)==NONE; i++, NextEmpty[4]++);
  for(;i<15&&BOARD(v,i,y)==BLACK; i++, NextBlack[4]++);
  
  //Search Left Down
  for(i=x+1,j=y-1; i<15&&j>=0&&BOARD(v,i,j)==BLACK; i++,j--,NearBlack[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==NONE; i++,j--,NearEmpty[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==BLACK; i++,j--, EmptyBlack[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==NONE; i++,j--, NextEmpty[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==BLACK; i++,j--, NextBlack[5]++);
  
  //Search Left
  for(j=y-1;j>=0&&BOARD(v,x,j)==BLACK; j--,NearBlack[6]++);
  for(;j>=0&&BOARD(v,x,j)==NONE; j--,NearEmpty[6]++);
  for(;j>=0&&BOARD(v,x,j)==BLACK; j--, EmptyBlack[6]++);
  for(;j>=0&&BOARD(v,x,j)==NONE; j--,NextEmpty[6]++);
  for(;j>=0&&BOARD(v,x,j)==BLACK; j--, NextBlack[6]++);
  
  //Search Left Up
  for(i=x-1,j=y-1; i>=0&&j>=0&&BOARD(v,i,j)==BLACK; i--,j--,NearBlack[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==NONE; i--,j--, NearEmpty[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==BLACK; i--,j--, EmptyBlack[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==NONE; i--,j--, NextEmpty[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==BLACK; i--,j--, NextBlack[7]++);
  
  BOARD(v,x,y)=NONE;
  
  int dbThree=0, dbFour=0;
  
  for(k=0; k<4; k++)
    {
      if(NearBlack[k]+NearBlack[k+4]>=5)
	return 1;
      else if(NearBlack[k]+NearBlack[k+4]==3)  // ?0000?
	{
	  int four=0;
	  if(NearEmpty[k]>0)                       // ?+0000?
	    {
	      if(key_forbid(v,x,y,NearBlack[k],k)==0)
		four=1;
	    }
	  if(NearEmpty[k+4]>0) //?0000+?
	    {
	      if(key_forbid(v,x,y,NearBlack[k+4],k+4)==0)
		four=1;
	    }
	  if(four) dbFour++;
	  
	}
      else if(NearBlack[k]+NearBlack[k+4]==2) // ?000?
	{
	  if(NearEmpty[k]==1&&EmptyBlack[k]==1) //?0+000?
	    {
	      if(key_forbid(v,x,y,NearBlack[k],k)==0)
		dbFour++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyBlack[k+4]==1) //?000+0?
	    {
	      if(key_forbid(v,x,y,NearBlack[k+4],k+4)==0)
		dbFour++;
	    }
	  
	  int three = 0;
	  
	  //?++000+?
	  if((NearEmpty[k]>2||(NearEmpty[k]==2&&EmptyBlack[k]==0))&&
	     (NearEmpty[k+4]>1||(NearEmpty[k+4]==1&&EmptyBlack[k+4]==0)))
	    {
	      if(key_forbid(v,x,y,NearBlack[k],k)==0)
		three = 1;
	    }
	  //?+000++?
	  if((NearEmpty[k+4]>2||(NearEmpty[k+4]==2&&EmptyBlack[k+4]==0))&&
	     (NearEmpty[k]>1||(NearEmpty[k]==1&&EmptyBlack[k]==0)))
	    {
	      if(key_forbid(v,x,y,NearBlack[k+4],k+4)==0)
		three = 1;
	    }
	  if(three) dbThree++;
	}
      else if(NearBlack[k]+NearBlack[k+4]==1)  // ?00?
	{
	  if(NearEmpty[k]==1&& EmptyBlack[k]==2) //?00+00?
	    {
	      if(key_forbid(v,x,y,NearBlack[k],k)==0)
		dbFour++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyBlack[k+4]==2) //?00+00?
	    {
	      if(key_forbid(v,x,y,NearBlack[k+4],k+4)==0)
		dbFour++;
	    }
	  
	  if(NearEmpty[k]==1&&EmptyBlack[k]==1&&(NextEmpty[k]>1||
						 (NextEmpty[k]==1&&NextBlack[k]==0))&&(NearEmpty[k+4]>1||
										       (NearEmpty[k+4]==1&&EmptyBlack[k+4]==0))) //?+0+00+?
	    {
	      if(key_forbid(v,x,y,NearBlack[k],k)==0)
		dbThree++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyBlack[k+4]==1&&(NextEmpty[k+4]>1||
						     (NextEmpty[k+4]==1&&NextBlack[k+4]==0))&&(NearEmpty[k]>1||
											       (NearEmpty[k]==1&&EmptyBlack[k]==0))) //?+00+0+?
	    {
	      if(key_forbid(v,x,y,NearBlack[k+4],k+4)==0)
		dbThree++;
	    }
	  
	}
      else if(NearBlack[k]+NearBlack[k+4]==0) // ?0?
	{
	  if(NearEmpty[k]==1&&EmptyBlack[k]==3) //?000+0?
	    {
	      if(key_forbid(v,x,y,NearBlack[k],k)==0)
		dbFour++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyBlack[k+4]==3) //?0+000?
	    {
	      if(key_forbid(v,x,y,NearBlack[k+4],k+4)==0)
		dbFour++;
	    }
	  if(NearEmpty[k]==1&&EmptyBlack[k]==2&&(NextEmpty[k]>1||
						 (NextEmpty[k]==1&&NextBlack[k]==0))&&(NearEmpty[k+4]>1||
										       (NearEmpty[k+4]==1&&EmptyBlack[k+4]==0))) //?+00+0+?
	    {
	      if(key_forbid(v,x,y,NearBlack[k],k)==0)
		dbThree++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyBlack[k+4]==2&&(NextEmpty[k+4]>1||
						     (NextEmpty[k+4]==1&&NextBlack[k+4]==0))&&(NearEmpty[k]>1||
											       (NearEmpty[k]==1&&EmptyBlack[k]==0))) //?+00+0+?
	    {
	      if(key_forbid(v,x,y,NearBlack[k+4],k+4)==0)
		dbThree++;
	    }
	}
    }
  
  if(dbThree>1)
    return 2;
  if(dbFour>1)
    return 3;
  
  return 0;
}

int key_forbid(Configuration v,int x, int y, int adjacent,int direction)
{
  if (!allowForbidden())
    return 0;
  int m = 0, n = 0;
  adjacent++;
  if(direction>=4)                     
    adjacent=-adjacent;
  
  switch(direction%4)
    {
    case 0:
      m=x-adjacent;
      n=y;
      break;
    case 1:
      m=x-adjacent;
      n=y+adjacent;
      break;
    case 2:
      m=x;
      n=y+adjacent;
      break;
    case 3:
      m=x+adjacent;
      n=y+adjacent;
      break;
      
    }
  
  BOARD(v,x,y)=BLACK;
  BOARD(v,m,n)=BLACK;

  int result = forbid(v,m,n);

  BOARD(v,m,n)=NONE;
  BOARD(v,x,y)=NONE;
  
  return result;
}


#ifdef TEST_FORBID


int main() {
  struct BaseNode node;
  /*
  memset(&node, 0, sizeof(node));
  node.data[5][5] = 1;
  node.data[6][6] = 1;
  printf("%d\n", forbid(&node, 7, 7));
  */

  /*
  memset(&node, 0, sizeof(node));
  node.data[6][6] = 1;
  node.data[8][8] = 1;
  node.data[6][8] = 1;
  node.data[8][6] = 1;
  printf("%d\n", forbid(&node, 7, 7));
  node.data[9][9] = 1;
  printf("%d\n", forbid(&node, 7, 7));
  node.data[5][9] = 1;
  printf("%d\n", forbid(&node, 7, 7));
  */

  memset(&node, 0, sizeof(node));
  node.data[3][6] = 1;
  node.data[3][8] = 1;
  node.data[5][6] = 1;
  node.data[5][7] = 1;
  node.data[5][8] = 1;
  node.data[6][7] = 1;
  node.data[7][6] = 1;
  node.data[7][8] = 1;
  //node.data[8][7] = 1;
  printf("%d\n", forbid(&node, 7, 7));

  return 0;
}

#endif
