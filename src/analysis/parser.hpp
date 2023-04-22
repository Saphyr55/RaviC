#pragma once

#include <unordered_map>
#include <functional>

#include "vm/chunk.hpp"
#include "analysis/lexer.hpp"
#include "analysis/checker.hpp"
#include "common/value.hpp"

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
    void Decleration();
    void Statement();
    void Literal();
    void Number();
    void Let();
    void Grouping();
    void Binary();
    void Unary();
    void String();
    void PrintStatement();
    void ExpressionStatement();
    void LetDeclaration();
    void ParsePrecedence(Precedence pre);
	bool IsAtEnd();
    bool Match(Token::Kind kind);
	bool Check(Token::Kind kind);
    void Synchronize();
	Ref<Token> Peek();
	Ref<Token> Advance();
	Ref<Token> Consume(Token::Kind kind, std::string_view message);
	std::exception Report(std::string_view msg);
    std::exception Report(const Ref<Token>& tk, std::string_view msg);
	std::string Diagnostic(const Ref<Token>& tk, std::string_view msg);
	void Emit8(const Byte& byte);
	void Emit16(const Byte& byte1, const Byte& byte2);
	void EmitConstant(const Ref<Common::Value>& value);
    std::size_t IdentifierConstant(const Ref<Token> &name);
    static std::string CopyString(const Ref<Token>& tk);
    static std::string CopyStringDQ(const Ref<Token>& tk);

public:
    Parser(Checker checker, VM::Chunk& current_chunk, Lexer& lexer);
    ~Parser() = default;

private:
    Checker checker;
    Lexer lexer;
    Ref<Token> m_current;
    Ref<Token> m_previous;
	VM::Chunk& m_current_chunk;
    bool m_panic_mode = false;
    bool m_can_assign = true;

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
