// Hash node class template
template <typename K, typename V>
class HashNode {
public:
	HashNode(const K &key, const V &value) :
		key(key), value(value){
	}

	K getKey() const {
		return key;
	}

	V getValue() const {
		return value;
	}

	void setValue(V value) {
		HashNode::value = value;
	}

private:
	// key-value pair
	K key;
	V value;
	// next bucket with the same key
};