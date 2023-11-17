// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
	: m_heap(nullptr), m_size(0), m_priorFunc(priFn), m_heapType(heapType), m_structure(structure) {
}

PQueue::~PQueue() { clear(); }
void PQueue::clear() {
	clearQueue(m_heap);
	m_size = 0;
	m_heap = nullptr;
}
PQueue::PQueue(PQueue& rhs)
{
	// Perform a deep copy of the queue
	m_heap = nullptr;
	m_size = 0;
	m_priorFunc = rhs.m_priorFunc;
	m_heapType = rhs.m_heapType;
	m_structure = rhs.m_structure;

	// Copy the queue structure recursively
	copyQueue(m_heap, rhs.m_heap);
	m_size = rhs.m_size;
}
PQueue& PQueue::operator=(PQueue& rhs) {
    if (this != &rhs) {
        // Clear the existing content of the queue
        clear();

        // Copy the queue structure recursively
        copyQueue(m_heap, rhs.m_heap);

        // Update other members
        m_size = rhs.m_size;
        m_priorFunc = rhs.m_priorFunc;
        m_heapType = rhs.m_heapType;
        m_structure = rhs.m_structure;
    }
    return *this;
}

//PQueue& PQueue::operator=(PQueue& rhs) {
//	if (this != &rhs) {
//		clear();
//		m_priorFunc = rhs.m_priorFunc;
//		m_heapType = rhs.m_heapType;
//		m_structure = rhs.m_structure;
//
//		if (rhs.m_heap) {
//			m_heap = new Node(rhs.m_heap->getPatient());
//			m_size = 1;
//			copyQueue(rhs.m_heap, m_heap);
//		}
//	}
//	return *this;
//}
void PQueue::mergeWithQueue(PQueue& rhs) {
	if (isEmpty() && rhs.isEmpty()) {
		return;  // Both queues are empty, nothing to merge
	}

	if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure) {
		throw domain_error("Cannot merge queues with different priority functions or data structures.");
	}
	// Check if both queues are empty
	if (this == &rhs) {
		return;  // Protection against self-merging
	}


	m_heap = merge(m_heap, rhs.m_heap);
	m_size += rhs.m_size;

	rhs.m_heap = nullptr;
	rhs.m_size = 0;
}

void PQueue::insertPatient(const Patient& input) {
	Node* newNode = new Node(input);
	m_heap = merge(m_heap, newNode);
	m_size++;
}
//void PQueue::insertPatient(const Patient& patient) {
//	Node* newNode = new Node(patient);
//
//	if (m_heap) {
//		m_heap = merge(m_heap, newNode);
//	}
//	else {
//		m_heap = newNode;
//	}
//
//	m_size++;
//}
int PQueue::numPatients() const { return m_size; }
prifn_t PQueue::getPriorityFn() const { return m_priorFunc; }
Patient PQueue::getNextPatient() {
	if (m_size == 0) {
		throw out_of_range("Queue is empty");
	}

	Patient highestPriorityPatient = m_heap->getPatient();

	Node* leftSubHeap = m_heap->getLeft();
	Node* rightSubHeap = m_heap->getRight();

	delete m_heap;

	m_heap = merge(leftSubHeap, rightSubHeap);
	m_size--;

	return highestPriorityPatient;
}
void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
	m_priorFunc = priFn;
	m_heapType = heapType;
	// Rebuild the heap with the new priority function
	Node* tempHeap = m_heap;
	m_heap = nullptr;
	m_size = 0;
	while (tempHeap) {
		insertPatient(tempHeap->getPatient());
		Node* leftSubHeap = tempHeap->getLeft();
		Node* rightSubHeap = tempHeap->getRight();
		delete tempHeap;
		tempHeap = merge(leftSubHeap, rightSubHeap);
	}
}
void PQueue::setStructure(STRUCTURE structure) {
	if (m_structure != structure) {
		m_structure = structure;
		// Rebuild the heap with the new structure
		Node* tempHeap = m_heap;
		m_heap = nullptr;
		m_size = 0;
		while (tempHeap) {
			insertPatient(tempHeap->getPatient());
			Node* leftSubHeap = tempHeap->getLeft();
			Node* rightSubHeap = tempHeap->getRight();
			delete tempHeap;
			tempHeap = merge(leftSubHeap, rightSubHeap);
		}
	}
}
STRUCTURE PQueue::getStructure() const { return m_structure; }
HEAPTYPE PQueue::getHeapType() const { return m_heapType; }
void PQueue::printPatientQueue() const { printQueue(m_heap); }
void PQueue::dump() const {
	if (m_size == 0) {
		cout << "Empty heap.\n";
	}
	else {
		dump(m_heap);
	}
	cout << endl;
}
void PQueue::dump(Node* pos) const {
	if (pos != nullptr) {
		cout << "(";
		dump(pos->m_left);
		if (m_structure == SKEW)
			cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
		else
			cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
		dump(pos->m_right);
		cout << ")";
	}
}
ostream& operator<<(ostream& sout, const Patient& patient) {
	sout << patient.getPatient()
		<< ", temperature: " << patient.getTemperature()
		<< ", oxygen: " << patient.getOxygen()
		<< ", respiratory: " << patient.getRR()
		<< ", blood pressure: " << patient.getBP()
		<< ", nurse opinion: " << patient.getOpinion();
	return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
	sout << node.getPatient();
	return sout;
}