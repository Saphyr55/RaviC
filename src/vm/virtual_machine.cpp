#include <iostream>
#include "vm/virtual_machine.hpp"
#include "vm/memory.hpp"

namespace VM {

	RVM::RVM(Chunk& c) : m_chunk(c), m_pc(), m_index_pc(0) {
		
	}

	void RVM::BinaryAdd()
	{
		Value b = m_values.top();
		m_values.pop();
		Value a = m_values.top();
		m_values.pop();
		m_values.push(a + b);
	}

	void RVM::BinaryMul()
	{
		Value b = m_values.top();
		m_values.pop();
		Value a = m_values.top();
		m_values.pop();
		m_values.push(a * b);
	}

	void RVM::BinarySub()
	{
		Value b = m_values.top();
		m_values.pop();
		Value a = m_values.top();
		m_values.pop();
		m_values.push(a - b);
	}

	void RVM::BinaryDiv()
	{
		Value b = m_values.top();
		m_values.pop();
		Value a = m_values.top();
		m_values.pop();
		m_values.push(a / b);
	}

	Byte RVM::Read8() {
		Byte pc = m_pc;
		if (m_index_pc + 1 < m_chunk.m_bytes.size())
			m_pc = m_chunk.m_bytes[++m_index_pc];
		return pc;
	}

	Value RVM::ReadConstant() {
		
		return m_chunk.m_memory.GetHandle()[Read8()];
	}

	InterpreteResult RVM::Run(std::string_view source) {
		return Run();
	}

	InterpreteResult RVM::Run() {

		while (m_index_pc < m_chunk.m_bytes.size()) {
#ifdef DEBUG_TRACE_EXECUTION
			m_chunk.Disassemble(m_index_pc);
#endif
			Byte instruction;
			switch (instruction = Read8()) {
			case OpCode::Return: {
				m_values.pop();
				return InterpreteResult::OK;
			}
			case OpCode::Constant: {
				Value constant = ReadConstant();
				m_values.push(constant);
				break;
			}
			case OpCode::Negate: {
				auto& negate_value = m_values.top();
				m_values.pop();
				m_values.push(-negate_value);
				break;
			}
			case OpCode::Add: 
				BinaryAdd(); 
				break;
			case OpCode::Substract:
				BinarySub();
				break;
			case OpCode::Divide:
				BinaryDiv();
				break;
			case OpCode::Multiply:
				BinaryMul();
				break;
			default:
				break;
			}
		}
	}

}

