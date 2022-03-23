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
void fifo();//�Ƚ��ȳ��û��㷨
int findpage();//����ҳ��
void RLU();//�������û��㷨
void opt();//����û��㷨

const int PAGEFRAMENUM = 6;//����ҳ��
//�����޸�ҳ�������

int   vpoint;           //ҳ�����ָ��
int   pageframe[PAGEFRAMENUM];    // �����ҳ�򣬼ٶ��̶�Ϊ6�����, ��дʱ��������һ������,
                        // ����ʱ�ı�����ҳ����
int   rpoint;          //ҳ���滻ָ��
int   inpflag;         //ȱҳ��־��0Ϊ��ȱҳ��1Ϊȱҳ
int   pagetime[PAGEFRAMENUM];//��¼ÿ�������ҳ�����һ�α��޸ĵ�ʱ��
int   page_s[PAGEFRAMENUM];//���ҳ��δ���ᱻ���ʵĴ�����

//ҳ����Ϣ�ṹ
struct PageInfo  
{
int       serial[100];  // ģ���������ҳ������ʵ�ʿ�����20����
int       diseffect;    // ȱҳ����
int       total_pf;     // �����ҳ����
int       total_pn;     // ����ҳ�����г���
} pf_info;

////////////////////////////////////////////////////////////////////////
///������ɷ������У��������˼򻯣�ҳ����16ҳ����
///�������ҳ�����
void createps( )
{
	int s,i;

    srand((unsigned int)time(NULL));    //��ʼ���������"����"
	initialize();
	s=(int)((float) rand() / 32767 * 20) + 20; // �������ҳ�����г��ȣ�20~40��ҳ�棩
	pf_info.total_pn = s;

	for(i=0;i<s;i++) //���������������
	{  
		pf_info.serial[i]=(int)((float) rand() / 32767 * 20) ; //20����ͬ��ҳ       
     } 	
}

///////////////////////////////////////////////////////////////////
//��ʼ��������ݽṹ
void initialize()                        
{
	int i;
	inpflag=0;
	pf_info.diseffect =0;
	pf_info.total_pf = PAGEFRAMENUM; //����ʹ������������������Ϊ�̶�
	for(i=0;i<100;i++) // ���ҳ������
	{
       pf_info.serial[i]=-1;
     }
}


////////////////////////////////////////////////////////////////////////
//  ��ʾ��ǰ״̬��ȱҳ���

void displayinfo(void)
{  
   printf("����%3d : �ڴ�<",pf_info.serial[vpoint]); 
   for(int n=0;n<PAGEFRAMENUM;n++){ // ҳ����Ϣ
     if (pageframe[n] >=0) 
		 printf("%3d",pageframe[n]);
	 else
         printf("   ");
   }
   printf(" >");

   if(inpflag==1)
	 {  
		 printf(" ==>ȱҳ "); 
		 printf("ȱҳ��%3.1f",(float)(pf_info.diseffect)*100.00/(vpoint+1)); 
	 }
	printf("\n");
}

////////////////////////////////////////////////////////////////////////
//  ����ҳ���Ƿ����ڴ棬1Ϊ���ڴ棬0Ϊ���ڼ�ȱҳ

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
	  
	   //ҳ�治���ڣ�ȱҳ
	  inpflag=1;
	  return 0;	   
}

////////////////////////////////////////////////////////////////////////
//  FIFOҳ���û��㷨

void fifo(void)
{
  
  int  pstate;  
  inpflag=0;  
  rpoint=0;          // ҳ���滻ָ��
  pf_info.diseffect =0;
  
   for(int n=0; n<PAGEFRAMENUM; n++) {// ���ҳ����Ϣ
         pageframe[n]=-1;
		 pagetime[n]=-1;
		  page_s[n]=1000;
  }
  
  //pstate������־ҳ���Ƿ���ڣ�������Ϊ�㣬����Ϊ�ͷ�����ҳ���еĵڼ������ֵ��±�
  
  for(vpoint=0;vpoint<pf_info.total_pn;vpoint++)  // ִ���㷨
  {    
	    pstate=findpage(pf_info.serial[vpoint]);

		if(pstate==0)// ҳ���������û�ҳ��
		{ 
			pageframe[rpoint]=pf_info.serial[vpoint];				
			rpoint=(rpoint+1) % PAGEFRAMENUM;					
			pf_info.diseffect=pf_info.diseffect+1; // ȱҳ������1				
		}
       //Sleep(10);
	   displayinfo(); // ��ʾ��ǰ״̬
     } 	// �û��㷨ѭ������
	 

	  printf("����ȱҳ��%3.1f\n",(float)(pf_info.diseffect)*100.00/(pf_info.total_pn)); 

   _getch();
   return;
}

////////LRU�㷨/////////////////
void LRU(void)
{
  
  int  pstate;  
  inpflag=0;  
  rpoint=0;          // ҳ���滻ָ��
  pf_info.diseffect =0;
  
  for(int n=0; n<PAGEFRAMENUM; n++) {// ���ҳ����Ϣ
         pageframe[n]=-1;
		 pagetime[n]=-1;
		 page_s[n]=1000;
  }
  
  //pstate
 
   int t=0;//��ǵ�ǰҳ����ҳ�����
  for(vpoint=0;vpoint<pf_info.total_pn;vpoint++)  // ִ���㷨
  {    
	    pstate=findpage(pf_info.serial[vpoint]);//����ҳ���Ƿ���ҳ����
		 
		if(pstate==0)// ҳ���������û�ҳ��
		{
			    pagetime[rpoint]=vpoint;
			    pageframe[rpoint]=pf_info.serial[vpoint];	//ҳ���滻ָ�����˼�������ָ��ָ���ҳ��Ҫ���滻
				if(t<PAGEFRAMENUM){t++;rpoint=t;}
				if(t==PAGEFRAMENUM){
			    //�滻���ƶ��滻ָ�룬ָ����һ��Ҫ���滻��λ��				
                 for(int i=0;i<PAGEFRAMENUM;i++){
                       if(pagetime[rpoint]>pagetime[i]){rpoint=i;}
				 }
		 }
		 pf_info.diseffect=pf_info.diseffect+1; // ȱҳ������1		
		}
		 else{
			pagetime[pstate-1]=vpoint;
		}
		//Sleep(10);
	    displayinfo(); // ��ʾ��ǰ״̬
				
        }
		 printf("����ȱҳ��%3.1f\n",(float)(pf_info.diseffect)*100.00/(pf_info.total_pn)); 
  // �û��㷨ѭ������		
_getch();
   return;
}
   


/////////OPT������û��㷨��///////////   
//����û��㷨�����滻��ҳ����δ������ʹ�û�����òű�ʹ�õ�
  void opt(void)
  {
	  int  pstate;  
      inpflag=0;  
      rpoint=0;          // ҳ���滻ָ��
      pf_info.diseffect =0;
      for(int n=0; n<PAGEFRAMENUM; n++) {// ���ҳ����Ϣ
         pageframe[n]=-1;
		 pagetime[n]=-1;
		 page_s[n]=1000;
      } 
	  for(vpoint=0;vpoint<pf_info.total_pn;vpoint++)  // ִ���㷨
	  {    
	    pstate=findpage(pf_info.serial[vpoint]);
		if(pstate==0)// ҳ���������û�ҳ��
		{
			pageframe[rpoint]=pf_info.serial[vpoint];//��3��ҳ��
		if(rpoint<PAGEFRAMENUM-1){rpoint++;}//ҳ��
			else//ҳ����
			{
				for(int t=0; t<PAGEFRAMENUM; t++) {
					page_s[t]=1000;
				}//����ÿ��ҳ��δ��������ʱ������Ϊ�ܴ�Ĭ��δ����������
				int max=-1;
				rpoint=0;
			    for(int i=0;i<PAGEFRAMENUM;i++){
				     for(int j=vpoint;j<pf_info.total_pn;j++){
					      if(pageframe[i]==pf_info.serial[j]){
							  page_s[i]=j;
							  break;//���ҳ���е�������Ҫ����ҳ�е�һ�������ڸ�ҳ���¼��Ӧ���±꣬�����˴�ѭ��
							}
			            }
					  if(max<page_s[i]){max=page_s[i];rpoint=i;}//����һ��Ҫ���滻ҳ���ָ��ָ���±�������һ��
			}
		}
		      pf_info.diseffect=pf_info.diseffect+1; // ȱҳ������1		  
        }
		displayinfo(); // ��ʾ��ǰ״̬		
  // �û��㷨ѭ������		
  }
   printf("����ȱҳ��%3.1f\n",(float)(pf_info.diseffect)*100.00/(pf_info.total_pn)); 
  getch();
   return;
  }

   

///////////////////////////////////////////////////////////////////
//�����������������
void random(void)
{
	system("cls");
	createps();        // ������ɷ�������
    for(int n=0; n<PAGEFRAMENUM; n++) {// ���ҳ����Ϣ
         pageframe[n]=-1;
  }
	 printf("\n=============ҳ���������=============\n");
	   
      for(int i=0; i<pf_info.total_pn; i++) 
	   {   printf("%4d",pf_info.serial[i]);
		   if ((i+1) % 10 ==0) printf("\n");		   
	   }
     printf("\n======================================\n");
	
}


///////////////////////////////////////////////////////////////////
//������
int main(int argc, char* argv[])
{
  char ch;
  
  system("cls");

  int PAGEFRAMENUM;
 
  while(true)                           
  {
    printf("***********************************\n");
	printf("     0:��ʼ����\n");
    printf("     1: FIFOҳ�����㷨 \n") ;
	printf("     2:LRUҳ�����㷨 \n");
	printf("     3:OPTҳ�����㷨 \n");
    printf("     4: �˳�\n"); 
	
    printf("***********************************\n");
    printf( "Enter your choice (1 or 2 or 3 or 0 or 4): \n");  
    do{   //���������Ϣ����ȷ����������
        ch = (char)_getch() ;
	} while(ch != '1' && ch != '2' && ch !='3'&& ch !='0' &&ch !='4');

    if(ch == '0')
	{
		printf("���������\n");
		random();
	}//������ɷ������в���ӡ
    if(ch == '4') //ѡ��3���˳�
	{ system("cls") ; return 0;}
	if(ch == '1')//ѡ��1��FIFO
	{printf("start [fifo]\n");fifo();} 
	if(ch == '2')
	{printf("start [lru]\n");LRU();}
    if(ch == '3')
	{printf("start [opt]\n");opt();}
  }
  //����
  printf("\nPress Any Key To Continue:");
  _getch() ; 
  return 0;
}
