#include "ast/expression.hpp"
#include "ast/statement.hpp"

namespace AST {

namespace Expr {

	Lambda::Lambda(Ref<::AST::Stmt::Function> function)
		: StatementFunc(function) { }

	Assign::Assign(Ref<Token> name, Ref<Expression> value)
		: Value(value), Name(name) { }

	Unary::Unary(Ref<Expression> right,  Ref<Token> op)
		: Right(right), Op(op) { }

	Logical::Logical(Ref<Expression> left, Ref<Token> op, Ref<Expression> right) :
		Left(left), Right(right), Op(op) { }

	This::This(Ref<Token> kw) : kw(kw) { }

	Grouping::Grouping(Ref<Expression> e)
		: Expression_(e) { }

	Let::Let(Ref<Token> name)
		: Name(name) { }

	Call::Call(Ref<Expression> calle, std::vector<Ref<Expression>> args)
		: Calle(calle), Args(std::move(args)) {}

	Binary::Binary(Ref<Expression> left, Ref<Token> op, Ref<Expression> right)
		: Op(op), Left(std::move(left)), Right(std::move(right)) { }

	Literal::Literal(void* value)
		: Value(value) { }

	Getter::Getter(Ref<Expression> value,Ref<Token> name)
		: Value(value), Name(name) { }

	Setter::Setter(Ref<Expression> object, Ref<Token> name, Ref<Expression> value)
		: Obj(object), Name(name), Value(value) { }

}
}