/*
 *		Tree Container class implementation
 *
 *      Author: besh81
 */

#ifndef _TREE_H
#define _TREE_H

#include <functional>


template<typename T>
struct tree_node
{
	typedef T	value_type;
	value_type	value;

	tree_node*	owner;
	tree_node*	next{ nullptr };
	tree_node*	child{ nullptr };

	tree_node(tree_node* owner)
		:owner(owner)
	{}


	~tree_node()
	{
		if(owner)
		{
			auto t = owner->child;
			if(t != this)
			{
				while(t->next != this)
					t = t->next;
				t->next = next;
			}
			else
				owner->child = next;
		}

		auto t = child;
		while(t)
		{
			auto t_next = t->next;
			delete t;
			t = t_next;
		}
	}


	tree_node* append(const T& elem)
	{
		auto new_node_ptr = &(this->child);
		if(this->child)
		{
			auto last = this->child;
			while(last->next)
				last = last->next;

			new_node_ptr = &(last->next);
		}

		*new_node_ptr = new tree_node<T>(this);

		(*new_node_ptr)->value = elem;
		return (*new_node_ptr);
	}


	bool move_before()
	{
		if(!owner)
			return false;

		auto t = owner->child;
		if(t == this)
			// this is the 1st sibling: nothing to do
			return false;

		if(t->next == this)
		{
			// this is the 2nd sibling
			t->next = next;
			next = t;
			owner->child = this;
			return true;
		}

		while(t->next->next != this)
			t = t->next;

		t->next->next = next;
		next = t->next;
		t->next = this;
		return true;
	}


	bool move_after()
	{
		if(!next)
			// this is the last sibling: nothing to do
			return false;

		if(!owner)
			return false;

		auto t = owner->child;
		if(t == this)
		{
			// this is the 1st sibling
			owner->child = next;
			next = owner->child->next;
			owner->child->next = this;
			return true;
		}

		while(t->next != this)
			t = t->next;

		t->next = next;
		next = t->next->next;
		t->next->next = this;
		return true;
	}


	unsigned pos() const
	{
		if(!owner)
			return 0;

		unsigned pos = 0;
		auto prev = owner->child;
		while(prev != this)
		{
			prev = prev->next;
			++pos;
		}
		return pos;
	}


	unsigned level() const
	{
		unsigned indent = 0;
		auto node = owner;
		while(node->owner)
		{
			node = node->owner;
			++indent;
		}
		return indent;
	}
};


template<typename UserData>
class tree
{
	typedef tree self_type;

public:
	typedef UserData						element_type;
	typedef tree_node<element_type>			node_type;
	typedef typename node_type::value_type	value_type;

	tree()
	{}


	~tree()
	{
		clear();
	}


	void clear()
	{
		remove(_root.child);
	}


	bool verify(const node_type* node) const
	{
		if(node)
		{
			while(node->owner)
			{
				if(node->owner == &_root)
					return true;

				node = node->owner;
			}
		}
		return false;
	}


	node_type* get_root() const
	{
		return &_root;
	}


	node_type* get_owner(const node_type* node) const
	{
		return (verify(node) && (node->owner != &_root) ? node->owner : nullptr);
	}


	node_type* append(const element_type& elem)
	{
		return append(nullptr, elem);
	}


	node_type* append(node_type* node, const element_type& elem)
	{
		if(nullptr == node)
			node = &_root;
		else if(!verify(node))
			return nullptr;

		return node->append(elem);
	}


	void remove(node_type* node)
	{
		if(verify(node))
			delete node;
	}


	bool move_before_sibling(node_type* node)
	{
		if(verify(node))
			return node->move_before();
		return false;
	}


	bool move_after_sibling(node_type* node)
	{
		if(verify(node))
			return node->move_after();
		return false;
	}


	unsigned level(const node_type* node) const
	{
		if(verify(node))
		{
			return node->level();
		}
		return 0;
	}


	void for_each(std::function<bool(node_type* node)> action)
	{
		_for_each(_root.child, action);
	}


	void for_each(node_type* node, std::function<bool(node_type* node)> action)
	{
		if(!node)
			return;

		if(!action(node))
			return;

		_for_each(node->child, action);
	}


private:
	bool _for_each(node_type* node, std::function<bool(node_type* node)> action)
	{
		if(!node)
			return true;

		if(!action(node))
			return false;

		if(!_for_each(node->child, action))
			return false;

		return _for_each(node->next, action);
	}

	mutable node_type _root{ 0 };
};

#endif //_TREE_H
