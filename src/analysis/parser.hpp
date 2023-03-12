#pragma once

#include "common/common.hpp"
#include "analysis/lexer.hpp"
#include "vm/chunk.hpp"

namespace Analysis {

class Parser {

enum Precedence{
	NONE,
	ASSIGNMENT,
	OR,
	AND,
	EQUALITY,
	COMPARISON,
	TERM,    
	FACTOR,  
	UNARY,  
	CALL,
	PRIMARY
};

private:
    void Expression();
    void Number();
    void Grouping();
    void Unary();
    void ParsePrecedence(Precedence pre);
private:
	bool IsAtEnd();
	bool Check(Token::Kind kind);
    Ref<Token> Peek();
    Ref<Token> Advance();
    Ref<Token> Consume(Token::Kind kind, const std::string_view message);
	std::exception Report(const std::string& msg);
    std::exception Report(Ref<Token> tk, const std::string& msg);
    std::string Diagnostic(Ref<Token> tk, const std::string& msg);
    void Emit8(const Byte& byte);
	void Emit16(const Byte& byte1, const Byte& byte2);
    void EmitConstant(const Value& value);

public:
    Parser(VM::Chunk& current_chunk, Lexer& lexer);
    ~Parser() = default;

private:
    Lexer lexer;
    Ref<Token> m_current;
    Ref<Token> m_previous;
	VM::Chunk& current_chunk;
};
    
}
