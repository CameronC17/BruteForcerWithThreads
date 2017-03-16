#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

struct thrd {
	thread t;
	bool inUse = false;

	void join() {
		cout << "We'll delete this";
		//t.join();
	}
};

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
	for (int i = 0; i < attempt->length(); i++) {
		if (attempt->at(i) >= 126) {
			//just to make sure we dont go out of the bounds of the string
			if (i < attempt->length() - 1) {
				attempt->at(i) = 48;
				attempt->at(i + 1)++;
			}
		}
	}
	attempt->front()++;
}

// ASCI CODES ALL FROM HERE
// https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html
// 33 is !, first ascii char
void crack(Lock* lock, int length) {
	bool foundPassword = false;
	string attempt = "";
	attempt.insert(0, length, 33);
	
	//if the last letter is z, it means we've ran out of iterations
	while (attempt.back() < 127) {
		if (lock->tryCode(attempt)) {
			cout << "Found it!" << endl;
			foundPassword = true;
			break;
		}
		else {
			incrementStringAtCaps(&attempt);
		}
	}

	if (foundPassword)
		cout << "Password found, it is: " << attempt << endl;
	else
		cout << "Unable to match the password." << endl;
}

class Threadpool {
public:
	Threadpool(Lock* lock, int size) : _lock(lock) {
		_pool.resize(size);
		fill(_pool.begin(), _pool.end(), new thrd());
	}
	~Threadpool() {
		cout << "delet this .O--r " << endl;
		for_each(_pool.begin(), _pool.end(), bind(&thrd::join, placeholders::_1));
		_pool.clear();
	}
private:
	Lock* _lock;
	vector<thrd*> _pool;
};

int main() {
	//start time
	clock_t start;
	start = clock();
	//----------------------------------------------------------------------------------------------------------------
	
	//thread t1(func, "beep");
	//t1.join();

	// -- easy
	Lock lock = Lock("C@m");
	// -- medium
	//Lock lock = Lock("Zr5VVP");
	// -- hard
	//Lock lock = Lock("kL3unCZw5VVx");

	//Threadpool* tp = new Threadpool(&lock, 1);
	//delete tp;

	cout << "Trying to find password..." << endl;

	crack(&lock, 3);
	
	//----------------------------------------------------------------------------------------------------------------
	//give us the time
	double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "That took " << duration << " seconds." << endl;
}