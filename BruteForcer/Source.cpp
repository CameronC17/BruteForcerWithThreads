// ASCI CODES ALL FROM HERE
// https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

#define START_CHAR 33 // !
#define END_CHAR 127 // ~

using namespace std;


class Lock {
public:
	Lock(string code) {
		_code = code;
	}
	bool tryCode(string code) {
		return (_code.compare(code) == 0);
	}
private:
	string _code;
};

/*void incrementChar(string* attempt, int index) {
	if (attempt->at(index) == 57) { attempt->at(index) = 64; };
	if (attempt->at(index) == 90) { attempt->at(index) = 96; };
	attempt->at(index)++;
}*/

void incrementStringAtCaps(string* attempt) {
	for (size_t i = 0; i < attempt->length(); i++) {
		if (attempt->at(i) >= END_CHAR) {
			//just to make sure we dont go out of the bounds of the string
			if (i < attempt->length() - 1) {
				attempt->at(i) = START_CHAR;
				attempt->at(i + 1)++;
			}
		}
	}
	attempt->front()++;
}

void crack(Lock* lock, int length) {
	bool foundPassword = false;
	string attempt = "";
	attempt.insert(0, length, START_CHAR);
	
	//if the last letter is ~, it means we've ran out of iterations
	while (attempt.back() < END_CHAR) {
		if (lock->tryCode(attempt)) {
			cout << "Found it!" << endl;
			foundPassword = true;
			break;
		}
		else {
			incrementStringAtCaps(&attempt);
		}
	}

	if (foundPassword) {
		cout << "Password found, it is: " << attempt << endl;
		// stop all threads
	}
	else {
		// terminate this one thread
	}
}

struct thrd {
	thread t;
	thrd(int length, Lock* lock) {
		t = thread(crack, lock, length);
	}

	void join() {
		t.join();
	}
};

class Threadpool {
public:
	Threadpool(Lock* lock, int size) : _lock(lock) {
		//_pool.resize(size);
		//fill(_pool.begin(), _pool.end(), new thrd(3, lock));
		for (int i = 1; i <= size; i++) {
			_pool.push_back(new thrd(i, lock));
		}

		for_each(_pool.begin(), _pool.end(), bind(&thrd::join, placeholders::_1));
	}
	~Threadpool() {
		cout << "delet this .O--r " << endl;
		_pool.clear();
	}
private:
	Lock* _lock;
	vector<thrd*> _pool;
};

int main() {
	//start time
	 clock_t start = clock();
	//----------------------------------------------------------------------------------------------------------------
	
	// -- easy
	Lock lock = Lock("C@m");
	// -- medium
	//Lock lock = Lock("Zr5VVP");
	// -- hard
	//Lock lock = Lock("kL3unCZw5VVx");

	cout << "Trying to find password..." << endl;
	Threadpool* tp = new Threadpool(&lock, 4);
	//delete tp;

	//----------------------------------------------------------------------------------------------------------------
	//give us the time
	double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "That took " << duration << " seconds." << endl;
}