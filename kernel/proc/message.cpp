#include <kernel/proc/message.hpp>

using namespace Process;

MessageQueue<Message> Process::postbox;

bool Process::SendMessage(int port_id, int src_thread, VariantType::Type data) {
	postbox.push(Message(port_id, src_thread, data));
	return true;
}

void Process::MessageQueue::push(Message msg) {
	list.push_back(msg);
}

std::vector<Message*> Process::MessageQueue::filter(int port_id) {
	std::vector<Message*> filtered = std::vector<Message*>();

	for(auto x : list) {
		if(x.dest_port == port_id) 
			filtered.push_back(&x);
	}
	return filtered;
}

Message* Process::MessageQueue::peek(int port_id) {
	std::vector<Message*> port_postbox = filter(port_id);

	if(port_postbox.size() > 0) { 
		return &(port_postbox[0]);
	}
	else { 
		return nullptr;
	}
}

Message* Process::MessageQueue::pop(int port_id) {
	std::vector<Message*> port_postbox = filter(port_id);

	if(port_postbox.size() > 0) { 
		return &(port_postbox[0]);
	}
	else { 
		return nullptr;
	}
}