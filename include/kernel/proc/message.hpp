#pragma once

#include <stddef.h>
#include <stdint.h>

#include <std/algorithm.hpp>
#include <std/vector.hpp>
#include <std/queue.hpp>

#include <kernel/proc/mutex.hpp>

namespace Process {

	inline static int floor(double x) {
		int xi = (int)x;
		return x < xi ? xi - 1 : xi;
	}

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

		Message() 
		: source_thread(0)
		, data(0) { }

		Message(int src_thread, char data) 
		: source_thread(src_thread)
		, data(data) { }

		~Message() { }

		friend bool operator==(const Message& m1, const Message& m2) { 
			return (m1.source_thread == m2.source_thread) 
				&& (m1.data == m2.data);
		}
		friend bool operator!=(const Message& m1, const Message& m2) { 
			return (m1.source_thread != m2.source_thread) 
				&& (m1.data != m2.data);
		}
	};

	// this is a wrapper so we can plug-n-play the message queue depending on what
	// works best
	class MessageList {
		const static int kMaxMessages = 32;
		int head_index;
		int list_size;
		Message* list;

	public:
		MessageList()
		: head_index(0)
		, list_size(0)
		, list(new Message[kMaxMessages]) { }

		~MessageList() { }

		bool push(Message msg);
		Message pop();

		int size() const { return list_size; }
		int wrap(int index) const { return index - floor((double)index / (double)kMaxMessages); }

		Message& operator[](size_t index);
	};

	// this class contains a queue for each port
	class PortQueue {
	public:
		int port;
		MessageList messages;

		PortQueue() = default;
		PortQueue(int port_id) 
		: port(port_id) { }
		~PortQueue() { }

		friend bool operator==(const PortQueue& p1, const PortQueue& p2) { 
			return (p1.port == p2.port);
		}
		
	};

	// "MessageQueue" is a wrapper class around the std::vector
	class MessageQueue {
	public:
		std::vector<PortQueue*> ports;

		MessageQueue() { ports = std::vector<PortQueue*>(); }
		~MessageQueue() { }

		Message* peek(int port_id);
		void push(int port_id, Message msg);
		Message pop(int port_id);
		
		std::vector<PortQueue*>::iterator search(int port_id);
	};

	struct PortQueueComp
	{
		explicit PortQueueComp(int port) : port(port) { }
		inline bool operator()(const PortQueue* m) const { return m->port == port; }
	private:
		int port;
	};

	extern MessageQueue postbox;
	extern Message kMessageNull; // used as a comparator for "NULL"
	
	extern SpinlockMutex messaging_mutex;

	bool SendMessage(int port_id, int src_thread, char data);
	bool SendMessage(int port_id, char data);
	void listen(int port_id);
	
}

void postbox_debug();
