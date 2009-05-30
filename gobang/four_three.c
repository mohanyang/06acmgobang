#include "four_three.h"
#include "basetypes.h"
#include "forbid.h"

#define BOARD(v, x, y) (v->data[x][y])

int four_three(Configuration v, int y, int x, PEBBLE_COLOR c) {
  int NearColor[8];
  int NearEmpty[8];
  int EmptyColor[8];
  int NextEmpty[8];
  int NextColor[8];
  int i = 0, j = 0, k = 0;
  //Initialize
  for (k = 0; k < 8; ++k)
    NearColor[k]=NearEmpty[k]=EmptyColor[k]=NextEmpty[k]=NextColor[k]=0;

  BOARD(v,x,y)=c;

  //Search Up
  for(i=x-1; i>=0&&BOARD(v,i,y)==c; i--, NearColor[0]++);
  for(;i>=0&&BOARD(v,i,y)==NONE; i--, NearEmpty[0]++);
  for(;i>=0&&BOARD(v,i,y)==c; i--, EmptyColor[0]++);
  for(;i>=0&&BOARD(v,i,y)==NONE; i--, NextEmpty[0]++);
  for(;i>=0&&BOARD(v,i,y)==c; i--, NextColor[0]++);
  
  //Search Right Up
  for(i=x-1,j=y+1; i>=0&&j<15&&BOARD(v,i,j)==c; i--,j++,NearColor[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==NONE; i--,j++, NearEmpty[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==c; i--,j++, EmptyColor[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==NONE; i--,j++, NextEmpty[1]++);
  for(;i>=0&&j<15&&BOARD(v,i,j)==c; i--,j++, NextColor[1]++);

  //Search Right
  for(j=y+1; j<15&&BOARD(v,x,j)==c; j++,NearColor[2]++);
  for(;j<15&&BOARD(v,x,j)==NONE; j++, NearEmpty[2]++);
  for(;j<15&&BOARD(v,x,j)==c; j++, EmptyColor[2]++);
  for(;j<15&&BOARD(v,x,j)==NONE; j++, NextEmpty[2]++);
  for(;j<15&&BOARD(v,x,j)==c; j++, NextColor[2]++);
  
  //Search Right Down
  for(i=x+1,j=y+1; i<15&&j<15&&BOARD(v,i,j)==c; i++,j++,NearColor[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==NONE; i++,j++, NearEmpty[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==c; i++,j++, EmptyColor[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==NONE; i++,j++, NextEmpty[3]++);
  for(;i<15&&j<15&&BOARD(v,i,j)==c; i++,j++, NextColor[3]++);
  
  //Search Down
  for(i=x+1; i<15&&BOARD(v,i,y)==c; i++,NearColor[4]++);
  for(;i<15&&BOARD(v,i,y)==NONE; i++, NearEmpty[4]++);
  for(;i<15&&BOARD(v,i,y)==c; i++, EmptyColor[4]++);
  for(;i<15&&BOARD(v,i,y)==NONE; i++, NextEmpty[4]++);
  for(;i<15&&BOARD(v,i,y)==c; i++, NextColor[4]++);
  
  //Search Left Down
  for(i=x+1,j=y-1; i<15&&j>=0&&BOARD(v,i,j)==c; i++,j--,NearColor[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==NONE; i++,j--,NearEmpty[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==c; i++,j--, EmptyColor[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==NONE; i++,j--, NextEmpty[5]++);
  for(;i<15&&j>=0&&BOARD(v,i,j)==c; i++,j--, NextColor[5]++);
  
  //Search Left
  for(j=y-1;j>=0&&BOARD(v,x,j)==c; j--,NearColor[6]++);
  for(;j>=0&&BOARD(v,x,j)==NONE; j--,NearEmpty[6]++);
  for(;j>=0&&BOARD(v,x,j)==c; j--, EmptyColor[6]++);
  for(;j>=0&&BOARD(v,x,j)==NONE; j--,NextEmpty[6]++);
  for(;j>=0&&BOARD(v,x,j)==c; j--, NextColor[6]++);
  
  //Search Left Up
  for(i=x-1,j=y-1; i>=0&&j>=0&&BOARD(v,i,j)==c; i--,j--,NearColor[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==NONE; i--,j--, NearEmpty[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==c; i--,j--, EmptyColor[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==NONE; i--,j--, NextEmpty[7]++);
  for(;i>=0&&j>=0&&BOARD(v,i,j)==c; i--,j--, NextColor[7]++);
  
  BOARD(v,x,y)=NONE;
  
  int countThree=0, countFour=0;
  
  for(k=0; k<4; k++)
    {
      if(NearColor[k]+NearColor[k+4]>=5)
	return 1;
      else if(NearColor[k]+NearColor[k+4]==3)  // ?0000?
	{
	  int four=0;
	  if(NearEmpty[k]>0)                       // ?+0000?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k],k)==0)
		four=1;
	    }
	  if(NearEmpty[k+4]>0) //?0000+?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k+4],k+4)==0)
		four=1;
	    }
	  if(four) countFour++;
	  
	}
      else if(NearColor[k]+NearColor[k+4]==2) // ?000?
	{
	  if(NearEmpty[k]==1&&EmptyColor[k]==1) //?0+000?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k],k)==0)
		countFour++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyColor[k+4]==1) //?000+0?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k+4],k+4)==0)
		countFour++;
	    }
	  
	  int three = 0;
	  
	  //?++000+?
	  if((NearEmpty[k]>2||(NearEmpty[k]==2&&EmptyColor[k]==0))&&
	     (NearEmpty[k+4]>1||(NearEmpty[k+4]==1&&EmptyColor[k+4]==0)))
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k],k)==0)
		three = 1;
	    }
	  //?+000++?
	  if((NearEmpty[k+4]>2||(NearEmpty[k+4]==2&&EmptyColor[k+4]==0))&&
	     (NearEmpty[k]>1||(NearEmpty[k]==1&&EmptyColor[k]==0)))
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k+4],k+4)==0)
		three = 1;
	    }
	  if(three) countThree++;
	}
      else if(NearColor[k]+NearColor[k+4]==1)  // ?00?
	{
	  if(NearEmpty[k]==1&& EmptyColor[k]==2) //?00+00?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k],k)==0)
		countFour++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyColor[k+4]==2) //?00+00?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k+4],k+4)==0)
		countFour++;
	    }
	  
	  if(NearEmpty[k]==1&&EmptyColor[k]==1&&(NextEmpty[k]>1||
						 (NextEmpty[k]==1&&NextColor[k]==0))&&(NearEmpty[k+4]>1||
										       (NearEmpty[k+4]==1&&EmptyColor[k+4]==0))) //?+0+00+?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k],k)==0)
		countThree++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyColor[k+4]==1&&(NextEmpty[k+4]>1||
						     (NextEmpty[k+4]==1&&NextColor[k+4]==0))&&(NearEmpty[k]>1||
											       (NearEmpty[k]==1&&EmptyColor[k]==0))) //?+00+0+?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k+4],k+4)==0)
		countThree++;
	    }
	  
	}
      else if(NearColor[k]+NearColor[k+4]==0) // ?0?
	{
	  if(NearEmpty[k]==1&&EmptyColor[k]==3) //?000+0?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k],k)==0)
		countFour++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyColor[k+4]==3) //?0+000?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k+4],k+4)==0)
		countFour++;
	    }
	  if(NearEmpty[k]==1&&EmptyColor[k]==2&&(NextEmpty[k]>1||
						 (NextEmpty[k]==1&&NextColor[k]==0))&&(NearEmpty[k+4]>1||
										       (NearEmpty[k+4]==1&&EmptyColor[k+4]==0))) //?+00+0+?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k],k)==0)
		countThree++;
	    }
	  if(NearEmpty[k+4]==1&&EmptyColor[k+4]==2&&(NextEmpty[k+4]>1||
						     (NextEmpty[k+4]==1&&NextColor[k+4]==0))&&(NearEmpty[k]>1||
											       (NearEmpty[k]==1&&EmptyColor[k]==0))) //?+00+0+?
	    {
	      if(c==WHITE||key_forbid(v,x,y,NearColor[k+4],k+4)==0)
		countThree++;
	    }
	}
    }

  int ret = 0;
  if (countFour > 1)
    ret |= FOUR_FOUR;
  if (countThree > 0 && countFour > 0)
    ret |= FOUR_THREE;
  if (countThree > 1)
    ret |= THREE_THREE;
  return ret;
}

#ifdef TEST_43
#include "string.h"
#include "stdio.h"

int main() {
  struct BaseNode node;
  memset(&node, 0, sizeof(node));
  node.data[7][8] = 2;
  node.data[7][10] = 2;
  node.data[8][7] = 2;
  node.data[9][7] = 2;
  node.data[11][7] = 2;
  //node.data[8][7] = 1;
  printf("%d\n", four_three(&node, 7, 7, WHITE));

  return 0;

}

#endif
