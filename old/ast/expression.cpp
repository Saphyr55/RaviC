#include "analysis/ast/expression.hpp"
#include "analysis/ast/statement.hpp"

namespace AST {

namespace Expr {

	Lambda::Lambda(Ref<::AST::Stmt::Function> function)
		: StatementFunc(function) { }

	Ref<Value> Lambda::GenCode()
	{
		return Ref<Value>();
	}

	Assign::Assign(Ref<Analysis::Token> name, Ref<Expression> value)
		: Value(value), Name(name) { }

	Ref<Value> Assign::GenCode()
	{
		return Ref<Value>();
	}

	Unary::Unary(Ref<Expression> right,  Ref<Analysis::Token> op)
		: Right(right), Op(op) { }

	Ref<Value> Unary::GenCode()
	{
		return Ref<Value>();
	}

	Logical::Logical(Ref<Expression> left, Ref<Analysis::Token> op, Ref<Expression> right) :
		Left(left), Right(right), Op(op) { }

	Ref<Value> Logical::GenCode()
	{
		return Ref<Value>();
	}

	This::This(Ref<Analysis::Token> kw) : kw(kw) { }

	Ref<Value> This::GenCode()
	{
		return Ref<Value>();
	}

	Grouping::Grouping(Ref<Expression> e)
		: Expression_(e) { }

	Ref<Value> Grouping::GenCode()
	{
		return Ref<Value>();
	}

	Let::Let(Ref<Analysis::Token> name)
		: Name(name) { }

	Ref<Value> Let::GenCode()
	{
		return Ref<Value>();
	}

	Call::Call(Ref<Expression> calle, std::vector<Ref<Expression>> args)
		: Calle(calle), Args(std::move(args)) {}

	Ref<Value> Call::GenCode()
	{
		return Ref<Value>();
	}

	Binary::Binary(Ref<Expression> left, Ref<Analysis::Token> op, Ref<Expression> right)
		: Op(op), Left(std::move(left)), Right(std::move(right)) { }

	Ref<Value> Binary::GenCode()
	{
		return Ref<Value>();
	}

	Literal::Literal(void* value)
		: Value(value) { }

	Ref<Value> Literal::GenCode()
	{
		return Ref<Value>();
	}

	Getter::Getter(Ref<Expression> value,Ref<Analysis::Token> name)
		: Value(value), Name(name) { }

	Ref<Value> Getter::GenCode()
	{
		return Ref<Value>();
	}

	Setter::Setter(Ref<Expression> object, Ref<Analysis::Token> name, Ref<Expression> value)
		: Obj(object), Name(name), Value(value) { }

	Ref<Value> Setter::GenCode()
	{
		return Ref<Value>();
	}

}
}