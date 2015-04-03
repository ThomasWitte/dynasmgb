%{
#include "instructions.h"
%}

%token OP        1

%%

block : inst block
      |
      ;
      
inst : OP
     ;

%%

