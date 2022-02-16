#include <stdio.h>
#include <stdlib.h>


int end_l1[10000];
int end_l2[1024];
int end_vir[10000];

int tam_vir=0;

int Val_corrigido[10000];


typedef struct _CACHE_
{
     int cache_tag;
     int first_addr;
     int last_addr;
     int n_hit;
     int n_miss;
     char v;
     char d;


}Cache;

Cache Linhas_Cache[256];


int cache_write( int end_fisi, int rw)
{
    
        #define data_index 4
        #define offset_index 8
        #define offset_mask 0xf
        #define set_mask 0xff
   


    int set_index = (end_fisi >> data_index) & set_mask;
    int tag=(end_fisi>> (offset_index + data_index));


    if(tag == Linhas_Cache[set_index].cache_tag)
    {
        Linhas_Cache[set_index].n_hit++;
    }
    else
    {
        Linhas_Cache[set_index].cache_tag = tag;
        Linhas_Cache[set_index].first_addr = end_fisi & ~(offset_mask);
        Linhas_Cache[set_index].last_addr = end_fisi | (offset_mask);
        Linhas_Cache[set_index].n_miss++;
        Linhas_Cache[set_index].v=1;
        Linhas_Cache[set_index].d=0;
    }

  /*  if(rw)
    {
        Linhas_Cache[set_index].d=1;
    }*/

    return 0;

}




void ArquivosP1()
{
    FILE *ArquivosL1 = fopen("L1.txt", "r+");
    if(ArquivosL1 == NULL)
    {
        printf("Erro ao abrir Arquivos de L1 \n");
    }
    int i=0;
   
    while(!feof(ArquivosL1))
    {
        fscanf(ArquivosL1,"%x",&end_l1[i]);
     
        i++;
    }
    

}

void ArquivosP2()
{
    FILE *ArquivosL2 = fopen("L2.txt", "r+");
    if(ArquivosL2 == NULL)
    {
        printf("Erro ao abrir Arquivos de L2 \n");
    }
    
    int i=0;
   
    while(!feof(ArquivosL2))
    {
        fscanf(ArquivosL2,"%x",&end_l2[i]);
    
        i++;
    }

}

void ArquivosP3()
{
    FILE *ArquivosL3 = fopen("end_Q1.txt", "r+");
    if(ArquivosL3 == NULL)
    {
        printf("Erro ao abrir Arquivos de L3 \n");
    }
    
    int i=0;
   
    while(!feof(ArquivosL3))
    {
        fscanf(ArquivosL3,"%x",&end_vir[i]);
      
        i++;
    }
   tam_vir=i;
   
}



int end_l2_course(int ende)
{
    int base=(ende&0x000FF000)>>12;
    int offset=(ende&0x00000FFF);
    int num=0;
    for(int l=0 ; l<256 ; l++)
    {
        if(base==l)
        {
            num=((end_l2[l]&0xFFFFF000)|offset);
            break;
        }

       
    }
    return num;
}





int end_l2_small(int ende)
{
                 
    int base=(ende&0x000FFC00)>>10;
    int offset=(ende&0x000003FF);
    int num=0;
    for(int l=0 ; l<1024 ; l++)
    {
        if(base==l)
        {
            num=((end_l2[l]&0xFFFFF000)|offset);
            break;
        }

       
    }

    return num;
}




void end_fisico()
{
    int m=0;
    for(int l=0; l < tam_vir ; l++)
    {
        int x=end_vir[l];
       

        
        
        int base=(end_vir[l]&0xFFF00000)>>20; 
        int offset=(end_vir[l]&0x000FFFFF);

   

        for(int j=0;j<4096;j++) // 4096 pq a l1 so tem isso de tamanho no nosso exemplo
        {
            if(base==j)
            {
               if((end_l1[j]&0x00000003)==0x00000003) 
               {
                   
                  Val_corrigido[m]=end_l2_small(x);
                  m++;
                  break;
               }
               
               else if((end_l1[j]&0x00000002)==0x00000002)
                {
                    Val_corrigido[m]=((end_l1[j]&0xFFF00000)|offset);
                   
                    m++;
                    break;
                    
                  
                }
                else if((end_l1[j]&0x00000001)==0x00000001)
                {
                   Val_corrigido[m]=end_l2_course(x);
                  
                   m++;
                   break;
                }
               
            }
        }
                  
                    
    }
}






int main()
{

  
    ArquivosP1();
  

    ArquivosP2();
 

    ArquivosP3();


    end_fisico();

    // inicializando nossa cache

    for(int z=0 ; z < 256 ; z++)
    {
        Linhas_Cache[z].cache_tag=0;
        Linhas_Cache[z].first_addr=0;
        Linhas_Cache[z].last_addr=0;
        Linhas_Cache[z].n_hit=0;
        Linhas_Cache[z].n_miss=0;
        Linhas_Cache[z].v=0;
        Linhas_Cache[z].d=0;
    }

    //carregando cache com endereços fisicos

    for (int z=0 ; z<tam_vir ; z++)
    {
       cache_write(Val_corrigido[z],0);
    }



    for(int z=0; z<256; z++)
    {
      

        printf("Linha %d: [0x%08x][%d][%d][0x%08x - 0x%08x] H(%d) M(%d)\n",z,Linhas_Cache[z].cache_tag,
       Linhas_Cache[z].v, Linhas_Cache[z].d,Linhas_Cache[z].first_addr, Linhas_Cache[z].last_addr,Linhas_Cache[z].n_hit,Linhas_Cache[z].n_miss);

    }




  

    
   
    
    
}


