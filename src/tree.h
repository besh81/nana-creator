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
		auto p = new tree_node<T>(this);
		p->value = elem;

		return append(p);
	}
	tree_node* append(tree_node* new_node)
	{
		auto new_node_ptr = &(this->child);
		if(this->child)
		{
			auto last = this->child;
			while(last->next)
				last = last->next;

			new_node_ptr = &(last->next);
		}

		new_node->owner = this;

		*new_node_ptr = new_node;
		return *new_node_ptr;
	}


	tree_node* insert_before(const T& elem)
	{
		auto p = new tree_node<T>(this);
		p->value = elem;

		return insert_before(p);
	}
	tree_node* insert_before(tree_node* new_node)
	{
		new_node->owner = owner;
		new_node->next = this;

		if(!owner)
			// the inserted node is the first of the chain
			return new_node;

		auto t = owner->child;
		if(t == this)
		{
			// this is the 1st sibling
			owner->child = new_node;
			return new_node;
		}

		while(t->next != this)
			t = t->next;

		t->next = new_node;
		return new_node;
	}


	tree_node* insert_after(const T& elem)
	{
		auto p = new tree_node<T>(this);
		p->value = elem;

		return insert_after(p);
	}
	tree_node* insert_after(tree_node* new_node)
	{
		new_node->owner = owner;
		new_node->next = next;
		next = new_node;
		return new_node;
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
		if(!owner)
			return false;

		if(!next)
			// this is the last sibling: nothing to do
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


	bool detach()
	{
		if(!owner)
			return false;

		auto t = owner->child;
		if(t == this)
		{
			// this is the 1st sibling
			owner->child = next;
			owner = next = 0;
			return true;
		}

		while(t->next != this)
			t = t->next;

		t->next = next;
		owner = next = 0;
		return true;
	}


	unsigned pos() const
	{
		unsigned pos = 0;
		if(owner)
		{
			auto prev = owner->child;
			while(prev != this)
			{
				prev = prev->next;
				++pos;
			}
		}
		return pos;
	}


	unsigned level() const
	{
		unsigned indent = 0;
		if(owner)
		{
			auto node = owner;
			while(node->owner)
			{
				node = node->owner;
				++indent;
			}
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


	bool empty()
	{
		return _root.child ? false : true;
	}


	void clear()
	{
		while(_root.child)
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


	node_type* append(node_type* new_node)
	{
		return append(nullptr, new_node);
	}

	node_type* append(node_type* node, node_type* new_node)
	{
		if(nullptr == node)
			node = &_root;
		else if(!verify(node))
			return nullptr;

		return node->append(new_node);
	}


	node_type* insert_before(node_type* node, const element_type& elem)
	{
		if(verify(node))
			return node->insert_before(elem);
		return nullptr;
	}

	node_type* insert_before(node_type* node, node_type* new_node)
	{
		if(verify(node))
			return node->insert_before(new_node);
		return nullptr;
	}


	node_type* insert_after(node_type* node, const element_type& elem)
	{
		if(verify(node))
			return node->insert_after(elem);
		return nullptr;
	}

	node_type* insert_after(node_type* node, node_type* new_node)
	{
		if(verify(node))
			return node->insert_after(new_node);
		return nullptr;
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


	bool detach(node_type* node)
	{
		if(verify(node))
			return node->detach();
		return false;
	}


	unsigned level(const node_type* node) const
	{
		if(verify(node))
			return node->level();
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


	void recursive_backward(std::function<bool(node_type* node)> action)
	{
		_recursive_backward(_root.child, action);
	}

	void recursive_backward(node_type* node, std::function<bool(node_type* node)> action)
	{
		if(!node)
			return;

		if(!_recursive_backward(node->child, action))
			return;

		action(node);
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


	bool _recursive_backward(node_type* node, std::function<bool(node_type* node)> action)
	{
		if(!node)
			return true;

		if(!_recursive_backward(node->child, action))
			return false;

		if(!_recursive_backward(node->next, action))
			return false;

		return action(node);
	}


	mutable node_type _root{ 0 };
};

#endif //_TREE_H
