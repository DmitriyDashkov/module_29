#include <mutex>
#include <iostream>
using namespace std;

struct Node
{
	int value;
	Node* next;
	std::mutex* node_mutex;
	Node() : next(nullptr), value(0) {
		node_mutex = new std::mutex;
	}
	~Node() {
		delete node_mutex;
	}
};

class FineGrainedQueue
{
public:
	Node* head;
	std::mutex* queue_mutex;
	FineGrainedQueue() {
		head = nullptr;
		queue_mutex = new std::mutex;
	}
	~FineGrainedQueue() {
		delete queue_mutex;
	}
	
	void insertIntoMiddle(int value, int pos)
	{
		queue_mutex->lock();				//лочим всю очередь
		Node* new_node = new Node;			//новый узел
		new_node->value = value;
		Node* cur = new Node;
		cur = head;							//текущий элемент
		cur->node_mutex->lock();			//лочим текущий элемент
		queue_mutex->unlock();				//разлочиваем очередь
		int i = 0;
		while (i < pos - 1) {
			if (cur->next != nullptr) {
			cur->next->node_mutex->lock();	// лочим следующий элемент
			cur->node_mutex->unlock();		// разлочим текущий элемент
			cur = cur->next;
			}
			i++;
		}
		// Добавляем новый узел new_node в позицию pos
		Node* tmp = cur->next;
		cur->next = new_node;
		new_node->next = tmp;

		cur->node_mutex->unlock();			//разлочим следующий элемент (cur уже = cur->next;)
	}
	void writeQueue() {
		Node* cur = this->head;
		cout << cur->value << "		";
		while (cur->next != nullptr) {
			cout << cur->next->value << "		";
			cur = cur->next;
		}
		cout << endl;
	}
};

int main()
{
	Node* node0 = new Node;
	node0->value = 0;
	Node *node1 = new Node;
	node1->value = 1;
	Node* node2 = new Node;
	node2->value = 2;
	Node* node3 = new Node;
	node3->value = 3;

	FineGrainedQueue f;
	f.head = node0;
	node0->next = node1;
	node1->next = node2;
	node2->next = node3;

	f.writeQueue();
	f.insertIntoMiddle(100, 90);
	f.writeQueue();


	
	return 0;
}

