#include "analysis/ast/statement.hpp"

namespace AST {

namespace Stmt {

	Function::Function(
		Analysis::RToken name, Analysis::RToken returnType,
		std::vector<Analysis::RToken> paramsId,
		std::vector<Analysis::RToken> paramsType,
		std::vector<RStatement> block
	) :
		Name(name),
		ParamsId(paramsId),
		ParamsType(paramsType),
		ReturnType(returnType) { }

	Block::Block(std::vector<Ref<Statement>> statements) : Statements(statements)
	{
	}

	While::While(Ref<::AST::Expression> condition_, Ref<Statement> body) : Body(body), Condition_(condition_)
	{
	}

	Expression::Expression(Ref<::AST::Expression> expression)
		: Expression_(expression) { }

	If::If(Ref<::AST::Expression> condition, RStatement thenBranch, RStatement elseBranch) :
		Condition(condition), ThenBranch(thenBranch), ElseBranch(elseBranch) {}

	Let::Let(Analysis::RToken name, Analysis::RToken type, Ref<::AST::Expression> initializer, bool mutable_) :
		Name(name),
		Type(type),
		Initializer(initializer),
		Mutable(mutable_) {}

	Return::Return(Analysis::RToken kw, ::AST::RExpression value) :
		Kw(kw), Value(value), FunctionStmt(nullptr) { }
}

}