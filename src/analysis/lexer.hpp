#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <deque>

#include "common/common.hpp"

namespace Analysis {

struct Token {

public:
    enum class Kind {
		TkEOF,
		Func,
		Number,
		Let,
		Mut,
		Return,
		Identifier,
		True,
		False,
		Struct,
		Class,
		While,
		For,
		If,
		Else,
		Namespace,
		Less,
		LessEqual,
		Greater,
		GreaterEqual,
		Semicolon,
		Colon,
		Dot,
		Arrow,
		OpenBracket,
		CloseBracket,
		OpenParenthesis,
		CloseParenthesis,
		LogicalAnd,
        LogicalOr,
        BinaryAnd,
        BinaryOr,
		Comma,
		Assign,
		Not,
		NotEqual,
		Equal,
		Minus, 
		Plus,
		Star,
		Slash,
		String,
        Null,
	};

public:
    static std::string ToString(Kind kind);

public:
	Token(Kind kindType, std::string_view text, std::size_t line, std::size_t col) :
        Type(kindType), Text(text), Line(line), Col(col) { }

public:
	Kind Type;
	std::string Text;
	std::size_t Line;
    std::size_t Col;
};

using RToken = Ref<Token>;

static const std::unordered_map<std::string, Token::Kind> Words {
	{ "func",       Token::Kind::Func},
	{ "true" ,      Token::Kind::True},
	{ "false",      Token::Kind::False},
	{ "let",        Token::Kind::Let},
	{ "mut",        Token::Kind::Mut},
	{ "return",     Token::Kind::Return},
	{ "struct",     Token::Kind::Struct},
	{ "class",      Token::Kind::Class},
	{ "while",      Token::Kind::While},
	{ "for",        Token::Kind::For},
	{ "if",         Token::Kind::If},
	{ "else",       Token::Kind::Else},
	{ "namespace",  Token::Kind::Namespace},
};

class Lexer {

public:
	static const char Space = ' ';
	static const char Dot = '.';
	static const char Ampersand = '&';
	static const char Pipeline = '|';
	static const char BackslashN = '\n';
	static const char BackslashT = '\t';
	static const char BackslashR = '\r';
	static const char Backslash0 = '\0';
	static const char PlusOp = '+';
	static const char MinusOp = '-';
	static const char StartOp = '*';
	static const char SlashOp = '/';
	static const char SingleQuote = '\'';
	static const char DoubleQuote = '"';
	static const char BackQuote = '`';
	static const char OpenParenthesis = '(';
	static const char CloseParenthesis = ')';
	static const char OpenBrackets = '{';
	static const char CloseBrackets = '}';
	static const char Colon = ':';
	static const char Semicolon = ';';
	static const char Comma = ',';
	static const char Bang = '!';
	static const char Greater = '>';
	static const char Less = '<';
	static const char Equal = '=';
	
public:
	void NextToken();
	Ref<Token> GetCurrentTk();
	Ref<Token> PeekNextToken();
	bool IsAtEnd();

private:
	void AddToken(Token::Kind kind);
	bool Match(char expected);
	char Peek();
	void AddDefaultToken(char c);
	void AddNumberToken();
	void AddIdentifierToken();
	void CreateStringToken(char q);
	void AddSlashToken();
	Ref<Token> GetLastToken();
	char PeekNext();
	char Advance();
	std::size_t Next();
	void Report(std::string message);

public:
	explicit Lexer(std::string_view text);
	Lexer(const Lexer&) = default;
	Lexer(Lexer&&) = default;
	~Lexer() = default;

	std::size_t m_position;
	std::size_t m_start;
private:
	std::string m_text;
	std::size_t m_line;
	std::size_t m_col;
	Ref<Token> m_last_token;
	Ref<Token> m_current_token;

};

}
