#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <string.h>
#include <time.h>

void initialize();
void createps();
void displayinfo();
void fifo();//先进先出置换算法
int findpage();//查找页面
void RLU();//最近最久置换算法
void opt();//最佳置换算法

const int PAGEFRAMENUM = 6;//六个页框
//这里修改页面物理块

int   vpoint;           //页面访问指针
int   pageframe[PAGEFRAMENUM];    // 分配的页框，假定固定为6物理块, 编写时可以设置一个变量,
                        // 来随时改变分配的页框数
int   rpoint;          //页面替换指针
int   inpflag;         //缺页标志，0为不缺页，1为缺页
int   pagetime[PAGEFRAMENUM];//记录每个分配的页框最后一次被修改的时间
int   page_s[PAGEFRAMENUM];//标记页框未来会被访问的次数；

//页面信息结构
struct PageInfo  
{
int       serial[100];  // 模拟的最大访问页面数，实际控制在20以上
int       diseffect;    // 缺页次数
int       total_pf;     // 分配的页框数
int       total_pn;     // 访问页面序列长度
} pf_info;

////////////////////////////////////////////////////////////////////////
///随机生成访问序列，这里作了简化，页面在16页以内
///这里调整页面个数
void createps( )
{
	int s,i;

    srand((unsigned int)time(NULL));    //初始化随机数的"种子"
	initialize();
	s=(int)((float) rand() / 32767 * 20) + 20; // 随机产生页面序列长度（20~40个页面）
	pf_info.total_pn = s;

	for(i=0;i<s;i++) //产生随机访问序列
	{  
		pf_info.serial[i]=(int)((float) rand() / 32767 * 20) ; //20个不同的页       
     } 	
}

///////////////////////////////////////////////////////////////////
//初始化相关数据结构
void initialize()                        
{
	int i;
	inpflag=0;
	pf_info.diseffect =0;
	pf_info.total_pf = PAGEFRAMENUM; //可以使用随机数产生，这里简化为固定
	for(i=0;i<100;i++) // 清空页面序列
	{
       pf_info.serial[i]=-1;
     }
}


////////////////////////////////////////////////////////////////////////
//  显示当前状态及缺页情况

void displayinfo(void)
{  
   printf("访问%3d : 内存<",pf_info.serial[vpoint]); 
   for(int n=0;n<PAGEFRAMENUM;n++){ // 页框信息
     if (pageframe[n] >=0) 
		 printf("%3d",pageframe[n]);
	 else
         printf("   ");
   }
   printf(" >");

   if(inpflag==1)
	 {  
		 printf(" ==>缺页 "); 
		 printf("缺页率%3.1f",(float)(pf_info.diseffect)*100.00/(vpoint+1)); 
	 }
	printf("\n");
}

////////////////////////////////////////////////////////////////////////
//  查找页面是否在内存，1为在内存，0为不在即缺页

int findpage(int page)
{ 
	int n;

	for(n=0;n<PAGEFRAMENUM;n++) {
      if (pageframe[n]==page )
	  {  
		 inpflag=0 ;	    
		 return n+1;
	  }
	}
	  
	   //页面不存在，缺页
	  inpflag=1;
	  return 0;	   
}

////////////////////////////////////////////////////////////////////////
//  FIFO页面置换算法

void fifo(void)
{
  
  int  pstate;  
  inpflag=0;  
  rpoint=0;          // 页面替换指针
  pf_info.diseffect =0;
  
   for(int n=0; n<PAGEFRAMENUM; n++) {// 清除页框信息
         pageframe[n]=-1;
		 pagetime[n]=-1;
		  page_s[n]=1000;
  }
  
  //pstate用来标志页面是否存在，不存在为零，存在为就返回是页框中的第几个数字的下标
  
  for(vpoint=0;vpoint<pf_info.total_pn;vpoint++)  // 执行算法
  {    
	    pstate=findpage(pf_info.serial[vpoint]);

		if(pstate==0)// 页不存在则置换页面
		{ 
			pageframe[rpoint]=pf_info.serial[vpoint];				
			rpoint=(rpoint+1) % PAGEFRAMENUM;					
			pf_info.diseffect=pf_info.diseffect+1; // 缺页次数加1				
		}
       //Sleep(10);
	   displayinfo(); // 显示当前状态
     } 	// 置换算法循环结束
	 

	  printf("最终缺页率%3.1f\n",(float)(pf_info.diseffect)*100.00/(pf_info.total_pn)); 

   _getch();
   return;
}

////////LRU算法/////////////////
void LRU(void)
{
  
  int  pstate;  
  inpflag=0;  
  rpoint=0;          // 页面替换指针
  pf_info.diseffect =0;
  
  for(int n=0; n<PAGEFRAMENUM; n++) {// 清除页框信息
         pageframe[n]=-1;
		 pagetime[n]=-1;
		 page_s[n]=1000;
  }
  
  //pstate
 
   int t=0;//标记当前页表中页面个数
  for(vpoint=0;vpoint<pf_info.total_pn;vpoint++)  // 执行算法
  {    
	    pstate=findpage(pf_info.serial[vpoint]);//查找页面是否在页表中
		 
		if(pstate==0)// 页不存在则置换页面
		{
			    pagetime[rpoint]=vpoint;
			    pageframe[rpoint]=pf_info.serial[vpoint];	//页面替换指针的意思就是这个指针指向的页面要被替换
				if(t<PAGEFRAMENUM){t++;rpoint=t;}
				if(t==PAGEFRAMENUM){
			    //替换后，移动替换指针，指向下一次要被替换的位置				
                 for(int i=0;i<PAGEFRAMENUM;i++){
                       if(pagetime[rpoint]>pagetime[i]){rpoint=i;}
				 }
		 }
		 pf_info.diseffect=pf_info.diseffect+1; // 缺页次数加1		
		}
		 else{
			pagetime[pstate-1]=vpoint;
		}
		//Sleep(10);
	    displayinfo(); // 显示当前状态
				
        }
		 printf("最终缺页率%3.1f\n",(float)(pf_info.diseffect)*100.00/(pf_info.total_pn)); 
  // 置换算法循环结束		
_getch();
   return;
}
   


/////////OPT（最佳置换算法）///////////   
//最佳置换算法就是替换的页面在未来不被使用或者最久才被使用的
  void opt(void)
  {
	  int  pstate;  
      inpflag=0;  
      rpoint=0;          // 页面替换指针
      pf_info.diseffect =0;
      for(int n=0; n<PAGEFRAMENUM; n++) {// 清除页框信息
         pageframe[n]=-1;
		 pagetime[n]=-1;
		 page_s[n]=1000;
      } 
	  for(vpoint=0;vpoint<pf_info.total_pn;vpoint++)  // 执行算法
	  {    
	    pstate=findpage(pf_info.serial[vpoint]);
		if(pstate==0)// 页不存在则置换页面
		{
			pageframe[rpoint]=pf_info.serial[vpoint];//有3个页框
		if(rpoint<PAGEFRAMENUM-1){rpoint++;}//页框
			else//页框满
			{
				for(int t=0; t<PAGEFRAMENUM; t++) {
					page_s[t]=1000;
				}//先让每个页框未来被访问时间设置为很大，默认未来不被访问
				int max=-1;
				rpoint=0;
			    for(int i=0;i<PAGEFRAMENUM;i++){
				     for(int j=vpoint;j<pf_info.total_pn;j++){
					      if(pageframe[i]==pf_info.serial[j]){
							  page_s[i]=j;
							  break;//如果页框中的数等于要访问页中的一个数，在该页框记录对应的下标，跳出此次循环
							}
			            }
					  if(max<page_s[i]){max=page_s[i];rpoint=i;}//让下一个要被替换页面的指针指向下标最大的那一个
			}
		}
		      pf_info.diseffect=pf_info.diseffect+1; // 缺页次数加1		  
        }
		displayinfo(); // 显示当前状态		
  // 置换算法循环结束		
  }
   printf("最终缺页率%3.1f\n",(float)(pf_info.diseffect)*100.00/(pf_info.total_pn)); 
  getch();
   return;
  }

   

///////////////////////////////////////////////////////////////////
//生成随机序列主函数
void random(void)
{
	system("cls");
	createps();        // 随机生成访问序列
    for(int n=0; n<PAGEFRAMENUM; n++) {// 清除页框信息
         pageframe[n]=-1;
  }
	 printf("\n=============页面访问序列=============\n");
	   
      for(int i=0; i<pf_info.total_pn; i++) 
	   {   printf("%4d",pf_info.serial[i]);
		   if ((i+1) % 10 ==0) printf("\n");		   
	   }
     printf("\n======================================\n");
	
}


///////////////////////////////////////////////////////////////////
//主函数
int main(int argc, char* argv[])
{
  char ch;
  
  system("cls");

  int PAGEFRAMENUM;
 
  while(true)                           
  {
    printf("***********************************\n");
	printf("     0:初始化：\n");
    printf("     1: FIFO页面置算法 \n") ;
	printf("     2:LRU页面置算法 \n");
	printf("     3:OPT页面置算法 \n");
    printf("     4: 退出\n"); 
	
    printf("***********************************\n");
    printf( "Enter your choice (1 or 2 or 3 or 0 or 4): \n");  
    do{   //如果输入信息不正确，继续输入
        ch = (char)_getch() ;
	} while(ch != '1' && ch != '2' && ch !='3'&& ch !='0' &&ch !='4');

    if(ch == '0')
	{
		printf("生成随机数\n");
		random();
	}//随机生成访问序列并打印
    if(ch == '4') //选择3，退出
	{ system("cls") ; return 0;}
	if(ch == '1')//选择1，FIFO
	{printf("start [fifo]\n");fifo();} 
	if(ch == '2')
	{printf("start [lru]\n");LRU();}
    if(ch == '3')
	{printf("start [opt]\n");opt();}
  }
  //结束
  printf("\nPress Any Key To Continue:");
  _getch() ; 
  return 0;
}
