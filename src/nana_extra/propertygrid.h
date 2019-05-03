/*
 *		nana::propertygrid Implementation
 *
 *      part of Nana Creator (https://github.com/besh81/nana-creator)
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROPERTYGRID_H
#define NANA_CREATOR_PROPERTYGRID_H

#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/menu.hpp>


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

				pgitem(const std::string& label, const std::string& value = "")	///< Constructor
					: label_(label), value_(value), def_(value)
				{}

				void init(window wd);							///< Initialize the inline controls (should not be used)

				virtual void activate(essence_t* ess, const index_pair& idx)	///< Activate inline widgets (should not be used)
				{
					ess_ = ess;
					idx_ = idx;
				}

				virtual void label(const std::string& label)	///< Sets item label
				{
					label_ = label;
				}
				virtual std::string label() const				///< Gets item label
				{
					return label_;
				}
				virtual void value(const std::string& value)	///< Sets item value
				{
					if(value_ == value)
						return;
					value_ = value;
					update();
				}
				virtual std::string value() const				///< Gets item value
				{
					return value_;
				}
				virtual void defvalue(const std::string& value)	///< Sets item default value
				{
					if(def_ == value)
						return;
					def_ = value;
					update();
				}
				virtual std::string defvalue() const			///< Gets item default value
				{
					return def_;
				}
				virtual bool isdefault() const					///< Returns true if item value is equal to default value. False otherwise
				{
					return def_ == value_;
				}

				virtual void reset()							///< Resets item value to default
				{
					value(def_);
				}

				virtual unsigned size() const					///< Returns the item (vertical) size
				{
					return size_;
				}

				virtual void tooltip(const std::string& help_text)
				{
					// nothing to be done for the base class
				}

				virtual void set(const std::vector<std::string>& vs)
				{
					// nothing to be done for the base class
				}

				virtual void typeface_changed(unsigned text_height)	///< Inform the item the font is changed (should not be used)
				{
					size_ = text_height + 10;
				}

				virtual void draw(paint::graphics* graph, rectangle area, unsigned labelw, unsigned valuew, unsigned iboxw, const int txtoff, color bgcolor, color fgcolor) const;	///< Draw the item. Position and size of inline widgets should be set here

				void update();		///< Update the item (refresh)
				void scroll();		///< Scrolls the view to show this item

				virtual void emit_event();			///< Emit property_changed event

				virtual void enabled(bool state)	///< Enables/disables the item
				{
					en_ = state;
				}
				virtual bool enabled()				///< Get the enables state of the item
				{
					return en_;
				}

			protected:
				virtual void create(window wd) = 0; ///< Creates the inline widgets. Position and size of widgets can be ignored at this stage

				virtual void draw_label(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const;	///< Draw the label of the item
				virtual void draw_value(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const;	///< Draw the value of the item
				virtual void draw_ibox(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const;	///< Draw the interaction box of the item

				std::string		label_, value_, def_;
				unsigned		size_{ 24 };
				bool			en_{ true };
				bool			evt_emit_{ true };

				essence_t*		ess_;
				index_pair		idx_;

				mutable nana::panel<true>	ibox_;
				mutable nana::menu			menu_;
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
				item_proxy(essence_t*);						///< Constructor
				item_proxy(essence_t*, const index_pair&);	///< Constructor

				bool empty() const;			///< Returns true if item is empty

				index_pair pos() const;		///< Returns the position of this item

				item_proxy& label(const std::string& label);	///< Sets item's label
				std::string label() const;						///< Gets item's label

				item_proxy& value(const std::string& value, bool emit = false);	///< Sets item's value. If emit is set to true then a property_changed event is generated
				item_proxy& operator=(const std::string& new_value_string );    ///< Sets item's value
				std::string value() const;										///< Gets item's value

				item_proxy& defvalue(const std::string& value);	///< Sets item's default value
				std::string defvalue() const;					///< Gets item's default value

				bool enabled();				///< Get the enables state of the item
				item_proxy& enabled(bool state);	///< Enables/disables the item

				/** \brief Provide help in pop-up when mouse hovers over property's value entry field

                @param[in] help_text

                Example Usage:

                <pre>
                    auto cat = pg.append("A category");
                    cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_choice("A property")))->tooltip( "A helpful hint" );
                </pre>
				*/
				item_proxy& tooltip(const std::string& help_text);

                /** \brief Set a vector of strings in the item

                @param[in] vs vector of stings to set in item

                Example Usage:

                <pre>
                    auto cat = pg.append("A category");
                    auto ip  = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_choice("A property")))
                    ip->set({ "A", "B", "C", "D" });
                </pre>
				*/
				item_proxy& set(const std::vector<std::string>& vs);

				// Behavior of Iterator's value_type
				bool operator==(const char* s) const;
				bool operator==(const ::std::string& s) const;

				// Behavior of Iterator
				item_proxy& operator=(const item_proxy&);
				item_proxy& operator++();
				item_proxy	operator++(int);
				item_proxy& operator*();
				const item_proxy& operator*() const;
				item_proxy* operator->();
				const item_proxy* operator->() const;
				bool operator==(const item_proxy&) const;
				bool operator!=(const item_proxy&) const;

				pgitem* _m_pgitem() const;	///< Internal use

			private:
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
				cat_proxy(essence_t*, std::size_t pos);		///< Constructor
				cat_proxy(essence_t*, category_t*);			///< Constructor

				item_proxy append(pgitem_ptr p);	///< Appends passed item at the end of this category

				void expand( bool exp );               ///< Expand or collapse the category

				item_proxy find( const std::string& prop_label ); ///< find item in category with specified label

				cat_proxy& text(std::string);	///< Sets category text
				std::string text() const;		///< Gets category text

				// Behavior of a container
				item_proxy begin() const;
				item_proxy end() const;
				item_proxy cbegin() const;
				item_proxy cend() const;

				item_proxy at(std::size_t pos) const;	///< Returns the item at the specified position
				item_proxy back() const;				///< Returns the last item of this category

				std::size_t position() const;		///< Returns the position of this category

				std::size_t size() const;			///< Returns the number of items

				// Behavior of Iterator
				cat_proxy& operator=(const cat_proxy&);
				cat_proxy& operator++();
				cat_proxy	operator++(int);
				cat_proxy& operator*();
				const cat_proxy& operator*() const;
				cat_proxy* operator->();
				const cat_proxy* operator->() const;

				bool operator==(const cat_proxy&) const;
				bool operator!=(const cat_proxy&) const;

			private:
				void _m_cat_by_pos();
				void _m_update();

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
		propertygrid(window, bool visible);									///< Constructor
		propertygrid(window, const rectangle& = {}, bool visible = true);	///< Constructor

		bool enabled() { return _en; }		///< Get the enables state of the control
		void enabled(bool state);			///< Enables/disables the control

		unsigned labels_min_width() const;					///< Gets the label column min width
		propertygrid& labels_min_width(unsigned pixels);	///< Sets the label column min width
		unsigned values_min_width() const;					///< Gets the value column min width
		propertygrid& values_min_width(unsigned pixels);	///< Sets the value column min width

		/** \brief Show or hide the interactive box
		\param[in] state true if you want the interactive boxes to show, this is the default.
		\param[in] state false to hide the boxes

		The interactive box appear on the left of each property in the grid and provide a pop-up menu for the property.
		*/
		void ibox_show(bool state);
		bool ibox_show() const;

		void auto_draw(bool);										///< Enables/disables automatic drawing

		void scroll(const index_pair& pos, bool as_first = false);	///< Scrolls the view to the selected item

		cat_proxy append(std::string str);						///< Appends a new category to the end
		cat_proxy insert(cat_proxy cat, ::std::string str);		///< Inser a new category before the specified one

		cat_proxy at(std::size_t pos) const;			///< Returns the category at the specified position
		std::size_t find(std::string) const;			///< Finds category with given name. Returns the category position if found or npos otherwise

		item_proxy at(const index_pair& idx) const;		///< Returns an item by the specified absolute position

		/** \brief Find a property by name and category name
            \param[in] catName
            \param[in] propName
            \throw runtime_error if property does not exist
        */
		item_proxy find(
					const std::string& catName,
					const std::string& propName ) const;

		void clear(std::size_t cat);					///< Removes all the items from the specified category
		void clear();									///< Removes all the items from all categories
		void erase(std::size_t cat);					///< Erases specified category
		void erase();									///< Erases all categories
		void erase(item_proxy ip);						///< Erases specified item

		std::size_t size_categ() const;					///< Get the number of categories
		std::size_t size_item(std::size_t cat) const;	///< Get the number of items in the specified category


	private:
		drawerbase::propertygrid::essence_t & _m_ess() const;

		bool	_en{ true };
	};
}//end namespace nana

#endif //NANA_CREATOR_PROPERTYGRID_H
