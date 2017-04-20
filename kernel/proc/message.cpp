#include <kernel/proc/message.hpp>
#include <kernel/proc/scheduler.hpp>

using namespace Process;

MessageQueue Process::postbox;
Message Process::kMessageNull;

bool Process::SendMessage(int port_id, int src_thread, char data) {
	Scheduler::lock();
	//bcprintf("Sending msg... %c\n", data ); // data.getChar()
	postbox.push(port_id, Message(src_thread, data));
	Scheduler::unlock();
	
	return true;
}

void Process::MessageQueue::push(int port_id, Message msg) {
	auto port_it = search(port_id);

	if(port_it != std::end(postbox.ports)) {
		PortQueue portbox = *port_it;
		portbox.list.push(msg);
	}
}

Message* Process::MessageQueue::peek(int port_id) {
	auto port_it = search(port_id);

	if(port_it != std::end(postbox.ports)) {
		PortQueue portbox = *port_it;
		if(portbox.list.size() > 0) {
			return &(portbox.list[0]);
		}
		else {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

Message Process::MessageQueue::pop(int port_id) {
	auto port_it = search(port_id);

	if(port_it != std::end(postbox.ports)) {
		PortQueue portbox = *port_it;
		if(portbox.list.size() > 0) {
			// we have to REMOVE the message from the list
			Message return_msg = portbox.list[0];
			portbox.list.pop();

			return return_msg;
		}
		else {
			return kMessageNull;
		}
	}
	else {
		return kMessageNull;
	}
}

std::vector<PortQueue>::iterator Process::MessageQueue::search(int port_id) {
	auto element_it = std::find_if(postbox.ports.begin(), 
									postbox.ports.end(),
									PortQueueComp(1) );

	return element_it;
}

void postbox_debug() {
	Message msg = Process::kMessageNull;
	for(;;) {
		Scheduler::lock();
		while((msg = Process::postbox.pop(1)) != Process::kMessageNull) {
			bcprintf("Message Recieved: %c\n",msg.data); // getChar()

			//msg = Process::postbox.pop(1);
		}
		Scheduler::unlock();
	}
}