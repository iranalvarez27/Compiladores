#include"imp_type_checker.hh"
#include <typeinfo>

ImpTypeChecker::ImpTypeChecker():inttype(), booltype(), voidtype(), longtype() {
  inttype.set_basic_type("int");
  booltype.set_basic_type("bool");
  voidtype.set_basic_type("void");
  longtype.set_basic_type("long");

  list<string> noparams;
  maintype.set_fun_type(noparams,"int");
  has_main = false;
}

void ImpTypeChecker::sp_incr(int n) {
  // sp++;  // TODO
  sp += n;
  if (sp > max_sp) max_sp = sp;
}

void ImpTypeChecker::sp_decr(int n) {
  // sp--;  // TODO
  sp -= n;
  if (sp < 0) {
    cout << "stack less than 0" << endl;
    exit(0);
  }
}

void ImpTypeChecker::typecheck(Program* p) {
    env.clear();
    p->accept(this);
    if (!has_main) {
        cout << "Error: Programa no tiene main" << endl;
        exit(0);
    }
    return;
}
void ImpTypeChecker::visit(Program* p) {
  env.add_level();
  ftable.add_level();
  p->functions->accept(this);

  if (!has_main) {
      cout << "Error: Programa no tiene main" << endl;
      exit(0);
  }

  env.remove_level();
  ftable.remove_level();

  return;
}

void ImpTypeChecker::visit(VarDecList* vdl) {
    for (VarDec* vd : vdl->vardecs) {
        vd->accept(this);
    }
    return;
}

void ImpTypeChecker::visit(ParamList* p) {
  list<Param*>::iterator it;
  for (it = p->params.begin(); it != p->params.end(); ++it) {
    ImpType type;
    type.set_basic_type((*it)->type);
    env.add_var((*it)->name, type);
  }
  return;
}

void ImpTypeChecker::visit(Param* p) {
  ImpType type;
  type.set_basic_type(p->type);
  env.add_var(p->name, type);
  return;
}

void ImpTypeChecker::visit(Operaciones* s) {
    if (!env.check(s->id)) {
        cout << "Variable " << s->id << " undefined" << endl;
        exit(0);
    }

    ImpType varType = env.lookup(s->id);  
    ImpType rhsType = s->rhs->accept(this); 

    if (!rhsType.match(varType)) {
        cout << "Tipo incorrecto en Assign a " << s->id << endl;
        exit(0);
    }

    sp_decr(1); 
    return;
}

void ImpTypeChecker::visit(FuncList* fl) {
    for (FuncDecl* func : fl->functions) {
        add_fundec(func);
    }
    for (FuncDecl* func : fl->functions) {
        sp = max_sp = 0;
        dir = max_dir = 0;
        func->accept(this);
        FEntry fentry;
        fentry.fname = func->name;
        fentry.ftype = env.lookup(func->name);
        fentry.max_stack = max_sp;
        fentry.mem_locals = max_dir;
        ftable.add_var(func->name, fentry);
    }
    return;
}


void ImpTypeChecker::visit(VarDec* vd) {
    ImpType type;
    type.set_basic_type(vd->type);
    if (type.ttype == ImpType::NOTYPE || type.ttype == ImpType::VOID) {
        cout << "Error: Tipo invalido para " << vd->varName << endl;
        exit(0);
    }
    env.add_var(vd->varName, type);
    dir++;
    if (dir > max_dir) max_dir = dir;
    return;
}



void ImpTypeChecker::add_fundec(FuncDecl* fd) {
    ImpType funtype;
    list<string> paramNames;

    for (Param* param : fd->params->params) {
        paramNames.push_back(param->type);
    }

    if (!funtype.set_fun_type(paramNames, fd->type)) {
        cout << "Error: Invalid function declaration for '" << fd->name << "'." << endl;
        exit(0);
    }

    if (fd->name == "main") {
        if (!funtype.match(maintype)) {
            cout << "Error: Invalid return type for 'main'." << endl;
            exit(0);
        }
        has_main = true;
    }


    env.add_var(fd->name, funtype);
}


void ImpTypeChecker::visit(FuncDecl* fd) {
    env.add_level();

    ImpType funtype = env.lookup(fd->name);
    ImpType rtype;
    rtype.set_basic_type(funtype.types.back());

    int i = 0;
    for (Param* param : fd->params->params) {
        ImpType ptype;
        ptype.set_basic_type(funtype.types[i++]);
        env.add_var(param->name, ptype);
    }

    fd->varDecs->accept(this);

    env.add_var("return", rtype);

    for (Stmt* stmt : fd->stmts->statements) {
      if (stmt == nullptr) {
        continue;
      }

      stmt->accept(this);
    }

    env.remove_level();
}

void ImpTypeChecker::visit(StatementList* s) {
  list<Stmt*>::iterator it;
  for (it = s->statements.begin(); it != s->statements.end(); ++it) {
    if (*it == nullptr) {
      continue;
    }


    (*it)->accept(this);
  }
  return;
}


void ImpTypeChecker::visit(Assignment* a) {
    if (!env.check(a->id)) {
        // TODO
        cout << "Declarando variable local: " << a->id << endl;
        ImpType varType = inttype;
        env.add_var(a->id, varType);
    }

    ImpType rhsType = a->rhs->accept(this);

    ImpType varType = env.lookup(a->id);
    if (!rhsType.match(varType)) {
        cout << "Error: Tipo incorrecto en la asignación de " << a->id << endl;
        exit(0);
    }

    sp_decr(1);
    return;
}

void ImpTypeChecker::visit(PrintStmt* s) {
  s->exp->accept(this);
  sp_decr(1);
  return;
}

void ImpTypeChecker::visit(IfStmt* s) {
  if (!s->condition->accept(this).match(booltype)) {
    cout << "Expresion conditional en IF debe de ser bool" << endl;
    exit(0);
  }
  sp_decr(1);
  s->thenBody->accept(this);
  if (s->elseBody != NULL)
    s->elseBody->accept(this);

  return;
}

void ImpTypeChecker::visit(ReturnStatement* rs) {
    ImpType rtype = env.lookup("return");
    ImpType etype = rs->exp ? rs->exp->accept(this) : voidtype;
    if (!rtype.match(etype)) {
        cout << "Error: Return type mismatch in function." << endl;
        exit(0);
    }
    sp_decr(1);
    return;
}


void ImpTypeChecker::visit(ForStmt* s) {
    env.add_level();

    if (s->init) {
        s->init->accept(this);
    }

    if (s->condition) {
        if (!s->condition->accept(this).match(booltype)) {
            cout << "Error: La condición del 'for' debe ser de tipo bool." << endl;
            exit(0);
        }
        sp_decr(1);
    }

    if (s->step) {
        s->step->accept(this);
    }

    if (s->body) {
        s->body->accept(this);
    }

    env.remove_level();
    return;
}

void ImpTypeChecker::visit(WhileStmt* s) {
    if (!s->condition->accept(this).match(booltype)) {
        cout << "Error: La condición del 'while' debe ser de tipo bool." << endl;
        exit(0);
    }
    sp_decr(1);
    s->body->accept(this);
    return;
}

void ImpTypeChecker::visit(Type* t) {
  ImpType type;
  type.set_basic_type(t->type);
  return;
}

ImpType ImpTypeChecker::visit(BinaryExp* e) {
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);
  if (!t1.match(inttype) || !t2.match(inttype)) {
    cout << "Tipos en BinExp deben de ser int" << endl;
    exit(0);
  }
  ImpType result;
  switch(e->op) {
  case PLUS_OP:
  case MINUS_OP:
  case MUL_OP:
  case DIV_OP:
    result = inttype;
    break;
  }
  sp_decr(1);
  return result;
}

ImpType ImpTypeChecker::visit(NumberExp* e) {
  sp_incr(1);
  return inttype;
}

ImpType ImpTypeChecker::visit(RelationalExp* e) {
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);
  if (!t1.match(inttype) || !t2.match(inttype)) {
    cout << "Tipos en Rel deben de ser op" << endl;
    exit(0);
  }
  ImpType result;
  switch(e->op) {
  case GT_OP:
  case GE_OP:
  case LT_OP:
  case LE_OP:
  case EQ_OP:
  case NE_OP:
    result = booltype;
    break;
  }
  sp_decr(1);

  return result;
}

ImpType ImpTypeChecker::visit(IdentifierExp* e) {
  sp_incr(1);
  if (env.check(e->name))
    return env.lookup(e->name);
  else {
    cout << "Variable indefinida: " << e->name << endl;
    exit(0);
  }
}

ImpType ImpTypeChecker::visit(FuncCallExp* e) {
  return process_fcall(e);
}


ImpType ImpTypeChecker::process_fcall(FuncCallExp* e) {
    if (!env.check(e->name)) {
        cout << "(Function call): " << e->name << " no existe" << endl;
        exit(0);
    }

    ImpType funtype = env.lookup(e->name);
    if (funtype.ttype != ImpType::FUN) {
        cout << "(Function call): " << e->name << " no es una funcion" << endl;
        exit(0);
    }

    int num_fun_args = funtype.types.size() - 1;
    int num_fcall_args = e->args->args.size();
    if (num_fun_args != num_fcall_args) {
        cout << "(Function call) Numero de argumentos no corresponde a declaracion de: " << e->name << endl;
        exit(0);
    }

    ImpType rtype;
    rtype.set_basic_type(funtype.types[num_fun_args]);
    if (!rtype.match(voidtype)) {
        sp_incr(1);
    }

    ImpType argtype;
    auto it = e->args->args.begin();
    for (int i = 0; it != e->args->args.end(); ++it, ++i) {
        argtype = (*it)->accept(this);
        if (argtype.ttype != funtype.types[i]) {
            cout << "(Function call) Tipo de argumento no corresponde a tipo de parametro en fcall de: " << e->name << endl;
            exit(0);
        }
    }

    sp_decr(num_fcall_args);
    return rtype;
}

void ImpTypeChecker::visit(ArgList* a) {
    list<CExp*>::iterator it;
    for (it = a->args.begin(); it != a->args.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpTypeChecker::visit(StepCondition* s) {
    if (!env.check(s->id)) {
        cout << "Error: Variable '" << s->id << "' no declarada antes de ser usada en la condicion de incremento/decremento." << endl;
        exit(0);
    }

    ImpType varType = env.lookup(s->id);

    if (varType.ttype != ImpType::INT) {
        cout << "Error: Variable '" << s->id << "' debe ser de tipo entero para ser usada en la condicion de incremento/decremento." << endl;
        exit(0);
    }

    return;
}

//Si o si debe ser de tipo void
void ImpTypeChecker::visit(FuncCallStmt* s) {
    if (!env.check(s->funcCall->name)) {
        cout << "(Function call): " << s->funcCall->name << " no existe" << endl;
        exit(0);
    }

    ImpType funtype = env.lookup(s->funcCall->name);
    if (funtype.ttype != ImpType::FUN) {
        cout << "(Function call): " << s->funcCall->name << " no es una funcion" << endl;
        exit(0);
    }

    int num_fun_args = funtype.types.size() - 1;
    int num_fcall_args = s->funcCall->args->args.size();
    ImpType rtype;
    rtype.set_basic_type(funtype.types[num_fun_args]);

    if (!rtype.match(voidtype)) {
        cout << "(Function call): " << s->funcCall->name << " no es de tipo void" << endl;
        exit(0);
    }

    if (num_fun_args != num_fcall_args) {
        cout << "(Function call) Numero de argumentos no corresponde a declaracion de: " << s->funcCall->name << endl;
        exit(0);
    }

    ImpType argtype;
    auto it = s->funcCall->args->args.begin();
    for (int i = 0; it != s->funcCall->args->args.end(); ++it, ++i) {
        argtype = (*it)->accept(this);
        if (argtype.ttype != funtype.types[i]) {
            cout << "(Function call) Tipo de argumento no corresponde a tipo de parametro en fcall de: " << s->funcCall->name << endl;
            exit(0);
        }
    }

    sp_decr(num_fcall_args);
    return;
}
