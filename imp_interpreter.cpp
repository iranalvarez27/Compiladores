#include "imp_interpreter.hh"

ImpValue BinaryExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}


ImpValue NumberExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue IdentifierExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue FuncCallExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}


ImpValue RelationalExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpType BinaryExp::accept(TypeVisitor* v) {
    return v->visit(this);
}

ImpType RelationalExp::accept(TypeVisitor* v) {
    return v->visit(this);
}

ImpType NumberExp::accept(TypeVisitor* v) {
    return v->visit(this);
}

ImpType IdentifierExp::accept(TypeVisitor* v) {
    return v->visit(this);
}

ImpType FuncCallExp::accept(TypeVisitor* v) {
    return v->visit(this);
}

void Program::accept(TypeVisitor* v) {
    return v->visit(this);
}

void FuncList::accept(TypeVisitor* v) {
    return v->visit(this);
}

void VarDecList::accept(TypeVisitor* v) {
    return v->visit(this);
}

void Param::accept(TypeVisitor* v) {
    return v->visit(this);
}

void StatementList::accept(TypeVisitor* v) {
    return v->visit(this);
}

void Assignment::accept(TypeVisitor* v) {
    return v->visit(this);
}

void PrintStmt::accept(TypeVisitor* v) {
    return v->visit(this);
}

void ParamList::accept(TypeVisitor* v) {
    return v->visit(this);
}

void IfStmt::accept(TypeVisitor* v) {
    return v->visit(this);
}

void ReturnStatement::accept(TypeVisitor* v) {
    return v->visit(this);
}

void ForStmt::accept(TypeVisitor* v) {
    return v->visit(this);
}

void WhileStmt::accept(TypeVisitor* v) {
    return v->visit(this);
}

void FuncCallStmt::accept(TypeVisitor* v) {
    return v->visit(this);
}

void Operaciones::accept(TypeVisitor* v) {
    return v->visit(this);
}

void Type::accept(TypeVisitor* v) {
    return v->visit(this);
}

void ArgList::accept(TypeVisitor* v) {
    return v->visit(this);
}

void VarDec::accept(TypeVisitor* v) {
    return v->visit(this);
}

void FuncDecl::accept(TypeVisitor* v) {
    return v->visit(this);
}

void StepCondition::accept(TypeVisitor* v) {
    return v->visit(this);
}

void Assignment::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void PrintStmt::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void IfStmt::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void StepCondition::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ReturnStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ForStmt::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void WhileStmt::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void StatementList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void VarDec::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void FuncDecl::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void VarDecList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void Param::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ParamList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void FuncList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}


void Program::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ImpInterpreter::interpret(Program* p) {
    env.clear();
    p->accept(this);
    return;
}

void FuncCallStmt::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void Operaciones::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void Type::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ArgList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ImpInterpreter::visit(Program* p) {
    env.add_level();
    fdecs.add_level();
    p->functions->accept(this);
    if (!fdecs.check("main")) {
        cout << "Error: No se encontro funcion main" << endl;
        exit(0);
    }
    FuncDecl* main_dec = fdecs.lookup("main");
    retcall = false;
    main_dec->stmts->accept(this);
    if (main_dec->type!= "void" && !retcall) {
        cout << "Error: Funcion main no ejecuto RETURN" << endl;
        exit(0);
    }

}

void ImpInterpreter::visit(FuncList* b) {
    env.add_level();
    list<FuncDecl*>::iterator it;
    for (it = b->functions.begin(); it != b->functions.end(); ++it) {
        (*it)->accept(this);
    }
    env.remove_level();
    return;
}

void ImpInterpreter::visit(VarDecList* decs) {
    list<VarDec*>::iterator it;
    for (it = decs->vardecs.begin(); it != decs->vardecs.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpInterpreter::visit(ParamList* pl) {
    list<Param*>::iterator it;
    for (it = pl->params.begin(); it != pl->params.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpInterpreter::visit(Param* p) {
    ImpValue v;
    ImpVType tt = ImpValue::get_basic_type(p->type);
    if (tt == NOTYPE) {
        cout << "Tipo invalido: " << p->type << endl;
        exit(0);
    }
    v.set_default_value(tt);
    env.add_var(p->name, v);
    return;
}


void ImpInterpreter::visit(ArgList* al) {
    list<CExp*>::iterator it;
    for (it = al->args.begin(); it != al->args.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpInterpreter::visit(Type* t) {
    if (t->type.empty()) {
        cout << "Error: Tipo vacío no es válido." << endl;
        exit(0);
    }

    return;
}


void ImpInterpreter::visit(Operaciones* o) {
    ImpValue rhsValue = o->rhs->accept(this);

    if (!env.check(o->id)) {
        exit(0);
    }

    env.update(o->id, rhsValue);

    return;
}

void ImpInterpreter::visit(VarDec* vd) {
    list<string>::iterator it;
    ImpValue v;
    ImpVType tt = ImpValue::get_basic_type(vd->type);
    if (tt == NOTYPE) {
        cout << "Tipo invalido: " << vd->type << endl;
        exit(0);
    }
    v.set_default_value(tt);
    env.add_var(vd->varName, v);

    return;
}

void ImpInterpreter::visit(FuncDecl* fd) {
    fdecs.add_var(fd->name, fd);
    return;
}

void ImpInterpreter::visit(StatementList* s) {
    list<Stmt*>::iterator it;
    for (it = s->statements.begin(); it != s->statements.end(); ++it) {
        (*it)->accept(this);
        if (retcall)
            break;
    }
    return;
}

void ImpInterpreter::visit(Assignment* s) {
    ImpValue v = s->rhs->accept(this);
    if (!env.check(s->id)) {
        cout << "Variable " << s->id << " undefined" << endl;
        exit(0);
    }
    ImpValue lhs = env.lookup(s->id);
    if (lhs.type != v.type) {
        cout << "Type Error en Assign: Tipos de variable " << s->id;
        exit(0);
    }
    env.update(s->id, v);
    return;
}

void ImpInterpreter::visit(PrintStmt* s) {
    ImpValue v = s->exp->accept(this);
    cout << v << endl;
    return;
}

void ImpInterpreter::visit(IfStmt* s) {
    ImpValue v = s->condition->accept(this);
    if (v.type != TBOOL) {
        cout << "Type error en If: esperaba bool en condicional" << endl;
        exit(0);
    }
    if(v.bool_value){
        s->thenBody->accept(this);
    }
    else{
        s->elseBody->accept(this);
    }
    return;
}

void ImpInterpreter::visit(ReturnStatement* s) {
    if (s->exp != NULL)
         retval= s->exp->accept(this);
    retcall = true;
    return;
}

void ImpInterpreter::visit(StepCondition* s) {
    ImpValue v = env.lookup(s->id);
    if (v.type != TINT) {
        cout << "Error de tipos:  tienen que ser enteros" << endl;
        exit(0);
    }
    v.int_value++;
    env.update(s->id, v);
    return;
}


void ImpInterpreter::visit(ForStmt* s) {
    env.add_level();

    s->init->accept(this);

    while (true) {
        ImpValue conditionValue = s->condition->accept(this);
        if (conditionValue.type != TBOOL) {
            cout << "Error de tipos: la condición del ciclo debe ser booleana." << endl;
            exit(0);
        }

        if (!conditionValue.bool_value) {
            break;
        }

        s->body->accept(this);

        s->step->accept(this);
    }

    env.remove_level();
    return;
}


void ImpInterpreter::visit(WhileStmt* s) {
    while (true) {
        ImpValue conditionValue = s->condition->accept(this);
        if (conditionValue.type != TBOOL) {
            cout << "Error de tipos: la condición del ciclo debe ser booleana." << endl;
            exit(0);
        }

        if (!conditionValue.bool_value) {
            break;
        }

        s->body->accept(this);
    }

    return;
}
ImpValue ImpInterpreter::visit(BinaryExp* e) {
    ImpValue result;
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);
    if (v1.type != TINT || v2.type != TINT) {
        cout << "Error de tipos: operandos en operacion binaria tienen que ser "
                "enteros"
             << endl;
        exit(0);
    }
    int iv, iv1, iv2;
    bool bv;
    ImpVType type = NOTYPE;
    iv1 = v1.int_value;
    iv2 = v2.int_value;
    switch (e->op) {
        case PLUS_OP:
            iv = iv1 + iv2;
            type = TINT;
            break;
        case MINUS_OP:
            iv = iv1 - iv2;
            type = TINT;
            break;
        case MUL_OP:
            iv = iv1 * iv2;
            type = TINT;
            break;
        case DIV_OP:
            iv = iv1 / iv2;
            type = TINT;
            break;
    }
    if (type == TINT)
        result.int_value = iv;
    else
        result.bool_value = bv;
    result.type = type;
    return result;
}

ImpValue ImpInterpreter::visit(NumberExp* e) {
    ImpValue v;
    v.set_default_value(TINT);
    v.int_value = e->value;
    return v;
}

ImpValue ImpInterpreter::visit(IdentifierExp* e) {
    if (env.check(e->name))
        return env.lookup(e->name);
    else {
        cout << "Variable indefinida: " << e->name << endl;
        exit(0);
    }
}

ImpValue ImpInterpreter::visit(RelationalExp* e) {
    ImpValue result;
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);
    if (v1.type != TINT || v2.type != TINT) {
        cout << "Error de tipos: operandos en operacion relacional tienen que "
                "ser enteros"
             << endl;
        exit(0);
    }
    int iv1, iv2;
    bool bv;
    ImpVType type = NOTYPE;
    iv1 = v1.int_value;
    iv2 = v2.int_value;
    switch (e->op) {
        case LT_OP:
            bv = iv1 < iv2;
            type = TBOOL;
            break;
        case LE_OP:
            bv = iv1 <= iv2;
            type = TBOOL;
            break;
        case EQ_OP:
            bv = iv1 == iv2;
            type = TBOOL;
            break;
        case GT_OP:
            bv = iv1 > iv2;
            type = TBOOL;
            break;
        case GE_OP:
            bv = iv1 >= iv2;
            type = TBOOL;
            break;
        case NE_OP:
            bv = iv1 != iv2;
            type = TBOOL;
            break;
    }
    if (type == TBOOL)
        result.bool_value = bv;
    else
        result.int_value = iv1;
    result.type = type;
    return result;
}

ImpValue ImpInterpreter::visit(FuncCallExp* e) {
    FuncDecl* fdec = fdecs.lookup(e->name);
    if (!fdec) {
        cout << "Error: La función " << e->name << " no está declarada." << endl;
        exit(0);
    }

    if (fdec->type == "void") {
        cout << "Error: Una función de tipo void no puede asignarse a una variable." << endl;
        exit(0);
    }

    env.add_level();

    if (fdec->params->params.size() != e->args->args.size()) {
        cout << "Error: Número incorrecto de parámetros en la llamada a la función "
             << e->name << endl;
        exit(0);
    }

    auto argIt = e->args->args.begin();
    auto paramIt = fdec->params->params.begin();
    while (argIt != e->args->args.end() && paramIt != fdec->params->params.end()) {
        Param* param = *paramIt;
        CExp* arg = *argIt;

        ImpValue argValue = arg->accept(this);

        ImpVType expectedType = ImpValue::get_basic_type(param->type);
        if (argValue.type != expectedType) {
            cout << "Error en llamada a función: Tipos de parámetro y argumento no coinciden. "
                 << "Función " << e->name << ", parámetro " << param->name << endl;
            exit(0);
        }

        env.add_var(param->name, argValue);

        ++argIt;
        ++paramIt;
    }

    retcall = false;
    fdec->stmts->accept(this);

    if (!retcall) {
        cout << "Error: La función " << e->name << " no ejecutó una sentencia RETURN." << endl;
        exit(0);
    }

    retcall = false;
    env.remove_level();

    ImpVType returnType = ImpValue::get_basic_type(fdec->type);
    if (retval.type != returnType) {
        cout << "Error: Tipo de retorno incorrecto en la función " << e->name << endl;
        exit(0);
    }

    return retval;
}

void ImpInterpreter::visit(FuncCallStmt* s) {
    FuncDecl* fdec = fdecs.lookup(s->funcCall->name);
    if (!fdec) {
        cout << "Error: Funcion " << s->funcCall->name << " no declarada." << endl;
        exit(0);
    }

    env.add_level();

    if (fdec->params->params.size() != s->funcCall->args->args.size()) {
        cout << "Error: Numero de parametros incorrecto en llamada a "
             << s->funcCall->name << endl;
        exit(0);
    }

    auto argIt = s->funcCall->args->args.begin();
    auto paramIt = fdec->params->params.begin();
    while (argIt != s->funcCall->args->args.end() && paramIt != fdec->params->params.end()) {
        Param* param = *paramIt;
        CExp* arg = *argIt;

        ImpValue v = arg->accept(this);

        ImpVType expectedType = ImpValue::get_basic_type(param->type);
        if (v.type != expectedType) {
            cout << "Error: No coincide el tipo en la llamada a la función " << s->funcCall->name
                << ". Se esperaba el tipo " << param->type << " para el parámetro " << param->name
                << ", pero se recibió un tipo diferente." << endl;
            exit(0);
        }

        env.add_var(param->name, v);

        ++argIt;
        ++paramIt;
    }

    fdec->stmts->accept(this);

    env.remove_level();

    return;
}
