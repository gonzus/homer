#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

nodeType *con(int value) {
  nodeType *p;

  /* Allocate node */
  if((p = malloc(sizeof(nodeType))) == NULL)
    ast_error("out of memory");

  /* Copy information */
  p->type = typeCon;
  p->con.value = value;

  return p;
}

nodeType *id(int i) {
  nodeType *p;

  /* Allocate node */
  if((p = malloc(sizeof(nodeType))) == NULL)
    ast_error("out of memory");

  /* Copy information */
  p->type = typeId;
  p->id.i = i;

  return p;
}

nodeType *opr(int oper, int nops, ...) {
  va_list ap;
  nodeType *p;
  int i;

  /* Allocate node */
  if((p = malloc(sizeof(nodeType))) == NULL)
    ast_error("out of memory");
  if((p->opr.op = malloc(nops * sizeof(nodeType))) == NULL)
    ast_error("out of memory");

  /* Copy information */
  p->type = typeOpr;
  p->opr.oper = oper;
  p->opr.nops = nops;
  va_start(ap, nops);
  for(i = 0; i < nops; i++)
    p->opr.op[i] = va_arg(ap, nodeType*);
  va_end(ap);

  return p;
}

void freeNode(nodeType *p) {
  int i;

  if(!p) return;

  if(p->type == typeOpr) {
    for(i = 0; i < p->opr.nops; i++)
      freeNode(p->opr.op[i]);
    free(p->opr.op);
  }
  free(p);
}

void ast_error(char *s)
{
  fprintf(stderr, "%s\n", s);
}

