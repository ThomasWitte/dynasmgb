%{
#include "instructions.h"
%}

%token END_OF_BLOCK 0 "end of block"
%token NOP
%token LD16
%token LD
%token INC16
%token INC
%token DEC16
%token DEC
%token RLC
%token ADD16
%token ADD
%token RRC
%token STOP
%token RL
%token JR
%token RR
%token DAA
%token CPL
%token SCF
%token CCF
%token HALT
%token ADC
%token SUB
%token SBC
%token AND
%token XOR
%token OR
%token CP
%token RET
%token POP
%token JP
%token CALL
%token PUSH
%token RST
%token RETI
%token DI
%token EI
%token SLA
%token SRA
%token SWAP
%token SRL
%token BIT
%token RES
%token SET
%token ERROR

%%

toplv : block      {opt_result = $1;}
      ;

block : block inst {$$ = g_list_concat($1, $2);}
      |            {$$ = NULL;}
      ;

inst  : NOP   | LD16  | LD    | INC16 | INC   | DEC16 | DEC   | RLC   | ADD16 | ADD
      | RRC   | STOP  | RL    | JR    | RR    | DAA   | CPL   | SCF   | CCF   | HALT
      | ADC   | SUB   | SBC   | AND   | XOR   | OR    | CP    | RET   | POP   | JP
      | CALL  | PUSH  | RST   | RETI  | DI    | EI    | SLA   | SRA   | SWAP  | SRL
      | BIT   | RES   | SET
      ;

%%

