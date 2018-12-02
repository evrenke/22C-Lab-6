// Hash node class template
template <typename K, typename V>
class HashNode {
public:
	HashNode(const K &key, const V &value) :
		key(key), value(value), next(nullptr){
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

	void setNext(HashNode *next) {
		HashNode::next = next;
	}

private:
	// key-value pair
	K key;
	V value;
	HashNode<K, V> *next;
	// next bucket with the same key
};