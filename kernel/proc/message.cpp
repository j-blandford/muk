#include <kernel/proc/message.hpp>
#include <kernel/proc/scheduler.hpp>

using namespace Process;

MessageQueue Process::postbox;
Message Process::kMessageNull;

SpinlockMutex Process::messaging_mutex;

bool Process::SendMessage(int port_id, int src_thread, char data) {
	//Scheduler::lock();
	bcprintf("Sending message \"%c\" on port %d\n", data, port_id ); // data.getChar()
	postbox.push(port_id, Message(src_thread, data));
	//Scheduler::unlock();
	
	return true;
}

void Process::listen(int port_id) {
	auto port_it = postbox.search(port_id);

	if(port_it == std::end(postbox.ports)) {
		// port queue isn't found - let's add it
		postbox.ports.push_back(new PortQueue(port_id));
		// maybe return the iterator?
	} 
	else {
		// maybe return the iterator?
	}
}

bool Process::MessageList::push(Message msg) {
	if(this->size() < this->kMaxMessages) {
		list[wrap(head_index+list_size)] = msg;
		list_size++;

		return true;
	}

	// there's not enough space to fit the message in!
	return false;
}

Message Process::MessageList::pop() {
	Message pop = list[head_index];

	head_index = wrap(head_index + 1);
	list_size--;

	return pop;
}

void Process::MessageQueue::push(int port_id, Message msg) {
	auto port_it = search(port_id);

	if(port_it != postbox.ports.end()) {
		(*port_it)->messages.push(msg);

		bcprintf("Adding to port %d postbox. Size is now %d\n", port_id, (*port_it)->messages.size());
	}
}

Message* Process::MessageQueue::peek(int port_id) {
	auto port_it = search(port_id);

	if(port_it != std::end(postbox.ports)) {
		// PortQueue portbox = *port_it;
		//if(portbox.list.size() > 0) {
			//return &(portbox.list[0]);
		//}
		//else {
			return nullptr;
		//}
	}
	else {
		return nullptr;
	}
}

Message Process::MessageQueue::pop(int port_id) {
	auto port_it = search(port_id);

	if(port_it != std::end(postbox.ports)) {
		if((*port_it)->messages.size() > 0) {
			return (*port_it)->messages.pop();
		}
		else {
			return kMessageNull;
		}
	}
	else {
		return kMessageNull;
	}
}

std::vector<PortQueue*>::iterator Process::MessageQueue::search(int port_id) {
	auto element_it = std::find_if(postbox.ports.begin(), 
									postbox.ports.end(),
									PortQueueComp(port_id) );
	return element_it;
}

void postbox_debug() {
	// Message msg = Process::kMessageNull;

	// {
	// 	Locker<SpinlockMutex> messaging_locker(Process::messaging_mutex, Scheduler::threadId());
	// 	Process::listen(1);
	// }

	for(;;) {

		bcprintf("one ");
		// while((msg = Process::postbox.pop(1)) != Process::kMessageNull) {
		// 	bcprintf("Recv. message '%c' (port 1)\n", msg.data);

		// 	// this sends the keyboard packet to port 2, at the moment is the TTY driver
		// 	Process::SendMessage(2,Scheduler::threadId(), msg.data); // maybe make a "process::forwardmessage" function...
		// }

	}
}