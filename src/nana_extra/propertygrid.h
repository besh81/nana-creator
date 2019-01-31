/*
 *		nana::propertygrid Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROPERTYGRID_H
#define NANA_CREATOR_PROPERTYGRID_H

#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/detail/inline_widget.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/pat/abstract_factory.hpp>
#include <functional>



namespace nana
{
	class propertygrid;

	namespace drawerbase
	{
		namespace propertygrid
		{
			/// usefull for both absolute and display (sorted) positions
			struct index_pair
			{
				std::size_t cat;	//pos of category
				std::size_t item;	//pos of item inside category

				index_pair(std::size_t cat_pos = npos, std::size_t item_pos = npos)
					: cat(cat_pos), item(item_pos)
				{}

				bool empty() const
				{
					return (npos == cat);
				}

				void reset()
				{
					cat = item = npos;
				}

				void set_both(std::size_t n)
				{
					cat = item = n;
				}

				bool is_category() const
				{
					return (npos != cat && npos == item);
				}

				bool is_item() const
				{
					return (npos != cat && npos != item);
				}

				bool operator==(const index_pair& r) const
				{
					return (r.cat == cat && r.item == item);
				}

				bool operator!=(const index_pair& r) const
				{
					return !this->operator==(r);
				}

				bool operator>(const index_pair& r) const
				{
					return (cat > r.cat) || (cat == r.cat && item > r.item);
				}
			};


			/// struct essence_t
			///@brief:	this struct gives many data for propertygrid,
			///			the state of the struct does not effect on member funcions, therefore all data members are public.
			struct essence_t;


			/// class pgitem
			class pgitem
			{
			public:
				pgitem() = default;

				pgitem(const std::string& label, const std::string& value = "")
					: label_(label), value_(value), def_(value)
				{}

				void init(window wd);

				//Creates inline widget
				//pgitem calls this method to create the widget
				//position and size of widget can be ignored in this process
				virtual void create(window wd) = 0;

				//Activate inline widget
				virtual void activate(essence_t* ess, const index_pair& idx)
				{
					ess_ = ess;
					idx_ = idx;
				}

				virtual void label(const std::string& label)
				{
					label_ = label;
				}
				virtual std::string label() const
				{
					return label_;
				}
				virtual void value(const std::string& value)
				{
					if(value_ == value)
						return;
					value_ = value;
					update();
				}
				virtual std::string value() const
				{
					return value_;
				}
				virtual void defvalue(const std::string& value)
				{
					if(def_ == value)
						return;
					def_ = value;
					update();
				}
				virtual std::string defvalue() const
				{
					return def_;
				}
				virtual bool isdefault() const
				{
					return def_ == value_;
				}

				virtual unsigned size() const
				{
					return size_;
				}

				virtual void typeface_changed(unsigned text_height)
				{
					size_ = text_height + 10;
				}

				virtual void draw(paint::graphics* graph, rectangle area, unsigned labelw, unsigned  valuew, unsigned  iboxw, const int txtoff, color bgcolor, color fgcolor) const;


				void update();
				void scroll();

				//Emit event
				virtual void emit_event();

				virtual void enabled(bool state)
				{
					en_ = state;
				}
				virtual bool enabled()
				{
					return en_;
				}

			protected:
				//Draw the label of the sub item
				virtual void draw_label(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const;

				//Draw the value of the sub item
				virtual void draw_value(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const;

				//Draw the interaction box of the sub item
				virtual void draw_ibox(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const;

				std::string		label_, value_, def_;
				unsigned		size_{ 24 };
				bool			en_{ true };
				bool			evt_emit_{ true };

				essence_t*		ess_;
				index_pair		idx_;

				mutable nana::panel<true>	box_;
			};

			typedef std::unique_ptr<pgitem>	pgitem_ptr;


			struct category_t;
			class drawer_lister_impl;

			/// mostly works on display positions
			class trigger : public drawer_trigger
			{
			public:
				trigger();
				~trigger();
				essence_t& essence() const;
			private:
				void _m_draw_border();
			private:
				void attached(widget_reference, graph_reference)	override;
				void detached()	override;
				void typeface_changed(graph_reference)	override;
				void refresh(graph_reference)	override;
				void mouse_move(graph_reference, const arg_mouse&)	override;
				void mouse_leave(graph_reference, const arg_mouse&)	override;
				void mouse_down(graph_reference, const arg_mouse&)	override;
				void mouse_up(graph_reference, const arg_mouse&)	override;
				void mouse_wheel(graph_reference, const arg_wheel&)	override;
				void click(graph_reference, const arg_click&)	override;
				void resized(graph_reference, const arg_resized&)		override;
				void key_press(graph_reference, const arg_keyboard&)	override;

			private:
				essence_t* essence_;
				drawer_lister_impl* drawer_lister_;
			};//end class trigger

			  /// operate with absolute positions and contain only the position but mantain pointers to parts of the real items 
			  /// item_proxy self, it references and iterators are not invalidated by sort()
			class item_proxy
				: public std::iterator<std::input_iterator_tag, item_proxy>
			{
			public:
				item_proxy(essence_t*);
				item_proxy(essence_t*, const index_pair&);

				bool empty() const;

				index_pair pos() const;

				item_proxy& label(const std::string& label);
				std::string label() const;

				item_proxy& value(const std::string& value, bool emit = false);
				std::string value() const;

				item_proxy& defvalue(const std::string& value);
				std::string defvalue() const;

				bool enabled();
				void enabled(bool state);

				/// Behavior of Iterator's value_type
				bool operator==(const char* s) const;
				bool operator==(const ::std::string& s) const;

				/// Behavior of Iterator
				item_proxy& operator=(const item_proxy&);

				/// Behavior of Iterator
				item_proxy& operator++();

				/// Behavior of Iterator
				item_proxy	operator++(int);

				/// Behavior of Iterator
				item_proxy& operator*();

				/// Behavior of Iterator
				const item_proxy& operator*() const;

				/// Behavior of Iterator
				item_proxy* operator->();

				/// Behavior of Iterator
				const item_proxy* operator->() const;

				/// Behavior of Iterator
				bool operator==(const item_proxy&) const;

				/// Behavior of Iterator
				bool operator!=(const item_proxy&) const;

				pgitem* _m_pgitem() const;

			private:
				//Undocumented method
				essence_t * _m_ess() const;
			
				pgitem& _m_property() const;

				essence_t* ess_;
				category_t*	cat_{ nullptr };
				index_pair	pos_;
			};

			class cat_proxy
				: public std::iterator < std::input_iterator_tag, cat_proxy >
			{
			public:
				cat_proxy() = default;
				cat_proxy(essence_t*, std::size_t pos);
				cat_proxy(essence_t*, category_t*);

				/// Appends one item at the end of this category
				item_proxy append(pgitem_ptr p);

				cat_proxy& text(std::string);
				std::string text() const;

				/// Behavior of a container
				item_proxy begin() const;
				item_proxy end() const;
				item_proxy cbegin() const;
				item_proxy cend() const;

				item_proxy at(std::size_t pos) const;
				item_proxy back() const;

				/// this cat position
				std::size_t position() const;

				/// Returns the number of items
				std::size_t size() const;

				/// Behavior of Iterator
				cat_proxy& operator=(const cat_proxy&);

				/// Behavior of Iterator
				cat_proxy& operator++();

				/// Behavior of Iterator
				cat_proxy	operator++(int);

				/// Behavior of Iterator
				cat_proxy& operator*();

				/// Behavior of Iterator
				const cat_proxy& operator*() const;

				/// Behavior of Iterator
				cat_proxy* operator->();

				/// Behavior of Iterator
				const cat_proxy* operator->() const;

				/// Behavior of Iterator
				bool operator==(const cat_proxy&) const;

				/// Behavior of Iterator
				bool operator!=(const cat_proxy&) const;

			private:
				void _m_cat_by_pos();
				void _m_update();
			private:
				essence_t*	ess_{ nullptr };
				category_t*	cat_{ nullptr };
				std::size_t	pos_{ 0 };  ///< Absolute position, not relative to display, and dont change during sort()
			};
		}
	}//end namespace drawerbase


	struct arg_propertygrid
		: public event_arg
	{
		mutable drawerbase::propertygrid::item_proxy item;

		arg_propertygrid(const drawerbase::propertygrid::item_proxy& ip) noexcept
			: item(ip)
		{}
	};


	namespace drawerbase
	{
		namespace propertygrid
		{
			struct propertygrid_events
				: public general_events
			{
				basic_event<arg_propertygrid> property_changed;
			};

			struct scheme
				: public widget_geometrics
			{
				unsigned max_header_width{ 3000 };
				unsigned ext_w{ 5 };
			};
		}
	}//end namespace drawerbase

	 /*! \class propertygrid
	 */
	class propertygrid
		: public widget_object<category::widget_tag, drawerbase::propertygrid::trigger, drawerbase::propertygrid::propertygrid_events, drawerbase::propertygrid::scheme>
	{
	public:
		using index_pair = drawerbase::propertygrid::index_pair;
		using cat_proxy = drawerbase::propertygrid::cat_proxy;
		using item_proxy = drawerbase::propertygrid::item_proxy;
		using pgitem = drawerbase::propertygrid::pgitem;
		using pgitem_ptr = drawerbase::propertygrid::pgitem_ptr;

		propertygrid() = default;
		propertygrid(window, bool visible);
		propertygrid(window, const rectangle& = {}, bool visible = true);

		bool enabled() { return _en; }
		void enabled(bool state);

		void auto_draw(bool);										///< Set state: Redraw automatically after an operation

		void scroll(const index_pair& pos, bool as_first = false);	/// Scrolls the view to the selected item


		propertygrid& labels_width(unsigned pixels);
		propertygrid& values_width(unsigned pixels);
		unsigned labels_width() const;
		unsigned values_width() const;
		unsigned labels_auto_width(unsigned max = 3000);
		unsigned values_auto_width(unsigned max = 3000);


		cat_proxy append(std::string);					///< Appends a new category to the end
		cat_proxy insert(cat_proxy, ::std::string);

		cat_proxy at(std::size_t pos) const;
		std::size_t find(std::string) const;			///< Finds category with given name, returns npos if not found

		item_proxy at(const index_pair& idx) const;		/// Returns an item by the specified absolute position

		void clear(std::size_t cat);					///<Removes all the items from the specified category
		void clear();									///<Removes all the items from all categories
		void erase(std::size_t cat);					///<Erases a category
		void erase();									///<Erases all categories.
		void erase(item_proxy);

		std::size_t size_categ() const;					///<Get the number of categories
		std::size_t size_item(std::size_t cat) const;	///<The number of items in category "cat"

	private:
		drawerbase::propertygrid::essence_t & _m_ess() const;

		bool	_en{ true };
	};
}//end namespace nana

#endif //NANA_CREATOR_PROPERTYGRID_H
