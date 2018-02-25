#include <stdio.h>
#include "ast.h"
#include "parser.h"
#include "interp.h"

int sym[26];

int run(nodeType *p) {
  if(!p)
    return 0;

  switch(p->type) {
    case typeCon:
      return p->con.value;

    case typeId:
      return sym[p->id.i];

    case typeOpr:
      switch(p->opr.oper) {
        case WHILE:
          while(run(p->opr.op[0]))
            run(p->opr.op[1]);
          return 0;

        case IF:
          if(run(p->opr.op[0]))
            run(p->opr.op[1]);
          else if(p->opr.nops> 2)
            run(p->opr.op[2]);
          return 0;

        case PRINT:
          printf("%d\n", run(p->opr.op[0]));
          return 0;

        case ';':
          run(p->opr.op[0]);
          return run(p->opr.op[1]);

        case '=':
          return sym[p->opr.op[0]->id.i] = run(p->opr.op[1]);

        case UMINUS:
          return -run(p->opr.op[0]);

        case '+': return run(p->opr.op[0]) +  run(p->opr.op[1]);
        case '-': return run(p->opr.op[0]) -  run(p->opr.op[1]);
        case '*': return run(p->opr.op[0]) *  run(p->opr.op[1]);
        case '/': return run(p->opr.op[0]) /  run(p->opr.op[1]);
        case LT:  return run(p->opr.op[0]) <  run(p->opr.op[1]);
        case GT:  return run(p->opr.op[0]) >  run(p->opr.op[1]);
        case GE:  return run(p->opr.op[0]) >= run(p->opr.op[1]);
        case LE:  return run(p->opr.op[0]) <= run(p->opr.op[1]);
        case EQ:  return run(p->opr.op[0]) == run(p->opr.op[1]);
        case NE:  return run(p->opr.op[0]) != run(p->opr.op[1]);
      }
  }

  return 0;
}

