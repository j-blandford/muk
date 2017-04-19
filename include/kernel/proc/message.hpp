#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std/vector.hpp>
#include <std/deque.hpp>

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
		
		char data;

		Message() : data('c') { }
		Message(int src_thread, char data) 
		, source_thread(src_thread)
		, data(data) { }
		~Message() { }

		friend bool operator==(const Message& m1, const Message& m2) { 
			return (m1.source_thread == m2.source_thread) 
				&& (m1.data == m2.data);
		}
	};

	// this class contains a queue for each port
	class PortQueue {
	public:
		int port;
		std::vector<Message> list;

		PortQueue(int port_id) : port(port_id) { list = std::vector<Message>(); }
		~PortQueue() { }
	}

	// "MessageQueue" is a wrapper class around the std::vector
	class MessageQueue {
	public:
		std::vector<PortQueue> ports;

		MessageQueue() { ports = std::vector<PortQueue>(); }
		~MessageQueue() { }

		Message* peek(int port_id);
		void push(int port_id, Message msg);
		Message* pop(int port_id);
		
		std::vector<Message*> filter(int port_id);
	};

	extern MessageQueue postbox;

	bool SendMessage(int port_id, int src_thread, char data);
}

void postbox_debug();
