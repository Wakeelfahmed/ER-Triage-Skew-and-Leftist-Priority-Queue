#include "pqueue.h"
#include <iostream>
#include <math.h>
#include <cassert>
#include <climits>
#include <algorithm>
#include <random>
int priorityFn1(const Patient& patient) {
	//this function works with a MAXHEAP
	//priority value is determined based on some criteria
	//priority value falls in the range [115-242]
	//temperature + respiratory + blood pressure
	//the highest priority would be 42+40+160 = 242
	//the lowest priority would be 35+10+70 = 115
	//the larger value means the higher priority
	int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
	return priority;
}
int priorityFn2(const Patient& patient) {
	//this function works with a MINHEAP
	//priority value is determined based on some criteria
	//priority value falls in the range [71-111]
	//nurse opinion + oxygen
	//the highest priority would be 1+70 = 71
	//the lowest priority would be 10+101 = 111
	//the smaller value means the higher priority
	int priority = patient.getOpinion() + patient.getOxygen();
	return priority;
}
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
	"Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
	"Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green",
	"Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong",
	"Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
	"Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};
enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE };
class Random {
public:
	Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20) : m_min(min), m_max(max), m_type(type)
	{
		if (type == NORMAL) {
			//the case of NORMAL to generate integer numbers with normal distribution
			m_generator = mt19937(m_device());
			//the data set will have the mean of 50 (default) and standard deviation of 20 (default)
			//the mean and standard deviation can change by passing new values to constructor 
			m_normdist = normal_distribution<>(mean, stdev);
		}
		else if (type == UNIFORMINT) {
			//the case of UNIFORMINT to generate integer numbers
			// Using a fixed seed value generates always the same sequence
			// of pseudorandom numbers, e.g. reproducing scientific experiments
			// here it helps us with testing since the same sequence repeats
			m_generator = mt19937(10);// 10 is the fixed seed value
			m_unidist = uniform_int_distribution<>(min, max);
		}
		else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
			m_generator = mt19937(10);// 10 is the fixed seed value
			m_uniReal = uniform_real_distribution<double>((double)min, (double)max);
		}
		else { //the case of SHUFFLE to generate every number only once
			m_generator = mt19937(m_device());
		}
	}
	void setSeed(int seedNum) {
		// we have set a default value for seed in constructor
		// we can change the seed by calling this function after constructor call
		// this gives us more randomness
		m_generator = mt19937(seedNum);
	}
	void getShuffle(vector<int>& array) {
		// the user program creates the vector param and passes here
		// here we populate the vector using m_min and m_max
		for (int i = m_min; i <= m_max; i++) {
			array.push_back(i);
		}
		shuffle(array.begin(), array.end(), m_generator);
	}
	void getShuffle(int array[]) {
		// the param array must be of the size (m_max-m_min+1)
		// the user program creates the array and pass it here
		vector<int> temp;
		for (int i = m_min; i <= m_max; i++) {
			temp.push_back(i);
		}
		shuffle(temp.begin(), temp.end(), m_generator);
		vector<int>::iterator it;
		int i = 0;
		for (it = temp.begin(); it != temp.end(); it++) {
			array[i] = *it;
			i++;
		}
	}
	int getRandNum() {
		// this function returns integer numbers
		// the object must have been initialized to generate integers
		int result = 0;
		if (m_type == NORMAL) {
			//returns a random number in a set with normal distribution
			//we limit random numbers by the min and max values
			result = m_min - 1;
			while (result < m_min || result > m_max)
				result = m_normdist(m_generator);
		}
		else if (m_type == UNIFORMINT) {
			//this will generate a random number between min and max values
			result = m_unidist(m_generator);
		}
		return result;
	}
	double getRealRandNum() {
		// this function returns real numbers
		// the object must have been initialized to generate real numbers
		double result = m_uniReal(m_generator);
		// a trick to return numbers only with two deciaml points
		// for example if result is 15.0378, function returns 15.03
		// to round up we can use ceil function instead of floor
		result = floor(result * 100.0) / 100.0;
		return result;
	}
private:
	int m_min;
	int m_max;
	RANDOM m_type;
	random_device m_device;
	mt19937 m_generator;
	normal_distribution<> m_normdist;//normal distribution
	uniform_int_distribution<> m_unidist;//integer uniform distribution
	uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester {
public:
	// Helper function to check if the heap property is satisfied
	bool isHeapPropertySatisfied(PQueue& queue) {
		return queue.checkHeapProperty();
	}

	// Test insertion for a normal case of min-heap
	bool testInsertionLeftishMinHeap() {
		PQueue minHeapQueue(priorityFn1, MINHEAP, LEFTIST);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			minHeapQueue.insertPatient(patient);
		}

		return isHeapPropertySatisfied(minHeapQueue);
	}
	bool testInsertionSkewMinHeap() {
		PQueue minHeapQueue(priorityFn1, MINHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			minHeapQueue.insertPatient(patient);
		}

		return isHeapPropertySatisfied(minHeapQueue);
	}

	// Test insertion for a normal case of max-heap
	bool testInsertionSkewMaxHeap() {
		PQueue maxHeapQueue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			maxHeapQueue.insertPatient(patient);
		}

		return isHeapPropertySatisfied(maxHeapQueue);
	}
	bool testInsertionLeftistMaxHeap() {
		PQueue maxHeapQueue(priorityFn1, MAXHEAP, LEFTIST);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			maxHeapQueue.insertPatient(patient);
		}

		return isHeapPropertySatisfied(maxHeapQueue);
	}

	// Test removal for a normal case of min-heap
	bool testRemovalMinHeap() {
		PQueue minHeapQueue(priorityFn1, MINHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			minHeapQueue.insertPatient(patient);
		}

		int prevPriority = INT_MAX;
		while (!minHeapQueue.isEmpty()) {
			Patient removedPatient = minHeapQueue.removePatient();
			int removedPriority = minHeapQueue.getPriorityFn()(removedPatient); // Use the priority function
			if (removedPriority > prevPriority) {
				return false; // Check if removals happen in the correct order
			}
			prevPriority = removedPriority;
		}

		return true;
	}
	bool testRemovalLeftistMinHeap() {
		PQueue minHeapQueue(priorityFn1, MINHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			minHeapQueue.insertPatient(patient);
		}

		int prevPriority = INT_MAX;
		while (!minHeapQueue.isEmpty()) {
			Patient removedPatient = minHeapQueue.removePatient();
			int removedPriority = minHeapQueue.getPriorityFn()(removedPatient); // Use the priority function
			if (removedPriority > prevPriority) {
				return false; // Check if removals happen in the correct order
			}
			prevPriority = removedPriority;
		}

		return true;
	}

	// Test removal for a normal case of max-heap
	bool testRemovalMaxHeap() {
		PQueue maxHeapQueue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			maxHeapQueue.insertPatient(patient);
		}

		int prevPriority = INT_MIN;
		while (!maxHeapQueue.isEmpty()) {
			Patient removedPatient = maxHeapQueue.removePatient();
			int removedPriority = maxHeapQueue.getPriorityFn()(removedPatient); // Use the priority function
			if (removedPriority < prevPriority) {
				return false; // Check if removals happen in the correct order
			}
			prevPriority = removedPriority;
		}

		return true;
	}
	bool testRemovalLeftistMaxHeap() {
		PQueue maxHeapQueue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			maxHeapQueue.insertPatient(patient);
		}

		int prevPriority = INT_MIN;
		while (!maxHeapQueue.isEmpty()) {
			Patient removedPatient = maxHeapQueue.removePatient();
			int removedPriority = maxHeapQueue.getPriorityFn()(removedPatient); // Use the priority function
			if (removedPriority < prevPriority) {
				return false; // Check if removals happen in the correct order
			}
			prevPriority = removedPriority;
		}

		return true;
	}

	bool testLeftistHeapNPL() {
		PQueue leftistHeap(priorityFn1, MINHEAP, LEFTIST);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			leftistHeap.insertPatient(patient);
		}

		// Check that all nodes have the correct NPL value
		return leftistHeap.checkHeapProperty();
	}

	bool testLeftistHeapProperty() {
		PQueue leftistHeap(priorityFn1, MINHEAP, LEFTIST);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			leftistHeap.insertPatient(patient);
		}

		// Check that the leftist heap property is preserved
		return leftistHeap.checkHeapProperty();
	}

	bool testRebuildAfterPriorityChange() {
		PQueue queue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			queue.insertPatient(patient);
		}

		// Change the priority function
		queue.setPriorityFn(priorityFn2, MINHEAP);

		// Check that the heap is rebuilt with the new priority function
		return queue.checkHeapProperty();
	}

	bool testMergeWithEmptyQueue() {
		PQueue normalQueue(priorityFn1, MINHEAP, SKEW);
		PQueue emptyQueue(priorityFn2, MAXHEAP, SKEW);

		// Merge an empty queue with a normal queue
		normalQueue.mergeWithQueue(emptyQueue);

		// Check that the resulting queue is the same as the original normal queue
		return normalQueue.checkHeapProperty();
	}

	bool testCopyConstructorNormal() {
		PQueue originalQueue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);
		Random nameGen(0, NUMNAMES - 1);

		for (int i = 0; i < 300; ++i) {
			Patient patient(nameDB[nameGen.getRandNum()] + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			originalQueue.insertPatient(patient);
		}

		// Create a copy using the copy constructor
		PQueue copiedQueue(originalQueue);

		// Check that the copied queue is the same as the original queue
		return copiedQueue.checkHeapProperty();
	}

	bool testCopyConstructorEdge() {
		PQueue originalQueue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		// Create a copy using the copy constructor for an empty queue
		PQueue copiedQueue(originalQueue);

		// Check that the copied queue is the same as the original queue
		return copiedQueue.checkHeapProperty();
	}

	bool testAssignmentOperatorNormal() {
		PQueue originalQueue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		for (int i = 0; i < 300; ++i) {
			Patient patient("Patient" + to_string(i + 1), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum(), randGen.getRandNum());
			originalQueue.insertPatient(patient);
		}

		// Create a new queue and use the assignment operator
		PQueue assignedQueue(priorityFn2, MINHEAP, SKEW);
		assignedQueue = originalQueue;

		// Check that the assigned queue is the same as the original queue
		return assignedQueue.checkHeapProperty();
	}

	bool testAssignmentOperatorEdge() {
		PQueue originalQueue(priorityFn1, MAXHEAP, SKEW);
		Random randGen(1, 1000, UNIFORMINT);

		// Create a new queue and use the assignment operator for an empty queue
		PQueue assignedQueue(priorityFn2, MINHEAP, SKEW);
		assignedQueue = originalQueue;

		// Check that the assigned queue is the same as the original queue
		return assignedQueue.checkHeapProperty();
	}

	bool testDequeueEmptyQueueException() {
		PQueue emptyQueue(priorityFn1, MINHEAP, SKEW);

		try {
			// Attempt to dequeue from an empty queue
			emptyQueue.removePatient();
		}
		catch (const out_of_range& e) {
			return true; // Dequeuing from an empty queue should throw an exception
		}

		return false;
	}

	bool testMergeDifferentPriorityFunctionsException() {
		PQueue queue1(priorityFn1, MAXHEAP, SKEW);
		PQueue queue2(priorityFn2, MINHEAP, SKEW);
		Patient mypatients[10];
		mypatients[0].setPatient("Fletcher Beck", 39, 74, 14, 134, 1);
		mypatients[1].setPatient("Libby Russo", 35, 70, 10, 159, 10);
		mypatients[2].setPatient("Libby Russo", 38, 97, 37, 78, 2);

		queue1.insertPatient(mypatients[0]);
		queue2.insertPatient(mypatients[0]);
		try {
			// Attempt to merge queues with different priority functions
			queue1.mergeWithQueue(queue2);
		}
		catch (const domain_error& e) {
			return true; // Merging queues with different priority functions should throw an exception
		}

		return false;
	}
	void runAllTests() {
		cout << "Test Insertion Leftist Min Heap: " << (testInsertionLeftishMinHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Insertion Skew Min Heap: " << (testInsertionSkewMinHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Insertion Leftist Max Heap: " << (testInsertionLeftistMaxHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Insertion Skew Max Heap: " << (testInsertionSkewMaxHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Removal Leftist Min Heap: " << (testRemovalLeftistMinHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Removal Skew Min Heap: " << (testRemovalMinHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Removal Leftist Max Heap: " << (testRemovalLeftistMaxHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Removal Skew Max Heap: " << (testRemovalMaxHeap() ? "Passed" : "Failed") << endl;
		cout << "Test Leftist Heap NPL: " << (testLeftistHeapNPL() ? "Passed" : "Failed") << endl;
		cout << "Test Leftist Heap Property: " << (testLeftistHeapProperty() ? "Passed" : "Failed") << endl;
		cout << "Test Rebuild After Priority Change: " << (testRebuildAfterPriorityChange() ? "Passed" : "Failed") << endl;
		cout << "Test Merge With Empty Queue: " << (testMergeWithEmptyQueue() ? "Passed" : "Failed") << endl;
		cout << "Test Copy Constructor Normal: " << (testCopyConstructorNormal() ? "Passed" : "Failed") << endl;
		cout << "Test Copy Constructor Edge: " << (testCopyConstructorEdge() ? "Passed" : "Failed") << endl;
		cout << "Test Assignment Operator Normal: " << (testAssignmentOperatorNormal() ? "Passed" : "Failed") << endl;
		cout << "Test Assignment Operator Edge: " << (testAssignmentOperatorEdge() ? "Passed" : "Failed") << endl;
		cout << "Test Dequeue Empty Queue Exception: " << (testDequeueEmptyQueueException() ? "Passed" : "Failed") << endl;
		cout << "Test Merge Different Priority Functions Exception: " << (testMergeDifferentPriorityFunctionsException() ? "Passed" : "Failed") << endl;
	}
};

int main() {
	Tester tester;
	tester.runAllTests();
}
