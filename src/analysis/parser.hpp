#pragma once

#include <unordered_map>
#include <functional>

#include "common/common.hpp"
#include "analysis/lexer.hpp"
#include "vm/chunk.hpp"

namespace Analysis {

class Parser {

public:
enum Precedence : Byte {
	NONE = 0,
	ASSIGNMENT = 1,
	OR = 2,
	AND = 3,
	EQUALITY = 4,
	COMPARISON = 5,
	TERM = 6,
	FACTOR = 7,
	UNARY = 8,
	CALL = 9,
	PRIMARY = 10
};
public:
	void Expression();
    void Number();
    void Grouping();
    void Binary();
    void Unary();
    void ParsePrecedence(Precedence pre);
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

private:

class Rule {

public:
	static Rule Get(Token::Kind type);
	std::function<void(Parser&)> prefix;
	std::function<void(Parser&)> infix;
	Parser::Precedence precedence;

public:
	Rule(std::function<void(Parser&)> prefix, std::function<void(Parser&)> infix, Parser::Precedence precedence);
	Rule() = default;
	Rule(const Rule&) = default;
	Rule(Rule&&) = default;
	~Rule() = default;


private:
	static std::unordered_map<Token::Kind, Rule> rules;

};


};

}
