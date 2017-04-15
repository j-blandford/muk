#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std/vector.hpp>

#include <std/ringbuffer.hpp>

namespace Process {
	struct VariantType {
		enum Type {
			Char,
			Int
		};

		VariantType(const char& val) : type(Char), val(val) { }
		VariantType(const int& val) : type(Int), val(val) { }

		Type getType() const { return type; }
		char getChar() const { assert(type == Char); return val.getChar(); }
		int getInt() const { assert(type == Int);  return val.getInt(); }
	private:
		union Union {
			Union(const char& val) : charValue(val) { }
			Union(const int& val) : intValue(val) { }
			char getChar() const { return charValue; }
			int getInt() const { return intValue; }
		private:
			char charValue;
			int intValue;
		};

		Type type;
		Union val;
	};

	class Message {
	public:
		int source_thread;
		int dest_port;
		VariantType data;

		Message(int dest_port, int src_thread, VariantType::Type data) 
		: dest_port(dest_port)
		, source_thread(src_thread)
		, data(data) { }
		~Message() { }
	};

	// "Queue" is a wrapper class around the std::vector
	class MessageQueue {
	public:
		std::vector<Message> list;

		MessageQueue() { list = std::vector<Message>(); }
		~MessageQueue() { }

		Message* peek(int port_id);
		void push(Message msg);
		void pop(int port_id);
		
		std::vector<Message*> filter(int port_id);
	};

	extern Queue<Message> postbox;

	bool SendMessage(int port_id, int src_thread, VariantType::Type data);
}
