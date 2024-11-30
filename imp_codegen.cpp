#include "imp_codegen.hh"
#include <typeinfo>

ImpCodeGen::ImpCodeGen(ImpTypeChecker* a):analysis(a) {

}

void ImpCodeGen::codegen(string label, string instr) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << endl;
}

void ImpCodeGen::codegen(string label, string instr, int arg) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << arg << endl;
}

void ImpCodeGen::codegen(string label, string instr, string jmplabel) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << jmplabel << endl;
}

string ImpCodeGen::next_label() {
  string l = "L";
  string n = to_string(current_label++);
  l.append(n);
  return l;
}

string ImpCodeGen::get_flabel(string fname) {
  string l = "L";
  l.append(fname);
  return l;
}

void ImpCodeGen::codegen(Program* p, string outfname) {
  nolabel = "";
  current_label = 0;

  p->accept(this);
  ofstream outfile;
  outfile.open(outfname);
  outfile << code.str();
  outfile.close();

  return;
}

void ImpCodeGen::visit(Type* t) {
  return ;
}

void ImpCodeGen::visit(Program* p) {
  current_dir = 0;
  direcciones.add_level();
  process_global = true;

  for (FuncDecl* func : p->functions->functions) {
      if (func->varDecs != nullptr) {
          func->varDecs->accept(this);
      }
  }

  mem_globals = current_dir;

  codegen("start", "skip");
  codegen(nolabel, "enter", mem_globals);
  codegen(nolabel, "alloc", mem_globals);
  codegen(nolabel, "mark");
  codegen(nolabel, "pusha", get_flabel("main"));
  codegen(nolabel, "call");
  codegen(nolabel, "halt");

  process_global = false;

  p->functions->accept(this);

  direcciones.remove_level();
  return;
}


void ImpCodeGen::visit(FuncList * b) {
  list<FuncDecl*>::iterator it;
  for (it = b->functions.begin(); it != b->functions.end(); ++it) {
    if (*it == nullptr) {
      cout << "Error: Funcion nula" << endl;
      continue;
    }

    (*it)->accept(this);
  }
  return ;
}

void ImpCodeGen::visit(VarDecList* s) {
  list<VarDec*>::iterator it;
  for (it = s->vardecs.begin(); it != s->vardecs.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}

void ImpCodeGen::visit(VarDec* vd) {
    VarEntry ventry;
    ventry.is_global = process_global;
    ventry.dir = current_dir++;
    direcciones.add_var(vd->varName, ventry);
}

void ImpCodeGen::visit(FuncDecl* fd) {
  direcciones.add_level();
  current_dir = 0;
  num_params = 0;

  fd->params->accept(this);
  fd->varDecs->accept(this);

  codegen(get_flabel(fd->name),"skip");

  fd->stmts->accept(this);

  direcciones.remove_level();
  return ;
}

void ImpCodeGen::visit(ParamList* p) {
  list<Param*>::iterator it;
  for (it = p->params.begin(); it != p->params.end(); ++it) {
    (*it)->accept(this);
  }
  return ;
}

void ImpCodeGen::visit(Param* p) {
  VarEntry ventry;
  ventry.is_global = false;
  ventry.dir = current_dir++;
  direcciones.add_var(p->type, ventry);
  num_params++;
  return ;
}

void ImpCodeGen::visit(StatementList* s) {
  list<Stmt*>::iterator it;
  for (it = s->statements.begin(); it != s->statements.end(); ++it) {
    if (*it == nullptr) {
      continue;
    }

    (*it)->accept(this);
  }
  return ;
}

void ImpCodeGen::visit(Assignment* s) {
  VarEntry ventry = direcciones.lookup(s->id);
  s->rhs->accept(this);
  if (ventry.is_global)
    codegen(nolabel,"store",ventry.dir);
  else
    codegen(nolabel,"storer",ventry.dir);
  return ;
}

void ImpCodeGen::visit(PrintStmt* s) {
  s->exp->accept(this);
  codegen(nolabel,"printf");
  return ;
}

void ImpCodeGen::visit(IfStmt* s) {
  string l1 = next_label();
  string l2 = next_label();
  
  s->condition->accept(this);
  codegen(nolabel,"jmpz",l1);
  s->thenBody->accept(this);
  codegen(nolabel,"goto",l2);
  codegen(l1,"skip");
  if (s->elseBody!=NULL) {
    s->elseBody->accept(this);
  }
  codegen(l2,"skip");
 
  return ;
}

void ImpCodeGen::visit(ReturnStatement* s) {
  if (s->exp != NULL) {
    s->exp->accept(this);
  }
  codegen(nolabel,"ret");
  return ;
}

void ImpCodeGen::visit(ForStmt* s) {
  string l1 = next_label();
  string l2 = next_label();
  string l3 = next_label();
  s->init->accept(this);
  codegen(l1,"skip");
  s->condition->accept(this);
  codegen(nolabel,"jmpz",l2);
  s->body->accept(this);
  s->step->accept(this);
  codegen(nolabel,"goto",l1);
  codegen(l2,"skip");
  codegen(l3,"skip");
  return ;
}

void ImpCodeGen::visit(WhileStmt* s) {
  string l1 = next_label();
  string l2 = next_label();
  codegen(l1,"skip");
  s->condition->accept(this);
  codegen(nolabel,"jmpz",l2);
  s->body->accept(this);
  codegen(nolabel,"goto",l1);
  codegen(l2,"skip");
  return ;
}

void ImpCodeGen::visit(StepCondition* s) {
  VarEntry ventry = direcciones.lookup(s->id);
  codegen(nolabel,"push",1);
  if (ventry.is_global)
    codegen(nolabel,"load",ventry.dir);
  else
    codegen(nolabel,"loadr",ventry.dir);
  codegen(nolabel,"add");
  if (ventry.is_global)
    codegen(nolabel,"store",ventry.dir);
  else
    codegen(nolabel,"storer",ventry.dir);
  return ;
}

void ImpCodeGen::visit(ArgList* a) {
  list<CExp*>::iterator it;
  for (it = a->args.begin(); it != a->args.end(); ++it) {
    (*it)->accept(this);
  }
  return ;
}


void ImpCodeGen::visit(Operaciones* s){
  VarEntry ventry = direcciones.lookup(s->id);
  s->rhs->accept(this);
  if (ventry.is_global)
    codegen(nolabel,"load",ventry.dir);
  else
    codegen(nolabel,"loadr",ventry.dir);
  codegen(nolabel,"add");
  if (ventry.is_global)
    codegen(nolabel,"store",ventry.dir);
  else
    codegen(nolabel,"storer",ventry.dir);
  return ;
}

int ImpCodeGen::visit(BinaryExp* e) {
  e->left->accept(this);
  e->right->accept(this);
  string op = "";
  switch(e->op) {
  case PLUS_OP: op =  "add"; break;
  case MINUS_OP: op = "sub"; break;
  case MUL_OP:  op = "mul"; break;
  case DIV_OP:  op = "div"; break;
  default: break;
  }
  codegen(nolabel, op);
  return 0;
}

int ImpCodeGen::visit(NumberExp* e) {
  codegen(nolabel,"push ",e->value);
  return 0;
}


int ImpCodeGen::visit(IdentifierExp* e) {
  VarEntry ventry = direcciones.lookup(e->name);
  if (ventry.is_global)
    codegen(nolabel,"load",ventry.dir);
  else
    codegen(nolabel,"loadr",ventry.dir);
  return 0;
}

int ImpCodeGen::visit(RelationalExp* e) {
  e->left->accept(this);
  e->right->accept(this);
  string op = "";
  switch(e->op) {
  case LT_OP: op =  "lt"; break;
  case LE_OP: op = "le"; break;
  case EQ_OP:  op = "eq"; break;
  case GT_OP:  op = "gt"; break;
  case GE_OP:  op = "ge"; break;
  case NE_OP:  op = "ne"; break;
  default: break;
  }
  codegen(nolabel, op);
  return 0;

}

int ImpCodeGen::visit(FuncCallExp* e) {
  list<CExp*>::iterator it;
  for (it = e->args->args.begin(); it != e->args->args.end(); ++it) {
    (*it)->accept(this);
  }
  codegen(nolabel,"mark");
  codegen(nolabel,"pusha",get_flabel(e->name));
  codegen(nolabel,"call");
  return 0;
}

void ImpCodeGen::visit(FuncCallStmt* s) {
  s->funcCall->accept(this);
  return ;
}
