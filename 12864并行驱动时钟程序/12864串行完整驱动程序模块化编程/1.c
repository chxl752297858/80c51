/*****12M����stc89c52��Ƭ��*********************/
#include<reg52.h>
#include"seri12864.h"
#include"defi.h"


 main(void)   
    { 
	 init_12864();
	while(1)
	 {
	 dis_page("wo ai ni my darling ����")	;
     dis_line(3,1,"i love you ");
	 }
	
	}