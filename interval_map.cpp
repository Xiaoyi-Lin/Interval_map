#include <iostream>
#include <map>
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
    	// INSERT YOUR SOLUTION HERE

        // Case 1. zeor interval
		if (keyBegin>=keyEnd) return;

        // Case 2. empty map
        if (m_map.empty()) {
            if (val != m_valBegin) {
                m_map[keyBegin] = val;
                m_map[keyEnd] = m_valBegin;
            }
            return;
        }

        // Case 3. Drag the last element to the tail
        if (m_map.find(keyEnd)==m_map.end()) {
            auto itTail = m_map.upper_bound(keyEnd);
            if (itTail == m_map.begin()) {
                if (val == m_valBegin) return;
                m_map[keyEnd] = m_valBegin;
            }
            else {
                --itTail;
                if (itTail->second != val) {
                    m_map[keyEnd] = itTail->second;
                    m_map.erase(itTail);
                }
            }
        }

        // Case 4. Erase intervals inbetween
        auto it = m_map.lower_bound(keyBegin);
        while (it!=m_map.end() && it->first < keyEnd) {
            it = m_map.erase(it);
        }

        // Case 5. Merge with previous interval
        it = m_map.lower_bound(keyEnd);
        if (it == m_map.begin()) {
            if (val != m_valBegin) {
                m_map[keyBegin] = val;
                return;
            }
        }
        else {
            it--;
            if (it->second != val) {
                m_map[keyBegin] = val;
                return;
            }
        }
	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}

    void iter() {
        for (auto i=m_map.begin(); i!=m_map.end(); i++) {
            std::cout<<i->first<<" "<<i->second<<std::endl;
        }
    }
};

void IntervalMapTest() {
	interval_map<int, char> my_map('A');

    // Test Case 0: Look up the initial element
    std::cout<<"Test 0: Look up the initial element"<<std::endl;
    std::cout<<my_map[3]<<std::endl<<std::endl;

	// Test Case 1: Assign single interval and check places
	std::cout<<"Test 1: Assign sinle interval"<<std::endl;
	my_map.assign(1, 5, 'B');
	std::cout << "Expected: A, B, B, A\n";
    std::cout << "Actual: " << my_map[0] << ", " << my_map[1] << ", " << my_map[4] << ", " << my_map[5] << "\n\n";
    my_map.iter();

    // Test 2: Overlap the interval with a new value
    std::cout << "Test 2: Overlapping interval [3, 7) with 'C'\n";
    my_map.assign(3, 7, 'C');
    assert(my_map[2] == 'B' && my_map[3] == 'C' && my_map[6] == 'C' && my_map[7] == 'A');
    std::cout << "Expected: B, C, C, A\n";
    std::cout << "Actual: " << my_map[2] << ", " << my_map[3] << ", " << my_map[6] << ", " << my_map[7] << "\n\n";
    my_map.iter();

    // Test 3: Assign an interval with the same begin and end
    std::cout << "Test 3: Assigning interval [10, 10) with 'D' (should be empty and do nothing)\n";
    my_map.assign(10, 10, 'D');
    assert(my_map[9] == 'A' && my_map[10] == 'A');
    std::cout << "Expected: A, A\n";
    std::cout << "Actual: " << my_map[9] << ", " << my_map[10] << "\n\n";
    my_map.iter();

    // Test 4: Assign an interval with the begin greater than end (should do nothing)
    std::cout << "Test 4: Assigning interval [7, 6) with 'E' (should be empty and do nothing)\n";
    my_map.assign(7, 6, 'E');
    assert(my_map[6] == 'C' && my_map[7] == 'A');
    std::cout << "Expected: C, A\n";
    std::cout << "Actual: " << my_map[6] << ", " << my_map[7] << "\n\n";
    my_map.iter();

    // Test 5: Assign a value that extends before the beginning of the map
    std::cout << "Test 5: Assigning interval [-2, 2) with 'F'\n";
    my_map.assign(-2, 2, 'F');
    assert(my_map[-3] == 'A' && my_map[-2] == 'F' && my_map[1] == 'F' && my_map[2] == 'B');
    std::cout << "Expected: A, F, F, B\n";
    std::cout << "Actual: " << my_map[-3] << ", " << my_map[-2] << ", " << my_map[1] << ", " << my_map[2] << "\n\n";
    my_map.iter();

    // Test 6: Assign the tail equals to m_valBegin
    std::cout << "Test 6: Assigning interval [9,15) with 'A'\n";
    my_map.assign(-9, -7, 'A');
    std::cout << "Expected: A, A\n";
    std::cout << "Actual: " << my_map[-9] << ", " << my_map[-7] << "\n\n";
    my_map.iter();

    // Test 6: Overwrite the whole map with a new value
    std::cout << "Test 6: Assigning interval [min, max) of int with 'G'\n";
    my_map.assign(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), 'G');
    assert(my_map[std::numeric_limits<int>::lowest()] == 'G' && my_map[0] == 'G' && my_map[std::numeric_limits<int>::max() - 1] == 'G');
    std::cout << "Expected: G, G, G\n";
    std::cout << "Actual: " << my_map[std::numeric_limits<int>::lowest()] << ", " << my_map[0] << ", " << my_map[std::numeric_limits<int>::max() - 1] << "\n\n";
    my_map.iter();

}

int main() {
	IntervalMapTest();
	std::cout<<"Done"<<std::endl;
	return 0;
}
// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.


