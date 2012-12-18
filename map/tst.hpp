#ifndef __TST_HPP__
#define __TST_HPP__

#include <memory>
#include <string>

// author : kisow@nhn.com

template <typename T>
struct sp
{
	typedef std::shared_ptr<T> type;
};

template <typename T>
class TernarySearchTree 
{

	struct Node
	{
		char	c_;
		typename sp<T>::type	value_;

		typename sp<Node>::type	ekid_;
		typename sp<Node>::type	hkid_;
		typename sp<Node>::type	lkid_;
	};

public:
	TernarySearchTree() : root_(new Node()) {}
	virtual ~TernarySearchTree() {}

	Node* found(const std::string& key) const { return traverse<true>(root_, key); }

	T& operator[](const std::string& key)
	{
		Node* node = traverse<false>(root_, key);

		if(!(node->value_)) node->value_.reset(new T());

		return *(node->value_);
	}

private:
	template <bool SRCH_ONLY>
	static Node* traverse(typename sp<Node>::type& start, const std::string& key)
	{
		Node* node;
		typename sp<Node>::type* p = &start;
		auto cit = key.begin();

		while(cit != key.end()) {
			char c = *cit;

			if(!(*p)) {
				if(SRCH_ONLY) {
					return nullptr;
				} else {
					(*p).reset(new Node());
					(*p)->c_ = c;
				}
			}

			if(c == (*p)->c_) {
				node = (*p).get();
				(*p)->c_ = c;
				p = &((*p)->ekid_); 
				++cit;
			} else if(c < (*p)->c_) {
				p = &((*p)->hkid_);
			} else /*if(c > p->c_)*/ {
				p = &((*p)->lkid_); 
			}
		}

		return node;
	}

private:
	typename sp<Node>::type	root_;
};

#endif // end of include guard: __TST_HPP__ 
