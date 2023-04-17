#include <utility>

#include "analysis/ast/expression.hpp"
#include "analysis/ast/statement.hpp"

namespace Analysis::AST::Expr {

	Lambda::Lambda(Ref<::Analysis::AST::Stmt::Function> function)
		: StatementFunc(std::move(function)) { }

	Ref<Common::Value> Lambda::GenCode() {
		return {};
	}

	Assign::Assign(Ref<Analysis::Token> name, Ref<Expression> value)
		: Value(std::move(value)), Name(std::move(name)) { }

	Ref<Common::Value> Assign::GenCode() {
		return {};
	}

	Unary::Unary(Ref<Expression> right,  Ref<Analysis::Token> op)
		: Right(std::move(right)), Op(std::move(op)) { }

	Ref<Common::Value> Unary::GenCode() {
		return {};
	}

	Logical::Logical(Ref<Expression> left, Ref<Analysis::Token> op, Ref<Expression> right) :
		Left(std::move(left)), Right(std::move(right)), Op(std::move(op)) { }

	Ref<Common::Value> Logical::GenCode()
	{
		return {};
	}

	This::This(Ref<Analysis::Token> kw) : kw(std::move(kw)) { }

	Ref<Common::Value> This::GenCode()
	{
		return {};
	}

	Grouping::Grouping(Ref<Expression> e)
		: Expression_(std::move(e)) { }

	Ref<Common::Value> Grouping::GenCode()
	{
		return {};
	}

	Let::Let(Ref<Analysis::Token> name)
		: Name(std::move(name)) { }

	Ref<Common::Value> Let::GenCode()
	{
		return {};
	}

	Call::Call(Ref<Expression> calle, std::vector<Ref<Expression>> args)
		: Calle(std::move(calle)), Args(std::move(args)) {}

	Ref<Common::Value> Call::GenCode()
	{
		return {};
	}

	Binary::Binary(Ref<Expression> left, Ref<Analysis::Token> op, Ref<Expression> right)
		: Op(std::move(op)), Left(std::move(left)), Right(std::move(right)) { }

	Ref<Common::Value> Binary::GenCode()
	{
		return {};
	}

	Literal::Literal(std::string_view value)
		: Value(value) { }

	Ref<Common::Value> Literal::GenCode()
	{
		return {};
	}

	Getter::Getter(Ref<Expression> value, Ref<Analysis::Token> name)
		: Value(std::move(value)), Name(std::move(name)) { }

	Ref<Common::Value> Getter::GenCode()
	{
		return {};
	}

	Setter::Setter(Ref<Expression> object, Ref<Analysis::Token> name, Ref<Expression> value)
		: Obj(std::move(object)), Name(std::move(name)), Value(std::move(value)) { }

	Ref<Common::Value> Setter::GenCode()
	{
		return {};
	}

}