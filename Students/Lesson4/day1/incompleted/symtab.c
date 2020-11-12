/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  Type * newType = (Type*)malloc(sizeof(Type));
  newType->typeClass = type->typeClass;
  newType->elementType = type->elementType;
  newType->arraySize = type->arraySize;
  return newType;
}

int compareType(Type* type1, Type* type2) {
  if(type1->typeClass == type2->typeClass)
    {
        if(type1->typeClass == TP_ARRAY)
          {
            if(!(type1->arraySize == type2->arraySize && compareType(type1->elementType, type2->elementType)))
              return 0;
          }
        return 1;
    }
  return 0;
}

void freeType(Type* type) {
  if(type->elementType != NULL)
    free(type->elementType);
  free(type);
  type = NULL;
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  ConstantValue * constvalue = (ConstantValue*)malloc(sizeof(ConstantValue));
  constvalue->type = TP_INT;
  constvalue->intValue = i;
  return constvalue;
}

ConstantValue* makeCharConstant(char ch) {
  ConstantValue * constvalue = (ConstantValue*)malloc(sizeof(ConstantValue));
  constvalue->type = TP_CHAR;
  constvalue->charValue = ch;
  return constvalue;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  ConstantValue* value = (ConstantValue*)malloc(sizeof(ConstantValue));
  value->type = v->type;
  if(v->type == TP_INT)
    value->intValue = v->intValue;
  else 
    value->charValue = v->charValue;
  return value;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;
  return program;
}

Object* createConstantObject(char *name) {
  Object *obj = (Object*)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_CONSTANT;
  obj->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  return obj;
}

Object* createTypeObject(char *name) {
  Object *obj = (Object*)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_TYPE;
  obj->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  return obj;
}

Object* createVariableObject(char *name) {
  Object *obj = (Object*)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_VARIABLE;
  obj->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  obj->varAttrs->scope = symtab->currentScope;
  return obj;
}

Object* createFunctionObject(char *name) {
  Object * obj = (Object*)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_FUNCTION;
  obj->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  obj->funcAttrs->paramList = NULL;
  obj->funcAttrs->scope = createScope(obj, symtab->currentScope);
  return obj;
}

Object* createProcedureObject(char *name) {
  Object *obj = (Object*)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PROCEDURE;
  obj->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  obj->procAttrs->paramList = NULL;
  obj->procAttrs->scope = createScope(obj, symtab->currentScope);
  return obj;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  Object *obj = (Object*)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PARAMETER;
  obj->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  obj->paramAttrs->kind = kind;
  obj->paramAttrs->function = owner;
  return obj;
}

void freeObject(Object* obj) {
  switch(obj->kind)
    {
      case OBJ_CONSTANT:
        free(obj->constAttrs->value);
        free(obj->constAttrs);
        break;
      case OBJ_TYPE:
        free(obj->typeAttrs->actualType);
        free(obj->typeAttrs);
        break;
      case OBJ_VARIABLE:
        free(obj->varAttrs->type);
        free(obj->varAttrs);
        break;
      case OBJ_FUNCTION:
        freeReferenceList(obj->funcAttrs->paramList);
        freeType(obj->funcAttrs->returnType);
        freeScope(obj->funcAttrs->scope);
        free(obj->funcAttrs);
        break;
      case OBJ_PROCEDURE:
        freeReferenceList(obj->procAttrs->paramList);
        freeScope(obj->procAttrs->scope);
        free(obj->procAttrs);
        break;
      case OBJ_PROGRAM:
        freeScope(obj->progAttrs->scope);
        free(obj->progAttrs);
        break;
      case OBJ_PARAMETER:
        freeType(obj->paramAttrs->type);
        free(obj->paramAttrs);
        break;
    }
    free(obj);
}

void freeScope(Scope* scope) {
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  ObjectNode* ls = objList;
  while(ls != NULL)
    {
      ObjectNode* node = ls;
      ls = ls->next;
      freeObject(node->object);
      free(node);
    }
}

void freeReferenceList(ObjectNode *objList) {
  ObjectNode* ls = objList;
  while(ls != NULL)
    {
      ObjectNode* node = ls;
      ls = ls->next;
      free(node);
    }
}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  while(objList != NULL)
    {
      if(strcmp(objList->object->name, name) == 0)
        return objList->object;
      objList = objList->next;
    }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
}


