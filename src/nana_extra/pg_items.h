/*
 *		nana::pg_items Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PG_ITEMS_H
#define NANA_CREATOR_PG_ITEMS_H

#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include "propgrid.h"


namespace nana
{
	using pgitem = drawerbase::propertygrid::pgitem;


	/// helper functions
	nana::color to_color(const std::string& s, bool& inherited);
	std::string to_string(const nana::color& c, bool inherited);


	/// class pg_string
	class pg_string
		: public pgitem
	{
	public:
		pg_string() = default;

		pg_string(const std::string& label, const std::string& value)
			: pgitem(label, value)
		{}

		virtual void value(const std::string& value) override;
		virtual std::string value() const override;

		virtual void set_accept(std::function<bool(wchar_t)> f);

	protected:
		virtual void create(window wd) override;

		virtual bool draw_value(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const override;

		mutable ::nana::textbox	txt_;
	};
			
			
	/// class pg_string_int
	class pg_string_int
		: public pg_string
	{
	public:
		pg_string_int() = default;

		pg_string_int(const std::string& label, const std::string& value)
			: pg_string(label, value)
		{}

		virtual void value(int value);
		virtual int to_int() const;

	protected:
		virtual void create(window wd) override;
	};


	/// class pg_string_uint
	class pg_string_uint
		: public pg_string
	{
	public:
		pg_string_uint() = default;

		pg_string_uint(const std::string& label, const std::string& value)
			: pg_string(label, value)
		{}

		virtual void value(unsigned value);
		virtual unsigned to_uint() const;

	protected:
		virtual void create(window wd) override;
	};


	/// class pg_choice
	class pg_choice
		: public pgitem
	{
	public:
		pg_choice() = default;

		pg_choice(const std::string& label)
			: pgitem(label)
		{}

		virtual void value(const std::string& value) override;
		virtual std::string value() const override;

		virtual void option(std::size_t value);
		virtual std::size_t option() const;

		virtual void push_back(const std::string& option);

	protected:
		virtual void create(window wd) override;

		virtual bool draw_value(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const override;

		mutable ::nana::combox	cmb_;
	};


	/// class pg_check
	class pg_check
		: public pgitem
	{
	public:
		pg_check() = default;

		pg_check(const std::string& label, bool value)
			: pgitem(label)
		{
			value_ = value ? "true" : "false";
		}

		virtual void value(const std::string& value) override;
		virtual std::string value() const override;

		virtual void check(bool value);
		virtual bool checked() const;

	protected:
		virtual void create(window wd) override;

		virtual bool draw_value(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const override;

		mutable ::nana::checkbox	chk_;
	};


	/// class pg_spin
	class pg_spin
		: public pgitem
	{
	public:
		pg_spin() = default;

		pg_spin(const std::string& label)
			: pgitem(label)
		{}

		virtual void value(const std::string& value) override;
		virtual std::string value() const override;

		virtual void value(int value);
		virtual int to_int() const;

		virtual void range(int begin, int last, int step);

	protected:
		virtual void create(window wd) override;

		virtual bool draw_value(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const override;

		mutable ::nana::spinbox	spn_;
	};


	/// class pg_color
	class pg_color
		: public pgitem
	{
	public:
		pg_color() = default;

		pg_color(const std::string& label, const std::string& value, bool inherited = false);

		virtual void value(const std::string& value) override;
		virtual std::string value() const override;

		virtual void value(::nana::color value);
		virtual ::nana::color to_color() const;

		virtual void inherited(bool value);
		virtual bool inherited() const;

		virtual unsigned size() const override;

	protected:
		virtual void create(window wd) override;

		virtual bool draw_value(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const override;

		mutable ::nana::textbox		txt_;
		mutable ::nana::checkbox	chk_;
		::nana::color	color_;
		bool			inherited_{ false };
		bool			inherited_value_{ false };
	};
}//end namespace nana

#endif //NANA_CREATOR_PG_ITEMS_H
